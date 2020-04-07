#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define pf printf
#define ffr(i,a,b) for(i=a;i<b;i++)
#define abinfo b_te_1

#define do_file 0
#define DEBUG 0


#define A 0
#define B 1
float time = 0.000;
//my findings/observations
// event list is linked list

#define BIDIRECTIONAL 1 /* change to 1 if you're doing extra credit */

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
void starttimer(int AorB, float increment);
void stoptimer(int AorB);
void tolayer3(int AorB, struct pkt packet);
void tolayer5(int AorB, char datasent[20]);

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


#define WAITINGFORDATA 0
#define WAITINGFORACK 1
#define HIGHEST_SEQ 7
#define MY_TIMEOUT 12.0
#define MY_ACK_TIMEOUT 12.0
#define MAX_BUF 30
#define WIN_LEN 7

#define PLUS(z) (z+1)%WIN_LEN
#define MINUS(z) (z+WIN_LEN-1)%WIN_LEN

#define TPLUS(z) (z+1)%(WIN_LEN+1)
#define TMINUS(z) (z+WIN_LEN)%(WIN_LEN+1)

struct device
{
    int expecting_ack;   //want to get ack this ack number
    int next_seq;        //want to send this seq number
    int expecting_rcv;   //want to rcv this seq nnumber
    int present_packet;  //how many present currently

    struct pkt window_data[WIN_LEN];

    float timer_timeout[WIN_LEN];  //timer queue

    int timer_is_running;  //boolean
    int head_timer;        //timer queue head
    int tail_timer;        //timer queue tail

    int simulator_value;   //entity ID
    int retransmit_head;   //retransmission window head
    int retransmit_tail;   //retransmission window tail

    int retransmit_done;   //boolean, is_required_to_retransmit
    float ack_timeout;

} AA, BB;


/* check if a <= b < c circularly */
int between(int a, int b, int c)
{
	if ((a <= b && b < c)
		|| (c < a && a <= b)
		|| (b < c && c < a))
		return 1;
	else
		return 0;
}

/* start a timer for a packet */
void start_multitimer(device *d, int seqnum)
{
	/* bound check */
	if (d->head_timer == d->tail_timer + 1) {
		printf("Warning: you can't create more than %d timers.\n", WIN_LEN);
		return;
	}
	if (!d->timer_is_running) {	/* if timers isn't running, start the timer right now */
		d->timer_is_running = 1;
        if(seqnum>=0)
		{starttimer(d->simulator_value, MY_TIMEOUT); }
        //else
        //starttimer(d->simulator_value, MY_ACK_TIMEOUT);

	} else {					/* else, add this timer into the queue */
        if(seqnum>=0)
        d->timer_timeout[d->tail_timer]= time + MY_TIMEOUT;
        else
        {
            //d->timer_timeout[d->tail_timer]= time + MY_ACK_TIMEOUT;
        }
        d->tail_timer= PLUS(d->tail_timer);
	}
    if(seqnum==-1) d->ack_timeout= time+MY_ACK_TIMEOUT;
}

/* stop the first timer */
void stop_multitimer(device *d, int seqnum)
{
	/* bound check */

    if(!d->timer_is_running)
    {
        printf("Warning: you are trying to stop a timer isn't running.\n");
		return;
    }
	/* stop the first timer */
	stoptimer(d->simulator_value);
	d->timer_is_running=0;
	/* if there is more timer, run it right now */

    if(d->head_timer != d->tail_timer)
    {

        float add=d->timer_timeout[d->head_timer] - time;

        d->head_timer=PLUS(d->head_timer);
        starttimer(d->simulator_value,add);
        d->timer_is_running=1;
    }
}

//queue class equivalent imolementation


int queue_head[2] = {0, 0};
int queue_tail[2] = {0, 0};
struct msg queue_buffer[2][MAX_BUF];

/* check if queue is empty */
#define empty(AorB) (queue_head[AorB] == queue_tail[AorB])

/* put message in queue */
void push(int AorB, struct msg message)
{
	/* bound check */
	if (queue_head[AorB] == queue_tail[AorB] + 1) {
		printf("Warning: there is no avaliable space in queue.\n");
		return;
	}
	queue_buffer[AorB][queue_tail[AorB]] = message;
	queue_tail[AorB] = PLUS(queue_tail[AorB]);
}

/* get messsage out of queue */
struct msg pop(int AorB)
{
	/* bound check */
	if (empty(AorB)) {
		printf("Warning: no packet in queue.\n");
        struct msg dummy;
		return dummy;
	}
	struct msg message = queue_buffer[AorB][queue_head[AorB]];
	queue_head[AorB] = PLUS(queue_head[AorB]);
	return message;
}

struct pkt encapsulate(device *d, struct msg message)
{
    struct pkt packet;
	memcpy(packet.payload, message.data, sizeof(message.data));
	packet.seqnum = d->next_seq;
	packet.acknum = -1;
	packet.checksum = 0;
	packet.checksum = findChecksum(&packet);

    d->window_data[d->next_seq]=packet;
    d->next_seq=PLUS(d->next_seq);
    d->present_packet++;

	return packet;
}

void Output(device *d, struct msg message)
{
    if ( d->present_packet < WIN_LEN) {
		/* construct a packet */
		struct pkt packet = encapsulate(d, message);
        pf("seq %d sending by %d\n", packet.seqnum, d->simulator_value);
		tolayer3(d->simulator_value, packet);
		start_multitimer(d, packet.seqnum);
		/* debug output */
		//if (DEBUG)
			//print_packet(d->simulator_value, "Send", packet);
	} else {
		push(d->simulator_value, message);
	}
}

void Input(device *d, struct pkt packet)
{

    if(findChecksum(&packet) != packet.checksum)
    {
        //corrupted
        pf("corrupted packet found by %d\n", d->simulator_value);
    }
	else if (packet.acknum==-1 && d->expecting_rcv == packet.seqnum) //this is data
    {
		/* pass data to layer5 and send ack*/
		struct msg message;
		memcpy(message.data, packet.payload, sizeof(packet.payload));
		tolayer5(d->simulator_value, message.data);
        d->expecting_rcv=PLUS(d->expecting_rcv);

        {
            //start_multitimer(d,-1);
        }
        struct pkt pk;
        pk.acknum=packet.seqnum;
        pk.seqnum=-1;
        pk.checksum=findChecksum(&pk);
        pf("ack: %d sending from %d\n", pk.acknum, d->simulator_value);
        tolayer3(d->simulator_value,pk);



	}
    else if (packet.acknum==-1 ) //this is old data
    {
        struct pkt pk;
        pk.acknum=packet.seqnum;
        pk.seqnum=-1;
        pk.checksum=findChecksum(&pk);
        pf("old seq %d found. ack: %d sending from %d\n", packet.seqnum, pk.acknum, d->simulator_value);
        tolayer3(d->simulator_value,pk);

        //start_multitimer(d,-1);

	}
    else if (packet.seqnum==-1 /*&& d->expecting_ack == packet.acknum*/) //this is ack/nak
    {
        if( between(d->expecting_ack, packet.acknum, d->next_seq) ) //really ack
        {
            pf("ack: %d received by %d\n", packet.acknum, d->simulator_value);
            d->retransmit_done=0;
            while ( between(d->expecting_ack , packet.acknum, d->next_seq) )
            {

                d->expecting_ack = PLUS(d->expecting_ack);
    			d->present_packet--;
    			stop_multitimer(d, d->expecting_ack);
    		}
            /* add new packet from queue */
            while ( d->present_packet < WIN_LEN && !empty(d->simulator_value)) {
    			struct msg message = pop(d->simulator_value);
    			struct pkt packet = encapsulate(d, message);
                pf("seq: %d sending from %d\n", packet.seqnum, d->simulator_value);
    			tolayer3(d->simulator_value, packet);

    			start_multitimer(d, packet.seqnum);
    		}
        }
        else if(packet.acknum==MINUS(d->expecting_ack)) //nak
        {
            //retransmission //bonus 3
            pf("nak (old ack): %d received by %d\n", packet.acknum, d->simulator_value);
            int seqnum;
            if(d->retransmit_done)
            {

            	for (seqnum = d->expecting_ack; seqnum != d->next_seq; seqnum = PLUS(seqnum))
                {
                    if( !between(d->retransmit_head, seqnum, d->retransmit_tail) )
                    {
                        stop_multitimer(d, seqnum);
                        //pf("seq: %d sending from %d\n", seqnum, d->simulator_value);
                		tolayer3(d->simulator_value, d->window_data[seqnum]);
                		start_multitimer(d, seqnum);
                    }
            	}
                pf("retransmit done upto seq %d\n", seqnum);
                d->retransmit_tail=seqnum;

            }
            else
            {
                for (seqnum = d->expecting_ack; seqnum != d->next_seq; seqnum = PLUS(seqnum))
                {
                    //if( !between(d->retransmit_head, seqnum, d->retransmit_tail) )
                    {
                        stop_multitimer(d, seqnum);
                		tolayer3(d->simulator_value, d->window_data[seqnum]);
                		start_multitimer(d, seqnum);
                    }
            	}
                d->retransmit_done=1;
                d->retransmit_head=d->expecting_ack;
                d->retransmit_tail=seqnum;
                pf("retransmit done for seq %d to seq %d\n", d->expecting_ack,seqnum);
            }
        }
        else
        {
            //garbage ack
            //pf("too old ack found\n");
        }
    }
}

void deviceInit(device *d, int a)
{
    d->present_packet=0;
    d->next_seq=0;
    d->expecting_rcv=0;
    d->expecting_ack=0;
    d->retransmit_done=0;
    d->simulator_value=a;
}


/* called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{
    pf("-------------------A_output--------------------\n");

    Output(&AA,message);
    return;
}

void B_output(struct msg message)
{
    pf("-------------------B_output--------------------\n");

    Output(&BB,message);
}

void A_input(struct pkt packet)
{
    pf("-------------------A_input--------------------\n");

    Input(&AA,packet);

    return;
}

void timerinterrupt(device *d)
{
    d->timer_is_running=0;
    if(d->ack_timeout==time)
    {
        struct pkt pk;
        pk.acknum=MINUS(d->expecting_rcv);
        pk.seqnum=-1;
        pk.checksum=findChecksum(&pk);
        tolayer3(d->simulator_value,pk);
        return;
    }
	int seqnum;
	for (seqnum = d->expecting_ack; seqnum != d->next_seq; seqnum = PLUS(seqnum))
    {

        if( seqnum != d->expecting_ack)
			stop_multitimer(d, seqnum);

		tolayer3(d->simulator_value, d->window_data[seqnum]);
		start_multitimer(d, seqnum);
/*		if (DEBUG)
			print_packet(AorB, "Timeout retransmit", window_buffer[AorB][seqnum]);*/
	}
    pf("retransmit done for seq %d to seq %d\n", d->expecting_ack,seqnum);

}

/* called when A's timer goes off */
void A_timerinterrupt(void)
{
    pf("----------------A_timerinterrupt-----------------\n");
    timerinterrupt(&AA);
}

void A_init(void)
{
    pf("--------------------A_init---------------------\n");
    deviceInit(&AA,0);
}


void B_input(struct pkt packet)
{
    pf("-------------------B_input--------------------\n");

    Input(&BB,packet);

    return;
}


void B_timerinterrupt(void)
{
    pf("----------------B_timerinterrupt-----------------\n");
    timerinterrupt(&BB);
}


void B_init(void)
{
    pf("--------------------B_init--------------------\n");
    deviceInit(&BB,1);
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

    pf("-----  Stop and Wait Network Simulator Version 1.1 -------- \n\n");
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
void stoptimer(int AorB /* A or B is trying to stop timer */) //linked list
{
    struct event *q, *qold;

    if (TRACE > 2)
        pf("          STOP TIMER: stopping timer at %f\n", time);
    /* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
    for (q = evlist; q != NULL; q = q->next)
        if ((q->evtype == TIMER_INTERRUPT && q->eventity == AorB))
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

void starttimer(int AorB /* A or B is trying to start timer */, float increment)
{
    struct event *q;
    struct event *evptr;

    if (TRACE > 2)
        pf("          START TIMER: starting timer at %f\n", time);
    /* be nice: check to see if timer is already started, if so, then  warn */
    /* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
    for (q = evlist; q != NULL; q = q->next)
        if ((q->evtype == TIMER_INTERRUPT && q->eventity == AorB))
        {
            pf("Warning: attempt to start a timer that is already started\n");
            return;
        }

    /* create future event for when timer goes off */
    evptr = (struct event *)malloc(sizeof(struct event));
    evptr->evtime = time + increment;
    evptr->evtype = TIMER_INTERRUPT;
    evptr->eventity = AorB;
    insertevent(evptr);
}

/************************** TOLAYER3 ***************/
void tolayer3(int AorB, struct pkt packet)
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
    evptr->eventity = (AorB ^ 1) ; /* event occurs at other entity */
    evptr->pktptr = mypktptr;         /* save ptr to my copy of packet */
    /* finally, compute the arrival time of packet at the other end->
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

void tolayer5(int AorB, char datasent[20])
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
