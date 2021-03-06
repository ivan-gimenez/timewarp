/*      Copyright (C) 1989, 1991, California Institute of Technology.
		U. S. Government Sponsorship under NASA Contract NAS7-918
		is acknowledged.        */

/*
 * $Log:	twsys.h,v $
 * Revision 1.19  92/02/25  10:36:50  reiher
 * added address table size to State definition
 * 
 * Revision 1.18  91/12/27  10:00:53  reiher
 * Added stuff to the OCB to support throttling and input queue splitting
 * 
 * Revision 1.17  91/12/27  09:28:14  pls
 * 1.  Add stats for variable address table length (SCR 214).
 * 2.  Add l_size macro.
 * 
 * Revision 1.16  91/11/04  15:23:23  reiher
 * Added code related to critical path computation.  Made the message flag
 * field into an int, to allow more values. (PLR)
 * 
 * Revision 1.15  91/11/04  15:11:01  configtw
 * Fix up #include statement.
 * 
 * Revision 1.14  91/11/01  13:50:45  pls
 * 1.  Add support for statesMovedQ.
 * 2.  Add support for shared cubeio.c (SCR 195).
 * 3.  Support bit usage for sflag.
 * 4.  Add support for message cancellation (SCR 172).
 * 
 * Revision 1.13  91/07/17  15:14:02  judy
 * New copyright notice.
 * 
 * Revision 1.12  91/07/09  15:38:17  steve
 * 1. Replaced char mistuff[8] with LowLevelMsgH.
 * 2. Lists low level message types.
 * 3. Defines IH_NODE.
 * 4. Add type field to Mem_hdr for memdraw tool.
 * 5. Moved tw_node_num and tw_num_nodes from tester.h
 * 
 * Revision 1.11  91/06/03  12:27:26  configtw
 * Tab conversion.
 * 
 * Revision 1.10  91/05/31  15:45:05  pls
 * 1.  Add flags for pending list time calc, migration, bad block
 *     size error, and state error message saving.
 * 
 * Revision 1.9  91/04/01  15:31:50  reiher
 * Several changes for dynamic load management.  They include defining a
 * stats data structure, a new dead ocb data structure, macros for handling
 * that data structure, and conditionally compiled code to support Som's
 * work
 * 
 * Revision 1.8  91/03/28  09:54:02  configtw
 * Add RBC change for Steve.
 * 
 * Revision 1.7  91/03/26  09:57:36  pls
 * 1.  Add Steve's RBC code.
 * 2.  Add support for TYPEINIT and library hook.
 * 3.  Remove gvtrow and gvtcol references.
 *
 * Revision 1.6  90/10/15  12:49:53  reiher
 * moved FILE_NAME_LENGTH constant to twcommon.h
 * 
 * Revision 1.5  90/10/15  12:45:32  reiher
 * changed FILE_NAME_LENGTH to 41
 * 
 * Revision 1.4  90/10/15  12:36:13  reiher
 * changed TW_FILE name length to fix problem for Fred with long name
 * 
 * Revision 1.3  90/08/27  10:46:51  configtw
 * Split cycle time from committed time.
 * Pad List_hdr to 8 byte boundary.
 * 
 * Revision 1.2  90/08/09  14:24:27  steve
 * Added last_send and out_of_sq fields to Ocb for phase migration
 * 
 * Revision 1.1  90/08/07  15:41:40  configtw
 * Initial revision
 * 
*/

#if MONITOR
#define Debug           mad_monitor();
#else
#define Debug
#endif
#define FUNCTION        /* to make it easy to grep function names */
#define CALL            /* to make it easy to grep function calls */

/*--------------------------------------------------------------------------*
   TIME WARP CONSTANTS AND MACROS
*--------------------------------------------------------------------------*/

#define MAXNTYP          32     /* Maximum number of object types */

#define BCAST           -1      /* Special value for Node                  */
#define GVTNODE  0              /* master node for GVT calculation         */

#define ON_NODE(w)  ((w)->node==tw_node_num)
#define OBJ_NFOUND(w) (!ON_NODE(w) && !OFF_NODE(w))

#define BITEST(FLAG,MASK) ((FLAG)&(MASK)) /* THIS WAY, I DON'T HAVE TO */
#define BITTST(FLAG,MASK) ((FLAG)&(MASK)) /* REMEMBER SPELLING */
#define BITTEST(FLAG,MASK) ((FLAG)&(MASK))
#define BITSET(FLAG,MASK) ((FLAG) |= (MASK))
#define BITCLR(FLAG,MASK) ((FLAG) &= (~(MASK)))

/* in our code, we assume all pointers to data are the same size, so that  */
/* sizeof(Dbl *) == sizeof(Byte *) == sizeof(struct whatever *) == etc.    */

#define NULLMSGH        (Msgh *)0       /* NULL ptrs for lint happiness    */
#define NULLOCB         (Ocb  *)0
#define NULLDOCB         (deadOcb  *)0
#define NULLSTATE       (State *)0

/* The following defines are used by a general hash function to determine
		the range of values to hash a name to.                              */

#define HOME_NODE 2
#define CACHE 3
#define HOMELIST 4

/* NOTINCACHE is the value returned by GetLocation() when it cannot find
		the object in the cache, and has sent a message to the home node.
		Generally, getting it back means that the calling routine must
		wait before proceding. */

#define NOTINCACHE -1

/* The following are used by MinPendingList to decide whether a
		pending action is a message or not.  If it is a message,
		the message's send time must be accounted for by the gvt
		calculation. */

#define MSG 1
#define NOTMSG 0

/*--------------------------------------------------------------------------*
   THE LOCAL OBJECT TYPE TABLE RECORD TYPE
*--------------------------------------------------------------------------*/
#ifndef SIMULATOR
typedef struct /* Typtbl  */{   /*                                         */
	char    * type ;            /* Object type string                      */
	int     (*init)() ;         /* Init entry point                        */
	int     (*event)() ;        /* Event entry point                       */
	int     (*term)() ;         /* Term entry point                        */
	int     (*displayMsg)() ;   /* Display Msg entry point                 */
	int     (*displayState)() ; /* Display State entry point               */
	int     statesize;          /* Size of user-defined state              */
	void*   (*initType)();      /* Type init routine                       */
	void    * typeArea;         /* Type init area                          */
	void    * libTable;         /* Pointer to library stuff                */
	}                   Typtbl ;
#endif /* SIMULATOR */

#define NULL_TYPE &type_table[1]

/*--------*
	Low level Message Header
*--------*/
/* low level message types */
#define NORMAL_MSG				-1

#define READ_THE_CONSOLE        0
#define EXIT                    1
#define SIGNAL_THE_CUBE         2
#define STDOUT_DATA             3
#define STDOUT_TIME             4
#define STATS_DATA              5
#define GVT_DATA                6
#define FLOW_DATA               7
#define MSG_DATA                8
#define TIME_SYNC               9
#define ISLOG_DATA              10
#define QLOG_DATA               11
#define ACK_MSG					12
#define OTHER_MSG				13
#define TESTER_COMMAND			14

/* low level destination */
#define IH_NODE	128

typedef struct
{
	short from_node;
	short to_node;
	short type;		/* only NORMAL_MSG go through timewarp	*/
	short length;	/* text length (including header)  */
}
	LowLevelMsgH;


/*--------------------------------------------------------------------------*
   MESSAGE HEADER (MSG TEXT FOLLOWS HEADER IMMEDIATELY BY CONVENTION)
*--------------------------------------------------------------------------*/
								/*                                         */
#define SIGN            0x01    /* ON for antimsgs                         */
#define DIRECTION       0x02    /* ON for flow ctl msgs                    */
#define USED            0x04    /* ON for messages in the output queue     */
								/*   which did not have to be resent       */
								/*   because lazy cancellation worked      */
								/* also ON when antimessage is first       */
								/* stored in output queue                  */
#define SYSERR          0x08    /* ON if an error is detected (error msgs  */
								/*   and states also have an independent   */
								/*   type field                            */
#define MOVED			0x10	/* ON for msgs that migrated (for debug)   */
#define LOCKED          0x20    /* ON when a msg buffer is not available   */
#define MOVING          0x40    /* ON for moving messages                  */
#define SYSMSG          0x80    /* ON if this is a TW system msg (system   */
								/*   msgs also have a type field like user */
								/*   msgs -- this bit is ON for all types  */
								/*   of system msgs.                       */
#define FOSSILMSG		0x100	/* Fossil message						   */
#define NONCRITMSG		0x200	/* Msg not on critical path				   */
#define NORMAL          0x00    /* For turning flags off                   */
								/*                                         */
#define NEGATIVE      SIGN      /*  for convenience...                     */
#define BACKWARD      DIRECTION /*   ''    ''                              */
								/*                                         */

/* These are the type fields -- WARNING -- Some routines assume that these */

/* These are the type fields -- WARNING -- Some routines assume that these */
/* types are defined in INCREASING ORDER: TMSG < EMSG */

#define MINTYPE         0       /* This type must be less than the minimum 
									    real type!                         */
#define CMSG            2       /* Create (SYSMSG)                         */
#define DYNCRMSG        3       /* Dynamic create message (OBJMSG)         */
#define TMSG            5       /* Term (SYSMSG)                           */
#define EMSG            6       /* Event  (OBJMSG)                         */
#define DYNDSMSG        7       /* Dynamic destroy message (OBJMSG)        */
#define CREATESYS       8       /* Create system message (SYSMSG)          */
#define GVTSYS          9       /* Gvt system msg (SYSMSG)                 */
#define COMMAND        10       /* Tester command message                  */
#define CRT_ACK        11       /* Outgoind crtctx done msg (SYSMSG)       */
#define SIM_END_MSG    12       /* Simulation Ended                        */
#define MIGR_LOG       13       /* Migration Log                           */
#ifndef SIMULATOR
#define XL_STATS       14       /* EXCEL formatted statistics              */
#else
extern pf_file;
#define XL_STATS       pf_file
#endif
#define MONINIT        15       /* Monitor Initialization message          */
#define TW_ERROR       16       /* Time Warp Error messages.               */
#define STDOUT_EMSG	   17

#define STATEMSG       18
#define STATEACK       19
#define STATENAK       20
#define STATEDONE      21
#define MOVEPHASE      22
#define PHASEACK       23
#define PHASENAK       24
#define PHASEDONE      25
#define MOVEVTIME      26
#define VTIMEACK       27
#define VTIMENAK       28
#define VTIMEDONE      29

#define HOMENOTIF      32       /* Notify object's home node of location 
									                             (SYSMSG)  */
#define HOMEASK        33       /* Ask home node for object's location     
									                             (SYSMSG)  */
#define HOMEANS        34       /* Response to HOMEASK (SYSMSG)            */
#define HOMECHANGE     35       /* Change an object's HL entry (SYSMSG)    */
#define CACHEINVAL     36       /* Request removal of an entry from a cache */
#define LOADSYS        37       /* Load gathering message (SYSMSG)         */
#define PCREATESYS     38     /* Static phase create message (SYSMSG)    */
#define ADDSTATS       39     /* Add an earlier phase's stats to a later
									  phase's (SYSMSG)                   */
#define CRITMSG	   	   40		/* Contains critical path computations
										information (SYSMSG) */
#define CRITSTEP	   41		/* Instructs node to find next step in 
										critical path (SYSMSG) */
#define CRITEND		   42		/* Tells node to output components of 
										critical path (SYSMSG) */
#define CRIT_LOG	   43		/* Sends data to the crit path log (SYSMSG) */
#define CRITRM		   44		/* Carries info for crit path pruning 
									(SYSMSG) 								*/


typedef struct /* Msgh */ {     /*                                         */
	LowLevelMsgH	low;		/* The low level message header is here	   */
	int     msgtimef;           /* Timestamp for Msg Log                   */
	int     msgtimet;           /* Timestamp for Msg Log                   */
	int     cputime;            /* Timestamp for Flow Analyzer             */
	int     checksum;           /* Check Sum                               */
	Name    snder ;             /* Object name of the sender               */
	VTime   sndtim ;            /* Virtual time at which msg was sent      */
	Name    rcver ;             /* Object name of the receiver             */
	VTime   rcvtim ;            /* Virtual time to receive msg             */
	long    selector;           /* Message Selector                        */
	Uid     gid ;               /* Unique message group id                 */
	int    flags ;              /* Individual flags accessible by masks:   */
	Byte    mtype ;             /* Message types                           */
	Byte    stype ;             /* Type of the state creating this msg.    */
	Byte    segno;              /* State Message Segment                   */
	Byte    spareFlags;         /* Unused byte for alignment               */
	int     txtlen ;            /* Length of msg text                      */
	int     seglen;             /* for object migration                    */
	int     pktno;              /* for object migration                    */
	int     no_pkts;            /* for object migration                    */
#if SOM
	long    Ept;
#endif SOM
	}                   Msgh ;  /*                                         */
								/*                                         */
/*--------------------------------------------------------------------------*
   TIME WARP OBJECT STATE
*--------------------------------------------------------------------------*/
#ifndef SIMULATOR
typedef struct
{
	int open_flag;
	int file_num;
	int char_pos;
	int sequence;
}
	TW_STREAM;
#endif /* SIMULATOR */

typedef Byte * Address;

#define DEFERRED ((Byte *)-1)

typedef struct /* State */{     /*                                         */
	VTime   sndtim ;            /* Virtual time at which state was sent    */
	int     cputime;            /* Timestamp for Flow Analyzer             */
	int     effectWork;         /* Effective work measure for Dyn Load Man */

#if SOM
	long    previousEpt;
	long    Ept;
#endif SOM
								/*                                         */
#define NOERR           0
#define OBJDIV0         "divide by zero"
#define OBJOFLOW        "floating or integer overflow"
#define OBJSUB          "subscript out of range ?"
#define OBJADR          "address fault"
#define OBJSTK          "stack overflow"
#define OBJINS          "illegal instruction"
#define OBJTOUT         "timed out"
#define UNKNOWNRCVER    "Sent message to unknown rcver"
#define MSGRANGE        "message index out of range"
#define MSGTIME         "message times illegal"
#define MSGLEN          "message length > pktlen"
#define MSGRCVER        "message receiver null"
#define TWMFU           "twmfu"
#define ADDROFLOW       "address table overflow"
#define STATE_DISPOSAL  "invalid offset for disposeBlockPtr"
#define TYPCHG          "Illegal type change"
#define NULLOBJST       "State belonging to a null object"
#define NULLEVT         "Event msg for a null object"
#define MSGVOFLOW       "Message Vector Overflow"
#define NEWBLKSZ        "Invalid size for newBlockPtr"

								/*                                         */
	char * serror ;             /* state error code                        */
	int   sflag;

/* The following are values for sflag. */

#define STATERR 1               /* The state is in error; must free serror */
#define FOSSILSTATE 2           /* This state is eligible for fossil
                                    collection                             */
#define CRITSTATE 4             /* This is a critical path state           */
#define STATECPCLR 8            /* This state's successor state has determined
                                    that this state does not make a CP
                                    contribution through it. */
#define STATETRUNC 16
#define STATEFORW 32


	Typtbl  *otype;             /* object type                             */
	int   stype;                /* state type                              */

	/* The following are valid state types. */

#define EVENTSTATE     6
#define CREATESTATE     2
#define DYNCREATESTATE  3
#define DESTROYSTATE    7

	int resultingEvents;	/* This field keeps track of how many events
								resulted from this event - one for the next
								state, one each for every output message.
								The latter is oversimplified, since two output
								messages could cause the same event, but the
								code dealing with this field handles that
								possibility.  This field is used only for
								critical path computation. */

#ifndef SIMULATOR
	TW_STREAM stream[MAX_TW_STREAMS];
#endif /* SIMULATOR */

	int stdout_sequence;

	int segCount;		/* number of segments in use in address table	*/
	int	addrTableSize;	/* Only valid while state is migrating.  We don't
							bother to keep track, at other times. */
	Address * address_table;

	Byte * ocb;
	Byte segno;         /* PJH These should be moved to */
	Byte no_segs;       /* the State_Migr_Hdr, but it   */
	Byte pktno;         /* means a total re-structure   */
	Byte no_pkts;       /* of state_recv(). Maybe later */
	Byte tot_pkts;

	}                   State ; /*                                         */

typedef struct  /* Truncated State       */
{
	VTime   sndtim ;            /* Virtual time at which state was sent    */
	long    Ept;				/* Earliest time event could end		   */
	int   sflag;                /* flag: TRUE => must free serror          */
	int   stype;                /* state type                              */
	int resultingEvents;		/* how many events caused by this event	   */
	Byte * ocb;					/* Pointer to owning ocb				   */
	long	causingSelector;	/* msg selector that caused this event	   */
	}                   truncState ; /*                                    */

typedef struct  /* State migration header       */

{

	Name	name;
	VTime   time_to_deliver;
	State  * state;
    int acksExpected;
    int acksReceived;
	Byte to_node;
	Byte waiting_for_ack;
	Byte waiting_for_done;
	Byte    migr_flags;         
#define PRE_INTERVAL    1
#define MIGRATING       2
#define WAIT_FOR_MIGRATE 4
#define STATE_MOVING 8

}       State_Migr_Hdr;



typedef struct
{
	VTime vtime;
	int cnt;
}
	Evtlog;

/* Time Warp Statistics data structure. */

struct stats_s {                /*                                         */
								/*                                         */
	long     numestart; /* Number of events which started     */
	long     numecomp;  /* Number of events which ended       */

	long         numcreate;     /* Number of times object created          */
	long         numdestroy;    /* Number of times object destroyed        */
	long         ccrmsgs;       /* Number of committed creates             */
	long         cdsmsgs;       /* Number of committed destroys            */

	long     cemsgs;    /* Committed event messages                */
	long     cebdls;    /* Committed event bundles                 */
								/* BUNDLE = group of messages at same time */
								/*                                         */
	long     coemsgs;   /* Committed OUTPUT event messages        */
	long     coebdls;   /* Committed OUTPUT event bundles         */
								/* BUNDLE = group of messages at same time */
								/*                                         */
	long     nssave;    /* number of states saved                  */
	long     nscom;             /* number of states committed              */

	long     eposfs;    /* number of Events, POSitive Forward Sent */
	long     eposfr;    /* number of Events, POSitive Forward Rec'd*/

	long     enegfs;    /* number of Events, NEGative Forward Sent */
	long     enegfr;    /* number of Events, NEGative Forward Rec'd */

	long     ezaps;             /* number of event annihilations           */

	long     evtmsg;    /* number of times evtmsg called           */

	long        eposrs;         /* number of Events, Positive Reverse Sent */
	long        eposrr;         /* number of Events, Positive Reverse Rec'd*/

	long        enegrs;         /* number of Events, Negative Reverse Sent */
	long        enegrr;         /* number of Events, Negative Reverse Rec'd*/

	long        cputime;        /* accumulated cpu time                    */

	long        rbtime;         /* amount of work rolled back              */
	long        comtime;        /* committed time                          */
	float       utilization;    /* last effective utilization calculated   */
	long        nummigr;        /* Number of times object migrated         */
	long        numstmigr;      /* Number of states migrated               */
	long        numimmigr;      /* Number of input messages migrated       */
	long        numommigr;      /* Number of output messages migrated      */
	long        sqlen;          /* Average length of state queue           */
	long        iqlen;          /* Average length of input queue           */
	long        oqlen;          /* Average length of output queue          */
	long        sqmax;          /* Max length of state queue at GVT        */
	long        iqmax;          /* Max length of input queue at GVT        */
	long        oqmax;          /* Max length of output queue at GVT       */
	long        stforw;         /* # of states forwarded                   */

	long		segMax;			/* max # segments in address table		   */

	};          

#if RBC
typedef struct
{
		int seg_no;
		int status;
#define SEG_FREE                0
#define SEG_ALLOC               0x01
#define SEG_FUTURE_FREE         0x02
#define SEG_PAST_FREE           0x04
		VTime first_alloc;
		VTime last_used;
		unsigned int rbc_start_KB;
		unsigned int rbc_end_KB;
}
SegmentCB;

#define SEG_PER_CB 10

typedef struct footCB 
{
		int first_used_segment;
		struct footCB * next;
		SegmentCB seg[SEG_PER_CB];
}
FootCB;

#endif

/*--------------------------------------------------------------------------*
   OBJECT CONTROL BLOCK (ELEMENT OF THE SCHED QUEUE)

	New fields added to the Ocb must have corresponding code in migr.c
	to migrate them--this does not happen automatically!

*--------------------------------------------------------------------------*/
typedef struct /* Ocb  */{      /*                                         */
	Name    name ;              /* Fixed-length object name                */
	int     oid;                /* unique object id                        */
	VTime   phase_begin;
	VTime   phase_end;
	VTime   phase_limit;
	VTime   next_limit;
/* The New Jump Forward Optimization for phase changes */
	State   *last_sent;
	int     out_of_sq;
	int     next_node;
	int		prev_node;
	int     num_states;
	int     num_imsgs;
	int     num_omsgs;
	State   *rstate;
	VTime   svt ;               /* Scheduler Virtual Time                  */
	Typtbl  *typepointer ;      /* Points to constant object type (code)   */
	Pointer libPointer;         /* Used by the library routines            */
	State   *sb ;               /* Temporary state buffer for running      */
    truncState *tsqh;           /* Truncated state queue head              */
	Byte    *stk;               /* Temporary stack                         */
	State   *cs ;               /* The current state in the queue          */
	State   *sqh ;              /* State queue head (or master element)    */
	Msgh    *ci ;               /* Current Input Q msg bundle              */
	Msgh    *iqh ;              /* Head of the Input Queue                 */
	Msgh    *co ;               /* Current Output Q msg bundle             */
	Msgh    *oqh ;              /* Head of the Output Queue                */
	int     ecount ;            /* Count of msgs in bundle                 */
	Msgh   **msgv;              /* Message Vector                          */
	Int     centry ;            /* What Object entry point am I in?        */
	Int     crcount;            /* Counter of committed creates/destroys.  */
/* The centry stuff is probably redundant, and can be deleted at some point. */
#define INIT CMSG
#define EVENT EMSG
#define TERM TMSG
#define DCRT DYNCRMSG           /*                                         */
#define DDES DYNDSMSG

	Byte    runstat ;           /* Run status:                             */
#define READY         0x00      /* All flags cleared                       */
#define BLKINF        0x01      /* Blocked at time = +infinity (finished)  */
#define BLKPKT        0x02      /* Blocked for retry of msg sending        */
#define GOFWD         0x08      /* Ready to Go Forward                     */
#define ARCP          0x20      /* Activate on Rect of Completion Packet   */
#define ARLBK         0x40      /* Activate on Rollback                    */
#define ITS_STDOUT    0x80      /* Object is of type "stdout", never runs  */

#define BLKSTATE      0x48      /* Blocked waiting for a state             */
#define BLKPHASE      0x49      /* Blocked waiting for a phase to move     */
#define STATESEND     0x50      /* Must retry to send last state to nxt phase */
								/*                                         */
	Byte    control ;           /*                                         */
								/*                                         */
#define     NONEDGE       0     /* Object interrupted                      */   
#define     EDGE          0x01  /* Object just finished all work at time t */

	Byte migrWait;
#define  WAITFORACK   0x01
#define  WAITFORDONE  0x02
	Byte migrStatus;
#define  MIGRNOTSTARTED 0x00
#define        MIGRSTART      0x01
#define  MIGRDONE     0x02
#define  RECVVTIME    0x04
#define  RECVSTATE    0x08
#define  SENDVTIME    0x10
#define  SENDNEXTVTIME  0x12
#define  SENDSTATE      0x14
#define  OCBSETUP     0x18
#define  SENDINGVTIME 0x20
#define  SPLITFORIQLEN 0x40

	int     generation;         /* Count of # of times phase moved.        */
	int     loststate;          /* Was state was shipped to later phase?   */

#if RBC
	int     uses_rbc;           /* flag to indicate if state on rbc        */
	int     first_seg;          /* seg number of required state            */
	int     frames_used;        /* debugging and to speed alloc's          */
	Byte  * footer;             /* state footer                            */
	FootCB * fcb;               /* footer control block                    */
	Byte  * last_chance;        /* segment's second chance to be re-used   */
	int     keep_all_segs;      /* don't release any dynamic segments      */
#endif

	union blk_restore_u {       /* Contains information to start up        */
		/*  argblock */         /*  previously blocked objects             */
		struct pkt_blk_str {    /*                                         */
			Byte    mtype ;     /*                                         */
			Name  * rcver ;     /*                                         */
			VTime   rcvtim ;    /*                                         */
			Int     len ;       /*                                         */
			Byte  * text ;      /*                                         */
			Uid     gid ;       /*                                         */
			long    selector;   /*                                         */
			} pkt_blk ;         /*                                         */
								/*                                         */
		Pointer  address_table_offset; /* returned by newBlockPtr          */
								/*                                         */
		}   argblock ;          /* This is the union's name                */
								/*                                         */
								/*                                         */
	int     pvz_len ;           /* The length of the perm variable zone    */
								/*                                         */
								/*                                         */
	struct stats_s stats;

	Evtlog * evtlog;
	int ce;

	long        cycletime;      /* effective utilization                   */
	int cancellations;
	int eventsPermitted;		/* Number of events permitted by throttling */
	int eventTimePermitted;		/* Amount of event time permitted by throttling */

#if SOM
	long    Ept;
	long    comEpt;
	long    lastComEpt;
	long    work;
	long    comWork;
	long    lastComWork;
#endif SOM


	}                   Ocb ;   /*                                         */

typedef struct deadOcb_str {
	Name      name;
	VTime     phaseBegin;
	VTime     phaseEnd;
}     deadOcb;

/* Defines for different types of throttling. */

#define NOTHROT 0
#define OMSGTHROT 1
#define ETIMETHROT 2
#define DYNWINTHROT 3
#define EVTHROT 4
#define RBTHROT 5
#define STATICWINDOW 6

/* The following define should be set to the highest legal value for any
	throttling strategy, as defined above. */

#define MAXTHROTSTRAT 6

/*--------------------------------------------------------------------------*
   OBJECT LOCATION RECORD
*--------------------------------------------------------------------------*/
typedef struct Objloc_str {
	Name    name;               /* Object name                             */
	VTime   phase_begin;        /* Start of virtual time interval          */
	VTime   phase_end;          /* End of virtual time interval            */
	Int     node;               /* Assigned node of the object             */
	Ocb     *po;                /* Points to Ocb if object is on this node */
	}                   Objloc;

/*--------------------------------------------------------------------------*
   CACHE ENTRY
*--------------------------------------------------------------------------*/
typedef struct Cache_str {
	Objloc    cache_entry;      /* Cached data                             */
	long      replace;          /* Replacement data                        */
	}                   Cache_entry;

/*--------------------------------------------------------------------------*
   HOME SITE LIST
*--------------------------------------------------------------------------*/
struct HomeList_str {
	Name        name;           /* Object name                             */
	VTime       phase_begin;    /* Start of virtual time interval          */
	VTime       phase_end;      /* End of virtual time interval            */
	Int         node;           /* Object location                         */
	int         generation;     /* How often the entry has been changed.   */
	struct HomeList_str *forward;       /* Forward pointer                 */
	struct HomeList_str *back;          /* Backward pointer                */
	}                   ;

/*--------------------------------------------------------------------------*
   ACTION PENDING LIST
*--------------------------------------------------------------------------*/
typedef struct Pending_str {
	Name        object;         /* Name of pending object                  */
	int         type;           /* Type of pending request                 */
	VTime       time;           /* Virtual time within pending object      */
	Int         (*restart) ();  /* Function to call when request arrives   */   
	Msgh        *Message;       /* Message to pass to function             */
	int         isMsg           /* True if this is a message               */
	}                   Pending_entry;

/*--------------------------------------------------------------------------*
   TEXT OF A CREATE MESSAGE - If you change the following struct at all, then
   change it in entry.c as well.
*--------------------------------------------------------------------------*/
typedef struct {                /*                                         */
	Type    tp ;                /* Type of the object                      */
	Int     node ;              /* Processor number                        */
	VTime   phase_begin;        /* Start of period                         */
	VTime   phase_end;          /* End of period                           */
	}                   Crttext;

/*--------------------------------------------------------------------------*
   TEXT OF A STATS MESSAGE - If you change the following struct at all, then
   change it in entry.c as well.
*--------------------------------------------------------------------------*/
typedef struct {
	VTime     phaseEnd;
	struct stats_s stats;
	}                 Stattext;

/*--------------------------------------------------------------------------*
   A MEMORY BLOCK HEADER -- USED BY MEM.C AND MAPPER.C
*--------------------------------------------------------------------------*/

typedef char Align[16];

#define PAD ((1+((sizeof(struct s_struct)-1)/sizeof(Align)))*sizeof(Align))

typedef union H_union {

	struct s_struct {
		union H_union * next ;  /* next free block */
		union H_union * prev ;  /* prev free block */
		long  size ;            /* size of this free block (Mem_hdrs) */
		long  type; /* for mark mem */
		} s;

	Byte p[ PAD ] ;             /* make sizeof(Mem_hdr) == n*sizeof(Align) */

	} 
		Mem_hdr;

#undef PAD


/*--------------------------------------------------------------------------*
   A LIST ELEMENT HEADER -- USED BY LIST.C

		This struct must be a multiple of 8 bytes, because it is added
		to objstksize (see l_create() in list.c) when the object's stack
		is created by loadstatebuffer() in state.c.  On the Sun4, a stack
		pointer must be on 8 byte boundaries.

*--------------------------------------------------------------------------*/



typedef struct H_struct {       /* list element header             */

	struct H_struct *prev ;     /* pointer to previous member      */
	struct H_struct *next ;     /* pointer to following member     */
	int  size ;                 /* size of data following List_hdr */
	int  pad;                   /* pad for 8 byte alignment        */

	} 
		List_hdr;


/*--------------------------------------------------------------------------*
   GVT INFORMATION -- USED BY GVT.C AND TESTER
*--------------------------------------------------------------------------*/



typedef Int Gvttype ;  /* type of the GVT message, which can be one of: */


#define GVTINIT         71      /* Initialize GVT routines      */
#define GVTSTART        72      /* start a GVT calculation      */
#define GVTACK          73      /* acknowledge the a GVTSTART   */
#define GVTSTOP         74      /* stop a GVT calculation       */
#define GVTLVT          75      /* report local minimum LVT     */
#define GVTUPDATE       76      /* report a new GVT value       */
#define GVTCHANGERATE   77      /* Change GVT interval          */

typedef Int Loadtype;

#define LOADSTART       81      /* Initialize load gathering routines   */
#define LOADIN          82      /* Incoming load gathering order        */
#define LOADUPDATE      83      /* List of current loads                */


/* Load management object selection strategies */

#define BEST_FIT 1
#define NEXT_BEST_FIT 2

/* Load management splitting strategies */

#define NEAR_FUTURE 1
#define MINIMAL_SPLIT 2
#define NO_SPLIT 3
#define LIMIT_EMSGS 4

#define MAX_MIGR 128

typedef struct G_struct {       /* GVT Message Buffer */
	Gvttype      msgtype ;      /* type of the message */
	Int          sender ;       /* which node sent the messsage */
	VTime        time ;         /* a virtual time */
	Int          flags;         /* no_stdout            */
	} 
		Gvtmsg ;

typedef struct L_struct {       /* Load Message Buffer */
	Loadtype     msgtype ;      /* type of the message */
	Int          sender ;       /* which node sent the messsage */
	VTime        time ;         /* a virtual time */
	int          util[MAX_MIGR];
	} 
		Loadmsg ;

typedef struct C_struct {		/* Critical Path Message Buffer */
	long		Ept;			/* An ept time tag for the critical path */
	Name 		object;
	long		node;
	VTime		sndtim;			/* The virtual time of an event. */
	Byte		etype;			/* The type of an event. */
	}
		Critmsg;


typedef Int HLtype ;  /* type of the object location message */

typedef struct HL_struct {       /* Object Location Message Buffer */
	HLtype       msgtype;       /* type of the message */
	Int          sender ;       /* which node sent the messsage */
	Int          newloc ;       /* new location of object */
	VTime        time ;         /* a virtual-time datum */
	Int          response;      /* node holding object */
	Name         object;        /* object being looked for */
	Type         objtype;       /* type of object */
	VTime       phase_begin;    /* the virtual start time of a phase */
	VTime       phase_end;      /* the virtual end time of a phase */
	int         generation;     /* "timestamp" of move, for ordering */
	} 
		HLmsg ;


typedef struct
{
	char name[FILE_NAME_LENGTH];
	char filename[60];
	Byte * area;
	int size;
}
	TW_FILE;

/*--------------------------------------------------------------------------*
   MISCELLANEOUS EXTERN DATA
*--------------------------------------------------------------------------*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * *   WARNING   * * * * * * * * * * * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * *   ALL THIS STUFF NEEDS TO BE INITIALIZED IN START.C   * * * * */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/* here is a trick to give you globaldef and globalref portably */
#if DATAMASTER
#define EXTERN /**/
#else
#define EXTERN extern
#endif

EXTERN Msgh * emergbuf;
EXTERN Msgh * report_buf;

EXTERN TW_FILE tw_file[MAX_TW_FILES];

EXTERN Byte *  _prqhd ;

EXTERN State_Migr_Hdr * sendStateQ;
EXTERN State_Migr_Hdr * statesMovedQ;
EXTERN Ocb * sendOcbQ;
EXTERN struct deadOcb * DeadOcbList ;

EXTERN Byte * _hlqhd;

EXTERN int		objectCode;	/* true if executing inside object */
EXTERN Ocb * xqting_ocb ; /* points to currently executing object */

EXTERN Ocb * stdout_ocb ;

EXTERN int unique_oid;

EXTERN struct machine_parm_block {
	Int  me;
	long maxnprc;
	} miparm;

EXTERN int tw_node_num;
EXTERN int tw_num_nodes;

EXTERN int objstksize;
EXTERN int msgdefsize;
EXTERN int pktlen;
EXTERN int truncStateSize;

EXTERN VTime gvt;      
		/* estimate of global virtual time */

EXTERN VTime pvt;      
		/* processor vt = minimum svt of all objects on this node */

#if DLM
EXTERN VTime local_min_vt ; 
#endif DLM
EXTERN VTime min_vt ; 
		/* minimum of pvt and all message sendtimes on this node */

#if DATAMASTER
VTime posinf = { POSINF, 0, 0 };
VTime neginf = { NEGINF, 0, 0 };
VTime posinfPlus1 = { POSINF+1, 0, 0 };
VTime neginfPlus1 = { NEGINF+1, 0, 0 };
#else
extern VTime posinf;
extern VTime neginf;
extern VTime posinfPlus1;
extern VTime neginfPlus1;
#endif

EXTERN Typtbl type_table[ MAXNTYP ] ; /* Static array of object types */

EXTERN long gvtinterval ;
EXTERN long gvtcount ;

EXTERN long  high , 
			 low ;   /* storage management parameters */

EXTERN long  bytes_used ;  /* memory mgt statistics */

EXTERN long  initial_freemem_bytes ;  /* memory mgt statistics */

EXTERN Int no_stdout;   /* flag to disable stdout for timing */

EXTERN int evtlog, chklog;      /* run time options */

EXTERN float myUtilization;

EXTERN long  migrin;
EXTERN long  migrout;
EXTERN long rfaults;
EXTERN long fstateout;
EXTERN long fstatein;
EXTERN long dupStateSend;
EXTERN long didNotSendState;
EXTERN long rollbackInMigrQ;

/* some macros for the library routines */

Pointer obj_getLibPointer();
void*   obj_getLibTable();
void    obj_setLibPointer();

#define getLibPointer           (obj_getLibPointer())
#define getLibTable             (obj_getLibTable())
#define setLibPointer(x)        (obj_setLibPointer(x))

/* these macros are NOT IDENTICAL in behavior to their corresponding functions
 * in LIST.C; in particular, they do not check for NULL inputs
 */

/* get the pointer to u's header */
#define LIST_HEADER(u)    (((List_hdr *)(u))-1)

/* TRUE if header size is 0 (ie first header) */
#define l_ishead_macro(p) ( LIST_HEADER(p)->size == 0 )
/* point to the object of the header following p */
#define l_next_macro(p)   ( LIST_HEADER(p)->next + 1 )
/* point to the object of the header prior to p */
#define l_prev_macro(p)   ( LIST_HEADER(p)->prev + 1 )
/* get size of memory segment */
#define l_size(p)		  ( LIST_HEADER(p)->size )

#define negate_macro(M)    BITSET( (M)->flags,SIGN )
#define unnegate_macro(M)  BITCLR( (M)->flags,SIGN )

#define issys_macro(M)     BITEST( (M)->flags,SYSMSG )
#define isanti_macro(M)    BITEST( (M)->flags,SIGN )
#define isposi_macro(M)    (!isanti_macro(M))
#define areopp_macro(M,N)  ( (isanti_macro(M) && isposi_macro(N) ) ||  \
							 (isposi_macro(M) && isanti_macro(N) ) )

#define mark_macro(M)      BITSET( (M)->flags,USED )
#define unmark_macro(M)    BITCLR( (M)->flags,USED )
#define ismarked_macro(M)  BITEST( (M)->flags,USED )

#define set_reverse_macro(M)   BITSET( (M)->flags,DIRECTION )
#define set_forward_macro(M)   BITCLR( (M)->flags,DIRECTION )
#define isreverse_macro(M)     BITEST( (M)->flags,DIRECTION )
#define isforward_macro(M)     (!isreverse_macro(M))

#define lock_macro(M)      BITSET( (M)->flags,LOCKED )
#define unlock_macro(M)    BITCLR( (M)->flags,LOCKED )
#define islocked_macro(M)  BITEST( (M)->flags,LOCKED ) 

EXTERN  List_hdr * _temp ;

#define nxtimsg_macro(m)   ( _temp = l_next_macro(m) , \
				l_ishead_macro(_temp) ? NULLMSGH : (Msgh *)_temp )

#define nxtomsg_macro(m)   ( _temp = l_next_macro(m) , \
				l_ishead_macro(_temp) ? NULLMSGH : (Msgh *)_temp )

#define nxtocb_macro(m)    ( _temp = l_next_macro(m) , \
				l_ishead_macro(_temp) ? NULLOCB : (Ocb *)_temp )

#define nxtDocb_macro(m)    ( _temp = l_next_macro(m) , \
			  l_ishead_macro(_temp) ? NULLDOCB : (deadOcb *)_temp )

#define nxtstate_macro(m)   ( _temp = l_next_macro(m) , \
				l_ishead_macro(_temp) ? NULLSTATE : (State *)_temp )


#define nxtmigrh_macro(m)   ( _temp = l_next_macro(m) , \
				l_ishead_macro(_temp) ? NULLSTATE : \
									    (State_Migr_Hdr *)_temp )




#define prvimsg_macro(m)   ( _temp = l_prev_macro(m) , \
				l_ishead_macro(_temp) ? NULLMSGH : (Msgh *)_temp )

#define prvomsg_macro(m)   ( _temp = l_prev_macro(m) , \
				l_ishead_macro(_temp) ? NULLMSGH : (Msgh *)_temp )

#define prvocb_macro(m)    ( _temp = l_prev_macro(m) , \
				l_ishead_macro(_temp) ? NULLOCB : (Ocb *)_temp )

#define prvstate_macro(m)   ( _temp = l_prev_macro(m) , \
				l_ishead_macro(_temp) ? NULLSTATE : (State *)_temp )

#define prvDocb_macro(m)    ( _temp = l_prev_macro(m) , \
			  l_ishead_macro(_temp) ? NULLDOCB : (deadOcb *)_temp )



#define fstimsg_macro(o)  ( _temp = l_next_macro(o->iqh) , \
				l_ishead_macro(_temp) ? NULLMSGH : (Msgh *)_temp ) 

#define fstomsg_macro(o)  ( _temp = l_next_macro(o->oqh) , \
				l_ishead_macro(_temp) ? NULLMSGH : (Msgh *)_temp ) 

#define fststate_macro(o)  ( _temp = l_next_macro(o->sqh) , \
				l_ishead_macro(_temp) ? NULLSTATE : (State *)_temp ) 

#define fstocb_macro  ( _temp = l_next_macro( _prqhd ) , \
				l_ishead_macro(_temp) ? NULLOCB : (Ocb *)_temp ) 

#define fstDocb_macro  ( _temp = l_next_macro( DeadOcbList ) , \
			  l_ishead_macro(_temp) ? NULLDOCB : (deadOcb *)_temp )

#define lstimsg_macro(o)  ( _temp = l_prev_macro(o->iqh) , \
				l_ishead_macro(_temp) ? NULLMSGH : (Msgh *)_temp ) 

#define lstomsg_macro(o)  ( _temp = l_prev_macro(o->oqh) , \
				l_ishead_macro(_temp) ? NULLMSGH : (Msgh *)_temp ) 

#define lststate_macro(o)  ( _temp = l_prev_macro(o->sqh) , \
				l_ishead_macro(_temp) ? NULLSTATE : (State *)_temp ) 

#define lstocb_macro  ( _temp = l_prev_macro( _prqhd ) , \
				l_ishead_macro(_temp) ? NULLOCB : (Ocb *)_temp ) 

#define lstDocb_macro  ( _temp = l_prev_macro( DeadOcbList ) , \
			  l_ishead_macro(_temp) ? NULLDOCB : (deadOcb *)_temp )

#define dqimsg(m)       l_remove(m)
#define dqomsg(m)       l_remove(m)
#define delimsg(m)      l_remove(m); l_destroy(m)
#define delomsg(m)      l_remove(m); l_destroy(m)

#ifndef SIMULATOR
#include "twsys.dcl"
#endif  SIMULATOR

#undef  EXTERN
