#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define pf printf
#define ffr(i,a,b) for(i=a;i<b;i++)
#define abinfo b_te_1
#define MY_TIME_OUT 12.0
#define do_file 0


#define A 0
#define B 1

//my findings/observations
// event list is linked list

#define BIDIRECTIONAL 0 /* change to 1 if you're doing extra credit */

struct msg
{
    char data[20];
};


struct pkt
{
    int seqnum;
    int acknum;
    int checksum;
    char payload[20];
};

/********* FUNCTION PROTOTYPES. DEFINED IN THE LATER PART******************/
void starttimer(int B1, float increment);
void stoptimer(int B1);
void tolayer3(int B1, struct pkt packet);
void tolayer5(int B1, char datasent[20]);

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

int findChecksum(struct pkt* packet)
{
	int sum, i = 0;
	sum = packet->seqnum;
	sum += packet->acknum;

	ffr(i,0,20) {
		sum += packet->payload[i];
	}

	return sum;
}

struct pkt encapsulatedPkt(struct msg* message, int ack, int seq)
{
    struct pkt *packet=(struct pkt*)malloc(sizeof(struct pkt));
    int i;

    ffr(i,0,20)
    packet->payload[i]=message->data[i];
    packet->acknum=ack;
    packet->seqnum=seq;
    packet->checksum=findChecksum(packet);
    pf("%d checksum\n", packet->checksum);
    return *packet;

}

#define WAITINGFORDATA 0
#define WAITINGFORACK 1
#define NN 2
#define PLUS(z) (z+1)%NN
#define MINUS(z) (z+NN-1)%NN

struct device
{
    int status;

    int will_send_seq;
    int will_expect_ack;

    int will_send_ack;
    int will_expect_seq;

} AA, BB;

struct pkt lastPacket;

/* called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{
    pf("-------------------A_output--------------------\n");
    if(AA.status==WAITINGFORDATA)
    {
        struct pkt pk = encapsulatedPkt(&message,0/*ignore*/, AA.will_send_seq);
        lastPacket=pk;
        AA.status=WAITINGFORACK;
        AA.will_send_seq=PLUS(AA.will_send_seq);
        tolayer3(0,pk);
        starttimer(0,MY_TIME_OUT);

    }
    return;
}

void B_output(struct msg message)
{
    pf("-------------------B_output--------------------\n");
}

void A_input(struct pkt packet)
{
    pf("-------------------A_input--------------------\n");

    if(AA.status==WAITINGFORACK)
    {
        if(findChecksum(&packet)!=packet.checksum)
        {
            //corrupted
        }
        else
        {
            //stoptimer(A);
            if(packet.acknum==AA.will_expect_ack)
            {
                AA.status=WAITINGFORDATA;
                //tolayer5(A,packet.payload);
                AA.will_expect_ack=PLUS(AA.will_expect_ack);
                pf("Received by A\n");
                stoptimer(0);
            }
            else
            {
                //wrong ack found
                tolayer3(0,lastPacket);
                stoptimer(0);
                starttimer(0,MY_TIME_OUT);
            }
        }
    }
    return;
}

/* called when A's timer goes off */
void A_timerinterrupt(void)
{
    pf("----------------A_timerinterrupt-----------------\n");
    tolayer3(0, lastPacket);
	starttimer(0, MY_TIME_OUT);
}

void A_init(void)
{
    pf("--------------------A_init---------------------\n");
    AA.status=WAITINGFORDATA;
    AA.will_send_seq=0;
    AA.will_expect_ack=0;
}


void B_input(struct pkt packet)
{
    pf("-------------------B_input--------------------\n");

    if(findChecksum(&packet)!=packet.checksum)
    {
        //corrupted
        struct pkt pk;
        pk.acknum=MINUS(BB.will_send_ack);
        pk.checksum=findChecksum(&pk);
        tolayer3(B,pk);
    }
    else
    {
        if(packet.seqnum==BB.will_expect_seq)
        {
            tolayer5(B,packet.payload);
            BB.will_expect_seq=PLUS(BB.will_expect_seq);
            struct pkt pk;
            pk.acknum=(BB.will_send_ack);
            BB.will_send_ack=PLUS(BB.will_send_ack);
            pk.checksum=findChecksum(&pk);
            pf("Received by B\n");
            tolayer3(B,pk);

        }
        else
        {
            //wrong ack found
            struct pkt pk;
            pk.acknum=MINUS(BB.will_send_ack);
            pk.checksum=findChecksum(&pk);
            tolayer3(B,pk);
        }
    }
    return;
}


void B_timerinterrupt(void)
{
    pf("----------------B_timerinterrupt-----------------\n");
}


void B_init(void)
{
    pf("--------------------B_init--------------------\n");
    BB.status=WAITINGFORDATA;
    BB.will_expect_seq=0;
    BB.will_send_ack=0;
}

/*****************************************************************
***************** NETWORK EMULATION CODE STARTS BELOW ***********
The code below emulates the layer 3 and below network environment:
    - emulates the tranmission and delivery (possibly with bit-level corruption
        and packet loss) of packets across the layer 3/4 interface
    - handles the starting/stopping of a timer, and generates timer
        interrupts (resulting in calling students timer handler).
    - generates message to be sent (passed from later 5 to 4)

THERE IS NO REASON THAT ANY STUDENT SHOULD HAVE TO READ OR UNDERSTAND
THE CODE BELOW.  YOU SHOLD NOT TOUCH, OR REFERENCE (in your code) ANY
OF THE DATA STRUCTURES BELOW.  If you're interested in how I designed
the emulator, you're welcome to look at the code - but again, you should have
to, and you defeinitely should not have to modify
******************************************************************/

struct event
{
    float evtime;       /* event time */
    int evtype;         /* event type code */
    int eventity;       /* entity where event occurs */
    struct pkt *pktptr; /* ptr to packet (if any) assoc w/ this event */
    struct event *prev;
    struct event *next;
};
struct event *evlist = NULL; /* the event list */

/* possible events: */
#define TIMER_INTERRUPT 0
#define FROM_LAYER5 1
#define FROM_LAYER3 2

#define OFF 0
#define ON 1

int TRACE = 1;     /* for my debugging */
int nsim = 0;      /* number of messages from 5 to 4 so far */
int nsimmax = 0;   /* number of msgs to generate, then stop */
float time = 0.000;
float lossprob;    /* probability that a packet is dropped  */
float corruptprob; /* probability that one bit is packet is flipped */
float lambda;      /* arrival rate of messages from layer 5 */
int ntolayer3;     /* number sent into layer 3 */
int nlost;         /* number lost in media */
int ncorrupt;      /* number corrupted by media*/

void init();
void generate_next_arrival(void);
void insertevent(struct event *p);

int main()
{
    if(do_file)
    {freopen("out.txt", "w", stdout);}
    struct event *eventptr;
    struct msg msg2give;
    struct pkt pkt2give;

    int i, j;
    char c;

    init();
    A_init();
    B_init();

    while (1)
    {
        eventptr = evlist; /* get next event to simulate */
        if (eventptr == NULL)
            goto terminate;
        evlist = evlist->next; /* remove this event from event list */
        if (evlist != NULL)
            evlist->prev = NULL;
        if (TRACE >= 2)
        {
            pf("\nEVENT time: %f,", eventptr->evtime);
            pf("  type: %d", eventptr->evtype);
            if (eventptr->evtype == 0)
                pf(", timerinterrupt  ");
            else if (eventptr->evtype == 1)
                pf(", fromlayer5 ");
            else
                pf(", fromlayer3 ");
            pf(" entity: %d\n", eventptr->eventity);
        }
        time = eventptr->evtime; /* update time to next event time */
        if (eventptr->evtype == FROM_LAYER5)
        {
            if (nsim < nsimmax)
            {
                if (nsim + 1 < nsimmax)
                    generate_next_arrival(); /* set up future arrival */
                /* fill in msg to give with string of same letter */
                j = nsim % 26;
                for (i = 0; i < 20; i++)
                    msg2give.data[i] = 97 + j;
                msg2give.data[19] = 0;
                if (TRACE > 2)
                {
                    pf("          MAINLOOP: data given to student: ");
                    for (i = 0; i < 20; i++)
                        pf("%c", msg2give.data[i]);
                    pf("\n");
                }
                nsim++;
                if (eventptr->eventity == A)
                    A_output(msg2give);
                else
                    B_output(msg2give);
            }
        }
        else if (eventptr->evtype == FROM_LAYER3)
        {
            pkt2give.seqnum = eventptr->pktptr->seqnum;
            pkt2give.acknum = eventptr->pktptr->acknum;
            pkt2give.checksum = eventptr->pktptr->checksum;
            for (i = 0; i < 20; i++)
                pkt2give.payload[i] = eventptr->pktptr->payload[i];
            if (eventptr->eventity == A) /* deliver packet by calling */
                A_input(pkt2give); /* appropriate entity */
            else
                B_input(pkt2give);
            free(eventptr->pktptr); /* free the memory for packet */
        }
        else if (eventptr->evtype == TIMER_INTERRUPT)
        {
            if (eventptr->eventity == A)
                A_timerinterrupt();
            else
                B_timerinterrupt();
        }
        else
        {
            pf("INTERNAL PANIC: unknown event type \n");
        }
        free(eventptr);
    }

terminate:
    pf(
        " Simulator terminated at time %f\n after sending %d msgs from layer5\n",
        time, nsim);
}

void init() /* initialize the simulator */
{
    int i;
    float sum, avg;
    float jimsrand();

    pf("--------  Stop and Wait Network Simulator Version 1.1 ---------- \n\n");
    pf("Enter the number of messages to simulate: ");
    scanf("%d",&nsimmax);
    pf("Enter packet loss probability [enter 0.0 for no loss]: ");
    scanf("%f",&lossprob);
    pf("Enter packet corruption probability [0.0 for no corruption]: ");
    scanf("%f",&corruptprob);
    pf("Enter average time between messages from sender's layer5 [ > 0.0 ]: ");
    scanf("%f",&lambda);
    pf("Enter TRACE:");
    scanf("%d",&TRACE);

    srand(9999); /* init random number generator */
    sum = 0.0;   /* test random number generator for students */
    for (i = 0; i < 1000; i++)
        sum = sum + jimsrand(); /* jimsrand() should be uniform in [0,1] */
    avg = sum / 1000.0;
    if (avg < 0.25 || avg > 0.75)
    {
        pf("It is likely that random number generation on your machine\n");
        pf("is different from what this emulator expects.  Please take\n");
        pf("a look at the routine jimsrand() in the emulator code. Sorry. \n");
        exit(1);
    }

    ntolayer3 = 0;
    nlost = 0;
    ncorrupt = 0;

    time = 0.0;              /* initialize time to 0.0 */
    generate_next_arrival(); /* initialize event list */
}

/****************************************************************************/
/* jimsrand(): return a float in range [0,1].  The routine below is used to */
/* isolate all random number generation in one location.  We assume that the*/
/* system-supplied rand() function return an int in therange [0,mmm]        */
/****************************************************************************/
float jimsrand(void)
{
    double mmm = RAND_MAX;
    float x;                 /* individual students may need to change mmm */
    x = rand() / mmm;        /* x should be uniform in [0,1] */
    return (x);
}

/********************* EVENT HANDLINE ROUTINES *******/
/*  The next set of routines handle the event list   */
/*****************************************************/

void generate_next_arrival(void)
{
    double x, log(), ceil();
    struct event *evptr;
    float ttime;
    int tempint;

    if (TRACE > 2)
        pf("          GENERATE NEXT ARRIVAL: creating new arrival\n");

    x = lambda * jimsrand() * 2; /* x is uniform on [0,2*lambda] */
    /* having mean of lambda        */
    evptr = (struct event *)malloc(sizeof(struct event));
    evptr->evtime = time + x;
    evptr->evtype = FROM_LAYER5;
    if (BIDIRECTIONAL && (jimsrand() > 0.5))
        evptr->eventity = B;
    else
        evptr->eventity = A;
    insertevent(evptr);
}

void insertevent(struct event *p)
{
    struct event *q, *qold;

    if (TRACE > 2)
    {
        pf("            INSERTEVENT: time is %lf\n", time);
        pf("            INSERTEVENT: future time will be %lf\n", p->evtime);
    }
    q = evlist;      /* q points to header of list in which p struct inserted */
    if (q == NULL)   /* list is empty */
    {
        evlist = p;
        p->next = NULL;
        p->prev = NULL;
    }
    else
    {
        for (qold = q; q != NULL && p->evtime > q->evtime; q = q->next)
            qold = q;
        if (q == NULL)   /* end of list */
        {
            qold->next = p;
            p->prev = qold;
            p->next = NULL;
        }
        else if (q == evlist)     /* front of list */
        {
            p->next = evlist;
            p->prev = NULL;
            p->next->prev = p;
            evlist = p;
        }
        else     /* middle of list */
        {
            p->next = q;
            p->prev = q->prev;
            q->prev->next = p;
            q->prev = p;
        }
    }
}

void printevlist(void)
{
    struct event *q;
    int i;
    pf("--------------\nEvent List Follows:\n");
    for (q = evlist; q != NULL; q = q->next)
    {
        pf("Event time: %f, type: %d entity: %d\n", q->evtime, q->evtype,
               q->eventity);
    }
    pf("--------------\n");
}

/********************** Student-callable ROUTINES ***********************/

/* called by students routine to cancel a previously-started timer */
void stoptimer(int B1 /* A or B is trying to stop timer */) //linked list
{
    struct event *q, *qold;

    if (TRACE > 2)
        pf("          STOP TIMER: stopping timer at %f\n", time);
    /* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
    for (q = evlist; q != NULL; q = q->next)
        if ((q->evtype == TIMER_INTERRUPT && q->eventity == B1))
        {
            /* remove this event */
            if (q->next == NULL && q->prev == NULL)
                evlist = NULL;          /* remove first and only event on list */
            else if (q->next == NULL) /* end of list - there is one in front */
                q->prev->next = NULL;
            else if (q == evlist)   /* front of list - there must be event after */
            {
                q->next->prev = NULL;
                evlist = q->next;
            }
            else     /* middle of list */
            {
                q->next->prev = q->prev;
                q->prev->next = q->next;
            }
            free(q);
            return;
        }
    pf("Warning: unable to cancel your timer. It wasn't running.\n");
}

void starttimer(int B1 /* A or B is trying to start timer */, float increment)
{
    struct event *q;
    struct event *evptr;

    if (TRACE > 2)
        pf("          START TIMER: starting timer at %f\n", time);
    /* be nice: check to see if timer is already started, if so, then  warn */
    /* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
    for (q = evlist; q != NULL; q = q->next)
        if ((q->evtype == TIMER_INTERRUPT && q->eventity == B1))
        {
            pf("Warning: attempt to start a timer that is already started\n");
            return;
        }

    /* create future event for when timer goes off */
    evptr = (struct event *)malloc(sizeof(struct event));
    evptr->evtime = time + increment;
    evptr->evtype = TIMER_INTERRUPT;
    evptr->eventity = B1;
    insertevent(evptr);
}

/************************** TOLAYER3 ***************/
void tolayer3(int B1, struct pkt packet)
{
    struct pkt *mypktptr;
    struct event *evptr, *q;
    float lastime, x;
    int i;

    ntolayer3++;

    /* simulate losses: */
    if (jimsrand() < lossprob)
    {
        nlost++;
        if (TRACE > 0)
            pf("          TOLAYER3: packet being lost\n");
        return;
    }

    /* make a copy of the packet student just gave me since he/she may decide */
    /* to do something with the packet after we return back to him/her */
    mypktptr = (struct pkt *)malloc(sizeof(struct pkt));
    mypktptr->seqnum = packet.seqnum;
    mypktptr->acknum = packet.acknum;
    mypktptr->checksum = packet.checksum;
    for (i = 0; i < 20; i++)
        mypktptr->payload[i] = packet.payload[i];
    if (TRACE > 2)
    {
        pf("          TOLAYER3: seq: %d, ack %d, check: %d ", mypktptr->seqnum,
               mypktptr->acknum, mypktptr->checksum);
        for (i = 0; i < 20; i++)
            pf("%c", mypktptr->payload[i]);
        pf("\n");
    }

    /* create future event for arrival of packet at the other side */
    evptr = (struct event *)malloc(sizeof(struct event));
    evptr->evtype = FROM_LAYER3;      /* packet will pop out from layer3 */
    evptr->eventity = (B1 ^ 1) ; /* event occurs at other entity */
    evptr->pktptr = mypktptr;         /* save ptr to my copy of packet */
    /* finally, compute the arrival time of packet at the other end.
       medium can not reorder, so make sure packet arrives between 1 and 10
       time units after the latest arrival time of packets
       currently in the medium on their way to the destination */
    lastime = time;
    /* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next) */
    for (q = evlist; q != NULL; q = q->next)
        if ((q->evtype == FROM_LAYER3 && q->eventity == evptr->eventity))
            lastime = q->evtime;
    evptr->evtime = lastime + 1 + 9 * jimsrand();

    /* simulate corruption: */
    if (jimsrand() < corruptprob)
    {
        ncorrupt++;
        if ((x = jimsrand()) < .75)
            mypktptr->payload[0] = 'Z'; /* corrupt payload */
        else if (x < .875)
            mypktptr->seqnum = 999999;
        else
            mypktptr->acknum = 999999;
        if (TRACE > 0)
            pf("          TOLAYER3: packet being corrupted\n");
    }

    if (TRACE > 2)
        pf("          TOLAYER3: scheduling arrival on other side\n");
    insertevent(evptr);
}

void tolayer5(int B1, char datasent[20])
{
    int i;
    if (TRACE > 2)
    {
        pf("          TOLAYER5: data received: ");
        for (i = 0; i < 20; i++)
            pf("%c", datasent[i]);
        pf("\n");
    }
}
