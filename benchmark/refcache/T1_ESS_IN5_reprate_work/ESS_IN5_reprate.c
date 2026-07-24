/* Automatically generated file. Do not edit. 
 * Format:     ANSI C source code
 * Creator:    McStas <http://www.mcstas.org>
 * Instrument: /opt/homebrew/Caskroom/miniconda/base/envs/mcstas/share/mcstas/resources/examples/ESS/ESS_IN5_reprate/ESS_IN5_reprate.instr (ESS_IN5_reprate)
 * Date:       Fri Jul 24 15:27:53 2026
 * File:       ./ESS_IN5_reprate.c
 * CFLAGS=
 */

#ifndef WIN32
#  ifndef OPENACC
#    define _GNU_SOURCE
#  endif
#  define _POSIX_C_SOURCE 200809L
#endif
/* In case of cl.exe on Windows, supppress warnings about #pragma acc */
#ifdef _MSC_EXTENSIONS
#pragma warning(disable: 4068)
#endif

#define MCCODE_STRING " 3.7.12, git"
#define FLAVOR        "mcstas"
#define FLAVOR_UPPER  "MCSTAS"

#define MC_USE_DEFAULT_MAIN
#define MC_TRACE_ENABLED

#include <string.h>
#include <inttypes.h>

typedef double MCNUM;
typedef struct {MCNUM x, y, z;} Coords;
typedef MCNUM Rotation[3][3];
#define MCCODE_BASE_TYPES

/* available random number generators */
#define _RNG_ALG_MT         1
#define _RNG_ALG_KISS       2
/* selection of random number generator */
#ifndef RNG_ALG
#  define RNG_ALG  _RNG_ALG_KISS
#endif
#if RNG_ALG == _RNG_ALG_MT // MT 
#define randstate_t uint32_t
#elif RNG_ALG == _RNG_ALG_KISS  // KISS
#define randstate_t uint64_t
#endif

#ifndef MC_NUSERVAR
#define MC_NUSERVAR 10
#endif

/* Particle JUMP control logic */
struct particle_logic_struct {
int dummy;
};

struct _struct_particle {
  double x,y,z; /* position [m] */
  double vx,vy,vz; /* velocity [m/s] */
  double sx,sy,sz; /* spin [0-1] */
  int mcgravitation; /* gravity-state */
  void *mcMagnet;    /* precession-state */
  int allow_backprop; /* allow backprop */
  /* Generic Temporaries: */
  /* May be used internally by components e.g. for special */
  /* return-values from functions used in trace, thusreturned via */
  /* particle struct. (Example: Wolter Conics from McStas, silicon slabs.) */
  double _mctmp_a; /* temp a */
  double _mctmp_b; /* temp b */
  double _mctmp_c; /* temp c */
  randstate_t randstate[7];
  double t, p;     /* time, event weight */
  long long _uid;  /* Unique event ID */
  long _index;     /* component index where to send this event */
  long _absorbed;  /* flag set to TRUE when this event is to be removed/ignored */
  long _scattered; /* flag set to TRUE when this event has interacted with the last component instance */
  long _restore;   /* set to true if neutron event must be restored */
  long flag_nocoordschange;   /* set to true if particle is jumping */
  struct particle_logic_struct _logic;
};
typedef struct _struct_particle _class_particle;

_class_particle _particle_global_randnbuse_var;
_class_particle* _particle = &_particle_global_randnbuse_var;

#pragma acc routine
_class_particle mcgenstate(void);
#pragma acc routine
_class_particle mcsetstate(double x, double y, double z, double vx, double vy, double vz,
			   double t, double sx, double sy, double sz, double p, int mcgravitation, void *mcMagnet, int mcallowbackprop);
#pragma acc routine
_class_particle mcgetstate(_class_particle mcneutron, double *x, double *y, double *z,
                           double *vx, double *vy, double *vz, double *t,
                           double *sx, double *sy, double *sz, double *p);

extern int mcgravitation;      /* flag to enable gravitation */
#pragma acc declare create ( mcgravitation )

_class_particle mcgenstate(void) {
  _class_particle particle = mcsetstate(0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, mcgravitation, NULL, 0);
  return(particle);
}
/*Generated user variable handlers:*/

#pragma acc routine
double particle_getvar(_class_particle *p, char *name, int *suc);

#ifdef OPENACC
#pragma acc routine
int str_comp(char *str1, char *str2);
#endif

double particle_getvar(_class_particle *p, char *name, int *suc){
#ifndef OPENACC
#define str_comp strcmp
#endif
  int s=1;
  double rval=0;
  if(!str_comp("x",name)){rval=p->x;s=0;}
  if(!str_comp("y",name)){rval=p->y;s=0;}
  if(!str_comp("z",name)){rval=p->z;s=0;}
  if(!str_comp("vx",name)){rval=p->vx;s=0;}
  if(!str_comp("vy",name)){rval=p->vy;s=0;}
  if(!str_comp("vz",name)){rval=p->vz;s=0;}
  if(!str_comp("sx",name)){rval=p->sx;s=0;}
  if(!str_comp("sy",name)){rval=p->sy;s=0;}
  if(!str_comp("sz",name)){rval=p->sz;s=0;}
  if(!str_comp("t",name)){rval=p->t;s=0;}
  if(!str_comp("p",name)){rval=p->p;s=0;}
  if(!str_comp("_mctmp_a",name)){rval=p->_mctmp_a;s=0;}
  if(!str_comp("_mctmp_b",name)){rval=p->_mctmp_b;s=0;}
  if(!str_comp("_mctmp_c",name)){rval=p->_mctmp_c;s=0;}
  if (suc!=0x0) {*suc=s;}
  return rval;
}

#pragma acc routine
void* particle_getvar_void(_class_particle *p, char *name, int *suc);

#ifdef OPENACC
#pragma acc routine
int str_comp(char *str1, char *str2);
#endif

void* particle_getvar_void(_class_particle *p, char *name, int *suc){
#ifndef OPENACC
#define str_comp strcmp
#endif
  int s=1;
  void* rval=0;
  if(!str_comp("x",name)) {rval=(void*)&(p->x); s=0;}
  if(!str_comp("y",name)) {rval=(void*)&(p->y); s=0;}
  if(!str_comp("z",name)) {rval=(void*)&(p->z); s=0;}
  if(!str_comp("vx",name)){rval=(void*)&(p->vx);s=0;}
  if(!str_comp("vy",name)){rval=(void*)&(p->vy);s=0;}
  if(!str_comp("vz",name)){rval=(void*)&(p->vz);s=0;}
  if(!str_comp("sx",name)){rval=(void*)&(p->sx);s=0;}
  if(!str_comp("sy",name)){rval=(void*)&(p->sy);s=0;}
  if(!str_comp("sz",name)){rval=(void*)&(p->sz);s=0;}
  if(!str_comp("t",name)) {rval=(void*)&(p->t); s=0;}
  if(!str_comp("p",name)) {rval=(void*)&(p->p); s=0;}
  if (suc!=0x0) {*suc=s;}
  return rval;
}

#pragma acc routine
int particle_setvar_void(_class_particle *, char *, void*);

int particle_setvar_void(_class_particle *p, char *name, void* value){
#ifndef OPENACC
#define str_comp strcmp
#endif
  int rval=1;
  if(!str_comp("x",name)) {memcpy(&(p->x),  value, sizeof(double)); rval=0;}
  if(!str_comp("y",name)) {memcpy(&(p->y),  value, sizeof(double)); rval=0;}
  if(!str_comp("z",name)) {memcpy(&(p->z),  value, sizeof(double)); rval=0;}
  if(!str_comp("vx",name)){memcpy(&(p->vx), value, sizeof(double)); rval=0;}
  if(!str_comp("vy",name)){memcpy(&(p->vy), value, sizeof(double)); rval=0;}
  if(!str_comp("vz",name)){memcpy(&(p->vz), value, sizeof(double)); rval=0;}
  if(!str_comp("sx",name)){memcpy(&(p->sx), value, sizeof(double)); rval=0;}
  if(!str_comp("sy",name)){memcpy(&(p->sy), value, sizeof(double)); rval=0;}
  if(!str_comp("sz",name)){memcpy(&(p->sz), value, sizeof(double)); rval=0;}
  if(!str_comp("p",name)) {memcpy(&(p->p),  value, sizeof(double)); rval=0;}
  if(!str_comp("t",name)) {memcpy(&(p->t),  value, sizeof(double)); rval=0;}
  return rval;
}

#pragma acc routine
int particle_setvar_void_array(_class_particle *, char *, void*, int);

int particle_setvar_void_array(_class_particle *p, char *name, void* value, int elements){
#ifndef OPENACC
#define str_comp strcmp
#endif
  int rval=1;
  return rval;
}

#pragma acc routine
void particle_restore(_class_particle *p, _class_particle *p0);

void particle_restore(_class_particle *p, _class_particle *p0) {
  p->x  = p0->x;  p->y  = p0->y;  p->z  = p0->z;
  p->vx = p0->vx; p->vy = p0->vy; p->vz = p0->vz;
  p->sx = p0->sx; p->sy = p0->sy; p->sz = p0->sz;
  p->t = p0->t;  p->p  = p0->p;
  p->_absorbed=0; p->_restore=0;
}

#pragma acc routine
double particle_getuservar_byid(_class_particle *p, int id, int *suc){
  int s=1;
  double rval=0;
  switch(id){
  }
  if (suc!=0x0) {*suc=s;}
  return rval;
}

#pragma acc routine
void particle_uservar_init(_class_particle *p){
}

#define MC_EMBEDDED_RUNTIME
/* embedding file "mccode-r.h" */

/*******************************************************************************
*
* McCode, neutron/xray ray-tracing package
*         Copyright (C) 1997-2009, All rights reserved
*         Risoe National Laboratory, Roskilde, Denmark
*         Institut Laue Langevin, Grenoble, France
*
* Runtime: share/mccode-r.h
*
* %Identification
* Written by: KN
* Date:    Aug 29, 1997
* Release: mcstas 3.7.12
* Version: $Revision$
*
* Runtime system header for McStas/McXtrace.
*
* In order to use this library as an external library, the following variables
* and macros must be declared (see details in the code)
*
*   struct mcinputtable_struct mcinputtable[];
*   int numipar;
*   metadata_table_t metadata_table[];
*   int num_metadata;
*   char instrument_name[], instrument_source[];
*   int traceenabled, defaultmain;
*   extern MCNUM  mccomp_storein[];
*   extern MCNUM  mcAbsorbProp[];
*   extern MCNUM  mcScattered;
*   #define MCCODE_STRING "the McStas/McXtrace version"
*
* Usage: Automatically embbeded in the c code.
*
* $Id$
*
*******************************************************************************/

#ifndef MCCODE_R_H
#define MCCODE_R_H "$Revision$"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include <errno.h>
#include <time.h>
#ifndef _MSC_EXTENSIONS
#include <sys/time.h>
#endif
#include <float.h>
#include <inttypes.h>
#include <stdint.h>
#ifdef OPENACC
#include <openacc.h>
#ifndef GCCOFFLOAD
#include <accelmath.h>
#else
#include <math.h>
#endif
#pragma acc routine
int noprintf();
#pragma acc routine
size_t str_len(const char *s);
#else
#include <math.h>
#endif

/* In case of gcc / clang, ensure to use
   the built-in isnan/isinf functions */
#if defined(__GNUC__) || defined(__clang__)
#  ifdef isnan
#    undef isnan
#  endif
#  ifdef isinf
#    undef isinf
#  endif
#  define isnan(x) __builtin_isnan(x)
#  define isinf(x) __builtin_isinf(x)
#endif

#ifdef _MSC_EXTENSIONS
#ifndef _TIMES_H
#define _TIMES_H

#if defined(WIN32) || defined(_WIN32)
#include <sys/timeb.h>
#include <sys/types.h>
#include <winsock2.h>

int gettimeofday(struct timeval* t,void* timezone);

#define __need_clock_t
#include <time.h>


/* Structure describing CPU time used by a process and its children.  */
struct tms
  {
    clock_t tms_utime;          /* User CPU time.  */
    clock_t tms_stime;          /* System CPU time.  */

    clock_t tms_cutime;         /* User CPU time of dead children.  */
    clock_t tms_cstime;         /* System CPU time of dead children.  */
  };

/* Store the CPU time used by this process and all its
   dead children (and their dead children) in BUFFER.
   Return the elapsed real time, or (clock_t) -1 for errors.
   All times are in CLK_TCKths of a second.  */
clock_t times (struct tms *__buffer);

typedef long long suseconds_t ;



int gettimeofday(struct timeval* t,void* timezone)
{       struct _timeb timebuffer;
        _ftime( &timebuffer );
        t->tv_sec=timebuffer.time;
        t->tv_usec=1000*timebuffer.millitm;
		return 0;
}

clock_t times (struct tms *__buffer) {

	__buffer->tms_utime = clock();
	__buffer->tms_stime = 0;
	__buffer->tms_cstime = 0;
	__buffer->tms_cutime = 0;
	return __buffer->tms_utime;
}


#endif
#endif
#endif

/* If the runtime is embedded in the simulation program, some definitions can
   be made static. */

#ifdef MC_EMBEDDED_RUNTIME
#  define mcstatic
#else
#  define mcstatic
#endif

#ifdef __dest_os
#  if (__dest_os == __mac_os)
#    define MAC
#  endif
#endif

#ifdef __FreeBSD__
#  define NEED_STAT_H
#endif

#if defined(__APPLE__) && defined(__GNUC__)
#  define NEED_STAT_H
#endif

#if defined(WIN32) || defined(_WIN32)
#  define NEED_STAT_H
#  define NEED_TYPES_H
#endif

#ifdef NEED_STAT_H
#  include <sys/stat.h>
#endif

#ifdef NEED_TYPES_H
#  include <sys/types.h>
#endif

#ifndef MC_PATHSEP_C
#if defined(WIN32) || defined(_WIN32)
#    define MC_PATHSEP_C '\\'
#    define MC_PATHSEP_S "\\"
#  else  /* !WIN32 */
#    define MC_PATHSEP_C '/'
#    define MC_PATHSEP_S "/"
#  endif /* !WIN32 */
#endif /* MC_PATHSEP_C */

#if defined(WIN32) || defined(_WIN32)
#if defined _MSC_VER
#include <direct.h>
#elif defined __GNUC__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#define mkdir(a,b) mkdir(a)
#define getpid() _getpid()
#endif

/* the version string is replaced when building distribution with mkdist */
#ifndef MCCODE_STRING
#  define MCCODE_STRING " 3.7.12, git"
#endif

#ifndef MCCODE_DATE
#  define MCCODE_DATE "git"
#endif

#ifndef MCCODE_VERSION
#  define MCCODE_VERSION "3.7.12"
#endif

#ifndef __MCCODE_VERSION__
#define __MCCODE_VERSION__ 307012L
#endif

#ifndef MCCODE_NAME
#  define MCCODE_NAME "mcstas"
#endif

#ifndef MCCODE_PARTICLE
#  define MCCODE_PARTICLE "neutron"
#endif

#ifndef MCCODE_PARTICLE_CODE
#  define MCCODE_PARTICLE_CODE 2112
#endif

#ifndef MCCODE_LIBENV
#  define MCCODE_LIBENV "MCSTAS"
#endif

#ifndef FLAVOR_UPPER
#  define FLAVOR_UPPER MCCODE_NAME
#endif

#ifdef MC_PORTABLE
#  ifndef NOSIGNALS
#    define NOSIGNALS 1
#  endif
#endif

#ifdef MAC
#  ifndef NOSIGNALS
#    define NOSIGNALS 1
#  endif
#endif

#if (USE_MPI == 0)
#  undef USE_MPI
#endif

#ifdef USE_MPI  /* default is to disable signals with MPI, as MPICH uses them to communicate */
#  ifndef NOSIGNALS
#    define NOSIGNALS 1
#  endif
#endif

#ifdef OPENACC  /* default is to disable signals with PGI/OpenACC */
#  ifndef NOSIGNALS
#    define NOSIGNALS 1
#  endif
#endif

#ifndef OPENACC
#  ifndef USE_OFF  /* default is to enable OFF when not using PGI/OpenACC */
#    define USE_OFF
#  endif
#  ifndef CPUFUNNEL  /* allow to enable FUNNEL-mode on CPU */
#  ifdef FUNNEL      /* by default disable FUNNEL-mode when not using PGI/OpenACC */
#    undef FUNNEL
#  endif
#  endif
#endif

#if (NOSIGNALS == 0)
#  undef NOSIGNALS
#endif

/** Header information for metadata-r.c ----------------------------------------------------------------------------- */
struct metadata_table_struct { /* stores metadata strings from components */
  char * source;  // component name which provided the metadata
  char * name;    // the name of the metadata
  char * type;    // the MIME type of the metadata (free form, valid identifier)
  char * value;   // the metadata string contents
};
typedef struct metadata_table_struct metadata_table_t;
char * metadata_table_key_component(char* key);
char * metadata_table_key_literal(char * key);
int metadata_table_defined(int, metadata_table_t *, char *);
char * metadata_table_name(int, metadata_table_t *, char *);
char * metadata_table_type(int, metadata_table_t *, char *);
char * metadata_table_literal(int, metadata_table_t *, char *);
void metadata_table_print_all_keys(int no, metadata_table_t * tab);
int metadata_table_print_all_components(int no, metadata_table_t * tab);
int metadata_table_print_component_keys(int no, metadata_table_t * tab, char * key);
/* -------------------------------------------------------------------------- Header information for metadata-r.c --- */

/* Note: the enum instr_formal_types definition MUST be kept
   synchronized with the one in mccode.h and with the
   instr_formal_type_names array in cogen.c. */
enum instr_formal_types
  {
    instr_type_int,
    instr_type_string, instr_type_char,
    instr_type_vector, instr_type_double
  };
struct mcinputtable_struct { /* defines instrument parameters */
  char *name; /* name of parameter */
  void *par;  /* pointer to instrument parameter (variable) */
  enum instr_formal_types type;
  char *val;  /* default value */
  char *unit; /* expected unit for parameter; informational only */
};


#ifndef MCCODE_BASE_TYPES
typedef double MCNUM;
typedef struct {MCNUM x, y, z;} Coords;
typedef MCNUM Rotation[3][3];
#endif

/* the following variables are defined in the McStas generated C code
   but should be defined externally in case of independent library usage */
#ifndef DANSE
extern struct mcinputtable_struct mcinputtable[];         /* list of instrument parameters */
extern int    numipar;                                    /* number of instrument parameters */
extern metadata_table_t metadata_table[];                 /* list of component-defined string metadata */
extern int    num_metadata;                               /* number of component-defined string metadata */
extern char   instrument_name[], instrument_source[]; /* instrument name and filename */
extern char  *instrument_exe;                           /* executable path = argv[0] or NULL */
extern char   instrument_code[];                        /* contains the initial 'instr' file */

#ifndef MC_ANCIENT_COMPATIBILITY
extern int traceenabled, defaultmain;
#endif
#endif


/* Useful macros ============================================================ */


/* SECTION: Dynamic Arrays */
typedef int* IArray1d;
IArray1d create_iarr1d(int n);
void destroy_iarr1d(IArray1d a);

typedef int** IArray2d;
IArray2d create_iarr2d(int nx, int ny);
void destroy_iarr2d(IArray2d a);

typedef int*** IArray3d;
IArray3d create_iarr3d(int nx, int ny, int nz);
void destroy_iarr3d(IArray3d a);

typedef double* DArray1d;
DArray1d create_darr1d(int n);
void destroy_darr1d(DArray1d a);

typedef double** DArray2d;
DArray2d create_darr2d(int nx, int ny);
void destroy_darr2d(DArray2d a);

typedef double*** DArray3d;
DArray3d create_darr3d(int nx, int ny, int nz);
void destroy_darr3d(DArray3d a);


/* MPI stuff */
#ifdef USE_MPI
#include "mpi.h"

#ifdef OMPI_MPI_H  /* openmpi does not use signals: we may install our sighandler */
#ifndef OPENACC    /* ... but only if we are not also running on GPU */
#undef NOSIGNALS
#endif
#endif

/*
 * MPI_MASTER(i):
 * execution of i only on master node
 */
#define MPI_MASTER(statement) { \
  if(mpi_node_rank == mpi_node_root)\
  { statement; } \
}

#ifndef MPI_REDUCE_BLOCKSIZE
#define MPI_REDUCE_BLOCKSIZE 100000
#endif

int mc_MPI_Sum(double* buf, long count);
int mc_MPI_Send(void *sbuf, long count, MPI_Datatype dtype, int dest);
int mc_MPI_Recv(void *rbuf, long count, MPI_Datatype dtype, int source);

/* MPI_Finalize exits gracefully and should be preferred to MPI_Abort */
#define exit(code) do {                                   \
    MPI_Finalize();                                       \
    exit(code);                                           \
  } while(0)

#else /* !USE_MPI */
#define MPI_MASTER(instr) instr
#endif /* USE_MPI */


#ifdef USE_MPI
static int mpi_node_count;
#endif

#ifdef USE_THREADS  /* user want threads */
#error Threading (USE_THREADS) support has been removed for very poor efficiency. Use MPI/SSH grid instead.
#endif


void   mcset_ncount(unsigned long long count);    /* wrapper to get mcncount */
#pragma acc routine
unsigned long long int mcget_ncount(void);            /* wrapper to set mcncount */
unsigned long long mcget_run_num(void);           /* wrapper to get mcrun_num=0:mcncount-1 */

/* Following part is only embedded when not redundant with mccode.h ========= */

#ifndef MCCODE_H

#ifndef NOSIGNALS
#include <signal.h>
char  *mcsig_message;
#define SIG_MESSAGE(msg) mcsig_message=(char *)(msg);
#else
#define SIG_MESSAGE(...)
#endif /* !NOSIGNALS */


/* Useful macros and constants ============================================== */


#ifndef FLT_MAX
#define FLT_MAX         3.40282347E+38F /* max decimal value of a "float" */
#endif

#ifndef MIN
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#endif
#ifndef SQR
#define SQR(x) ( (x) * (x) )
#endif
#ifndef SIGN
#define SIGN(x) (((x)>0.0)?(1):(-1))
#endif


#  ifndef M_E
#    define M_E        2.71828182845904523536  // e
#  endif
#  ifndef M_LOG2E
#    define M_LOG2E    1.44269504088896340736  //  log2(e)
#  endif
#  ifndef M_LOG10E
#    define M_LOG10E   0.434294481903251827651 //  log10(e)
#  endif
#  ifndef M_LN2
#    define M_LN2      0.693147180559945309417 //  ln(2)
#  endif
#  ifndef M_LN10
#    define M_LN10     2.30258509299404568402  //  ln(10)
#  endif
#  ifndef M_PI
#    define M_PI       3.14159265358979323846  //  pi
#  endif
#  ifndef PI
#    define PI       M_PI                      //  pi - also used in some places
#  endif
#  ifndef M_PI_2
#    define M_PI_2     1.57079632679489661923  //  pi/2
#  endif
#  ifndef M_PI_4
#    define M_PI_4     0.785398163397448309616 //  pi/4
#  endif
#  ifndef M_1_PI
#    define M_1_PI     0.318309886183790671538 //  1/pi
#  endif
#  ifndef M_2_PI
#    define M_2_PI     0.636619772367581343076 //  2/pi
#  endif
#  ifndef M_2_SQRTPI
#    define M_2_SQRTPI 1.12837916709551257390  //  2/sqrt(pi)
#  endif
#  ifndef M_SQRT2
#    define M_SQRT2    1.41421356237309504880  //  sqrt(2)
#  endif
#  ifndef M_SQRT1_2
#    define M_SQRT1_2  0.707106781186547524401 //  1/sqrt(2)
#  endif

#define RAD2MIN  ((180*60)/PI)
#define MIN2RAD  (PI/(180*60))
#define DEG2RAD  (PI/180)
#define RAD2DEG  (180/PI)
#define FWHM2RMS 0.424660900144    /* Convert between full-width-half-max and */
#define RMS2FWHM 2.35482004503     /* root-mean-square (standard deviation) */
#define HBAR     1.05457168e-34    /* [Js] h bar Planck constant CODATA 2002 */
#define MNEUTRON 1.67492728e-27    /* [kg] mass of neutron CODATA 2002 */
#define GRAVITY  9.81              /* [m/s^2] gravitational acceleration */
#define NA       6.02214179e23     /* [#atoms/g .mole] Avogadro's number*/


#define UNSET nan("0x6E6F74736574")
int nans_match(double, double);
int is_unset(double);
int is_valid(double);
int is_set(double);
int all_unset(int n, ...);
int all_set(int n, ...);
int any_unset(int n, ...);
int any_set(int n, ...);


/* wrapper to get absolute and relative position of comp */
/* mccomp_posa and mccomp_posr are defined in McStas generated C code */
#define POS_A_COMP_INDEX(index) (instrument->_position_absolute[index])
#define POS_R_COMP_INDEX(index) (instrument->_position_relative[index])

/* setting parameters based COMP_GETPAR (returned as pointer)         */
/* compname must be given as a string, type and par are symbols.      */
#define COMP_GETPAR3(type, compname, par) \
    &( ((_class_ ## type ##_parameters *) _getvar_parameters(compname))->par )
/* the body of this function depends on component instances, and is cogen'd */
void* _getvar_parameters(char* compname);

int _getcomp_index(char* compname);

/* Note: The two-stage approach to COMP_GETPAR is NOT redundant; without it,
* after #define C sample, COMP_GETPAR(C,x) would refer to component C, not to
* component sample. Such are the joys of ANSI C.

* Anyway the usage of COMP_GETPAR requires that we use sometimes bare names...
* NOTE: This can ONLY be used in instrument descriptions, not components.
*/
#define COMP_GETPAR2(comp, par) (_ ## comp ## _var._parameters.par)
#define COMP_GETPAR(comp, par) COMP_GETPAR2(comp,par)

#define INSTRUMENT_GETPAR(par) (_instrument_var._parameters.par)

/* Current component name, index, position and orientation */
/* These macros work because, using class-based functions, "comp" is usually
*  the local variable of the active/current component. */
#define INDEX_CURRENT_COMP (_comp->_index)
#define NAME_CURRENT_COMP (_comp->_name)
#define TYPE_CURRENT_COMP (_comp->_type)
#define POS_A_CURRENT_COMP (_comp->_position_absolute)
#define POS_R_CURRENT_COMP (_comp->_position_relative)
#define ROT_A_CURRENT_COMP (_comp->_rotation_absolute)
#define ROT_R_CURRENT_COMP (_comp->_rotation_relative)

#define NAME_INSTRUMENT (instrument->_name)


/* MCDISPLAY/trace and debugging message sent to stdout */
#ifdef MC_TRACE_ENABLED
#define DEBUG
#endif

#ifdef DEBUG
#define DEBUG_INSTR() if(!mcdotrace); else { printf("INSTRUMENT:\n"); printf("Instrument '%s' (%s)\n", instrument_name, instrument_source); }
#define DEBUG_COMPONENT(name,c,t) if(!mcdotrace); else {\
     printf("COMPONENT: \"%s\"\n"					  \
     "POS: %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g\n", \
     name, c.x, c.y, c.z, t[0][0], t[0][1], t[0][2], \
     t[1][0], t[1][1], t[1][2], t[2][0], t[2][1], t[2][2]); \
     fflush(stdout);\
     printf("Component %30s AT (%g,%g,%g)\n", name, c.x, c.y, c.z);\
     fflush(stdout);}
#define DEBUG_INSTR_END() if(!mcdotrace); else printf("INSTRUMENT END:\n");
#define DEBUG_ENTER() if(!mcdotrace); else printf("ENTER:\n");
#define DEBUG_COMP(c) if(!mcdotrace); else printf("COMP: \"%s\"\n", c);
#define DEBUG_LEAVE() if(!mcdotrace); else printf("LEAVE:\n");
#define DEBUG_ABSORB() if(!mcdotrace); else printf("ABSORB:\n");
#else
#define DEBUG_INSTR()
#define DEBUG_COMPONENT(name,c,t)
#define DEBUG_INSTR_END()
#define DEBUG_ENTER()
#define DEBUG_COMP(c)
#define DEBUG_LEAVE()
#define DEBUG_ABSORB()
#endif

// mcDEBUG_STATE and mcDEBUG_SCATTER are defined by mcstas-r.h and mcxtrace-r.h



#ifdef TEST
#define test_printf printf
#else
#define test_printf while(0) printf
#endif

/* send MCDISPLAY message to stdout to show gemoetry */
void mcdis_magnify(char *what);
void mcdis_line(double x1, double y1, double z1,
                double x2, double y2, double z2);
void mcdis_dashed_line(double x1, double y1, double z1,
		       double x2, double y2, double z2, int n);
void mcdis_multiline(int count, ...);
void mcdis_rectangle(char* plane, double x, double y, double z,
		     double width, double height);
void mcdis_box(double x, double y, double z,
	       double width, double height, double length, double thickness, double nx, double ny, double nz);
void mcdis_circle(char *plane, double x, double y, double z, double r);
void mcdis_Circle(double x, double y, double z, double r, double nx, double ny, double nz);
void mcdis_cylinder( double x, double y, double z,
		     double r, double height, double thickness, double nx, double ny, double nz);
void mcdis_cone( double x, double y, double z,
        double r, double height, double nx, double ny, double nz);
void mcdis_sphere(double x, double y, double z, double r);


/* random number generation. ================================================ */

#if RNG_ALG == _RNG_ALG_MT  // MT (currently not functional for GPU)
#  define MC_RAND_MAX ((uint32_t)0xffffffffUL)
#  define RANDSTATE_LEN 1
#  define srandom(seed) mt_srandom_empty()
#  define random() mt_random()
#  define _random() mt_random()
#elif RNG_ALG == _RNG_ALG_KISS  // KISS
#  ifndef UINT64_MAX
#    define UINT64_MAX ((uint64_t)0xffffffffffffffffULL)
#  endif
#  define MC_RAND_MAX UINT64_MAX
#  define RANDSTATE_LEN 7
#  define srandom(seed) kiss_srandom(_particle->randstate, seed)
#  define random() kiss_random(_particle->randstate)
#  define _random() kiss_random(state)
#endif

#pragma acc routine
double _randnorm2(randstate_t* state);

// Component writer interface
#define randnorm() _randnorm2(_particle->randstate)        // NOTE: can't use _randnorm on GPU
#define rand01() _rand01(_particle->randstate)
#define randpm1() _randpm1(_particle->randstate)
#define rand0max(p1) _rand0max(p1, _particle->randstate)
#define randminmax(p1, p2) _randminmax(p1, p2, _particle->randstate)
#define randtriangle() _randtriangle(_particle->randstate)

// Mersenne Twister rng
uint32_t mt_random(void);
void mt_srandom (uint32_t x);
void mt_srandom_empty();

// KISS rng
#pragma acc routine
uint64_t *kiss_srandom(uint64_t state[7], uint64_t seed);
#pragma acc routine
uint64_t kiss_random(uint64_t state[7]);

// Scrambler / hash function
#pragma acc routine seq
randstate_t _hash(randstate_t x);

// internal RNG (transforms) interface
#pragma acc routine
double _rand01(randstate_t* state);
#pragma acc routine
double _randpm1(randstate_t* state);
#pragma acc routine
double _rand0max(double max, randstate_t* state);
#pragma acc routine
double _randminmax(double min, double max, randstate_t* state);
#pragma acc routine
double _randtriangle(randstate_t* state);
// version which pass randstate_t* as opague void*
#pragma acc routine
double _rand01_opague(void* state);


#ifdef USE_OPENCL
#include "opencl-lib.h"
#include "opencl-lib.c"
#endif

#ifndef DANSE
int init(void);
int raytrace(_class_particle*);
int save(FILE *);
int finally(void);
int display(void);
#endif


/* GPU related algorithms =================================================== */

/*
*  Divide-and-conquer strategy for parallel sort absorbed last.
*/
#ifdef FUNNEL
long sort_absorb_last(_class_particle* particles, _class_particle* pbuffer, long len, long buffer_len, long flag_split, long* multiplier);
#endif
long sort_absorb_last_serial(_class_particle* particles, long len);


/* simple vector algebra ==================================================== */


#define vec_prod(x, y, z, x1, y1, z1, x2, y2, z2) \
	vec_prod_func(&x, &y, &z, x1, y1, z1, x2, y2, z2)
#pragma acc routine seq
mcstatic void vec_prod_func(double *x, double *y, double *z,
		double x1, double y1, double z1, double x2, double y2, double z2);

#pragma acc routine seq
mcstatic double scalar_prod(
		double x1, double y1, double z1, double x2, double y2, double z2);

#pragma acc routine seq
mcstatic void norm_func(double *x, double *y, double *z);
#define NORM(x,y,z)	norm_func(&x, &y, &z)

#pragma acc routine seq
void normal_vec(double *nx, double *ny, double *nz,
    double x, double y, double z);

/**
 * Rotate the vector vx,vy,vz psi radians around the vector ax,ay,az
 * and put the result in x,y,z.
 */
#define rotate(x, y, z, vx, vy, vz, phi, ax, ay, az) \
  do { \
    double mcrt_tmpx = (ax), mcrt_tmpy = (ay), mcrt_tmpz = (az); \
    double mcrt_vp, mcrt_vpx, mcrt_vpy, mcrt_vpz; \
    double mcrt_vnx, mcrt_vny, mcrt_vnz, mcrt_vn1x, mcrt_vn1y, mcrt_vn1z; \
    double mcrt_bx, mcrt_by, mcrt_bz; \
    double mcrt_cos, mcrt_sin; \
    NORM(mcrt_tmpx, mcrt_tmpy, mcrt_tmpz); \
    mcrt_vp = scalar_prod((vx), (vy), (vz), mcrt_tmpx, mcrt_tmpy, mcrt_tmpz); \
    mcrt_vpx = mcrt_vp*mcrt_tmpx; \
    mcrt_vpy = mcrt_vp*mcrt_tmpy; \
    mcrt_vpz = mcrt_vp*mcrt_tmpz; \
    mcrt_vnx = (vx) - mcrt_vpx; \
    mcrt_vny = (vy) - mcrt_vpy; \
    mcrt_vnz = (vz) - mcrt_vpz; \
    vec_prod(mcrt_bx, mcrt_by, mcrt_bz, \
             mcrt_tmpx, mcrt_tmpy, mcrt_tmpz, mcrt_vnx, mcrt_vny, mcrt_vnz); \
    mcrt_cos = cos((phi)); mcrt_sin = sin((phi)); \
    mcrt_vn1x = mcrt_vnx*mcrt_cos + mcrt_bx*mcrt_sin; \
    mcrt_vn1y = mcrt_vny*mcrt_cos + mcrt_by*mcrt_sin; \
    mcrt_vn1z = mcrt_vnz*mcrt_cos + mcrt_bz*mcrt_sin; \
    (x) = mcrt_vpx + mcrt_vn1x; \
    (y) = mcrt_vpy + mcrt_vn1y; \
    (z) = mcrt_vpz + mcrt_vn1z; \
  } while(0)

/**
 * Mirror (xyz) in the plane given by the point (rx,ry,rz) and normal (nx,ny,nz)
 *
 * TODO: This define is seemingly never used...
 */
#define mirror(x,y,z,rx,ry,rz,nx,ny,nz) \
  do { \
    double mcrt_tmpx= (nx), mcrt_tmpy = (ny), mcrt_tmpz = (nz); \
    double mcrt_tmpt; \
    NORM(mcrt_tmpx, mcrt_tmpy, mcrt_tmpz); \
    mcrt_tmpt=scalar_prod((rx),(ry),(rz),mcrt_tmpx,mcrt_tmpy,mcrt_tmpz); \
    (x) = rx -2 * mcrt_tmpt*mcrt_rmpx; \
    (y) = ry -2 * mcrt_tmpt*mcrt_rmpy; \
    (z) = rz -2 * mcrt_tmpt*mcrt_rmpz; \
  } while (0)

#pragma acc routine
Coords coords_set(MCNUM x, MCNUM y, MCNUM z);
#pragma acc routine
Coords coords_get(Coords a, MCNUM *x, MCNUM *y, MCNUM *z);
#pragma acc routine
Coords coords_add(Coords a, Coords b);
#pragma acc routine
Coords coords_sub(Coords a, Coords b);
#pragma acc routine
Coords coords_neg(Coords a);
#pragma acc routine
Coords coords_scale(Coords b, double scale);
#pragma acc routine
double coords_sp(Coords a, Coords b);
#pragma acc routine
Coords coords_xp(Coords b, Coords c);
#pragma acc routine
double coords_len(Coords a);
#pragma acc routine seq
void   coords_print(Coords a);
#pragma acc routine seq
mcstatic void coords_norm(Coords* c);

#pragma acc routine seq
void rot_set_rotation(Rotation t, double phx, double phy, double phz);
#pragma acc routine seq
int  rot_test_identity(Rotation t);
#pragma acc routine seq
void rot_mul(Rotation t1, Rotation t2, Rotation t3);
#pragma acc routine seq
void rot_copy(Rotation dest, Rotation src);
#pragma acc routine seq
void rot_transpose(Rotation src, Rotation dst);
#pragma acc routine seq
Coords rot_apply(Rotation t, Coords a);

#pragma acc routine seq
void mccoordschange(Coords a, Rotation t, _class_particle *particle);
#pragma acc routine seq
void mccoordschange_polarisation(Rotation t, double *sx, double *sy, double *sz);

double mcestimate_error(double N, double p1, double p2);
void mcreadparams(void);

/* this is now in mcstas-r.h and mcxtrace-r.h as the number of state parameters
is no longer equal */

_class_particle mcgenstate(void);

// trajectory/shape intersection routines
#pragma acc routine seq
int inside_rectangle(double, double, double, double);
#pragma acc routine seq
int box_intersect(double *dt_in, double *dt_out, double x, double y, double z,
      double vx, double vy, double vz, double dx, double dy, double dz);
#pragma acc routine seq
int cylinder_intersect(double *t0, double *t1, double x, double y, double z,
      double vx, double vy, double vz, double r, double h);
#pragma acc routine seq
int sphere_intersect(double *t0, double *t1, double x, double y, double z,
      double vx, double vy, double vz, double r);
// second order equation roots
#pragma acc routine seq
int solve_2nd_order(double *t1, double *t2,
      double A,  double B,  double C);

// random vector generation to shape
// defines silently introducing _particle as the last argument
#define randvec_target_circle(xo, yo, zo, solid_angle, xi, yi, zi, radius) \
  _randvec_target_circle(xo, yo, zo, solid_angle, xi, yi, zi, radius, _particle)
#define randvec_target_rect_angular(xo, yo, zo, solid_angle, xi, yi, zi, height, width, A) \
  _randvec_target_rect_angular(xo, yo, zo, solid_angle, xi, yi, zi, height, width, A, _particle)
#define randvec_target_rect_real(xo, yo, zo, solid_angle, xi, yi, zi, height, width, A, lx, ly, lz, order) \
  _randvec_target_rect_real(xo, yo, zo, solid_angle, xi, yi, zi, height, width, A, lx, ly, lz, order, _particle)
// defines forwarding to "inner" functions
#define randvec_target_sphere randvec_target_circle
#define randvec_target_rect(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9) \
  randvec_target_rect_real(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,0,0,0,1)
// headers for randvec
#pragma acc routine seq
void _randvec_target_circle(double *xo, double *yo, double *zo,
  double *solid_angle, double xi, double yi, double zi, double radius,
  _class_particle* _particle);
#pragma acc routine seq
void _randvec_target_rect_angular(double *xo, double *yo, double *zo,
  double *solid_angle, double xi, double yi, double zi, double height,
  double width, Rotation A,
  _class_particle* _particle);
#pragma acc routine seq
void _randvec_target_rect_real(double *xo, double *yo, double *zo, double *solid_angle,
  double xi, double yi, double zi, double height, double width, Rotation A,
  double lx, double ly, double lz, int order,
  _class_particle* _particle);


// this is the main()
int mccode_main(int argc, char *argv[]);


#endif /* !MCCODE_H */

#ifndef MCCODE_R_IO_H
#define MCCODE_R_IO_H "$Revision$"

#if (USE_NEXUS == 0)
#undef USE_NEXUS
#endif

#ifndef CHAR_BUF_LENGTH
#define CHAR_BUF_LENGTH 1024
#endif


/* I/O section part ========================================================= */

/* ========================================================================== */

/*                               MCCODE_R_IO_C                                */

/* ========================================================================== */


/* main DETECTOR structure which stores most information to write to data files */
struct mcdetector_struct {
  char   filename[CHAR_BUF_LENGTH];   /* file name of monitor */
  double Position[3];                 /* position of detector component*/
  char   position[CHAR_BUF_LENGTH];   /* position of detector component (string)*/
  Rotation Rotation;                  /* position of detector component*/
  char   options[CHAR_BUF_LENGTH];    /* Monitor_nD style list-mode'options' (string)*/
  char   component[CHAR_BUF_LENGTH];  /* component instance name */
  char   nexuscomp[CHAR_BUF_LENGTH];  /* component naming in NeXus/HDF case */
  char   instrument[CHAR_BUF_LENGTH]; /* instrument name */
  char   type[CHAR_BUF_LENGTH];       /* data type, e.g. 0d, 1d, 2d, 3d */
  char   user[CHAR_BUF_LENGTH];       /* user name, e.g. HOME */
  char   date[CHAR_BUF_LENGTH];       /* date of simulation end/write time */
  char   title[CHAR_BUF_LENGTH];      /* title of detector */
  char   xlabel[CHAR_BUF_LENGTH];     /* X axis label */
  char   ylabel[CHAR_BUF_LENGTH];     /* Y axis label */
  char   zlabel[CHAR_BUF_LENGTH];     /* Z axis label */
  char   xvar[CHAR_BUF_LENGTH];       /* X variable name */
  char   yvar[CHAR_BUF_LENGTH];       /* Y variable name */
  char   zvar[CHAR_BUF_LENGTH];       /* Z variable name */
  char   ncount[CHAR_BUF_LENGTH];     /* number of events initially generated */
  char   limits[CHAR_BUF_LENGTH];     /* X Y Z limits, e.g. [xmin xmax ymin ymax zmin zmax] */
  char   variables[CHAR_BUF_LENGTH];  /* variables written into data block */
  char   statistics[CHAR_BUF_LENGTH]; /* center, mean and half width along axis */
  char   signal[CHAR_BUF_LENGTH];     /* min max and mean of signal (data block) */
  char   values[CHAR_BUF_LENGTH];     /* integrated values e.g. [I I_err N] */
  double xmin,xmax;                   /* min max of axes */
  double ymin,ymax;
  double zmin,zmax;
  double intensity;                   /* integrated values for data block */
  double error;
  double events;
  double min;                         /* statistics for data block */
  double max;
  double mean;
  double centerX;                     /* statistics for axes */
  double halfwidthX;
  double centerY;
  double halfwidthY;
  int    rank;                        /* dimensionaly of monitor, e.g. 0 1 2 3 */
  char   istransposed;                /* flag to transpose matrix for some formats */

  long   m,n,p;                       /* dimensions of data block and along axes */
  long   date_l;                      /* same as date, but in sec since 1970 */

  double *p0, *p1, *p2;               /* pointers to saved data, NULL when freed */
  char   format[CHAR_BUF_LENGTH];    /* format for file generation */
};

typedef struct mcdetector_struct MCDETECTOR;

static   char *dirname             = NULL;      /* name of output directory */
static   char *siminfo_name        = "mccode";  /* default output sim file name */
char    *mcformat                    = NULL;      /* NULL (default) or a specific format */

/* file I/O definitions and function prototypes */

#ifndef MC_EMBEDDED_RUNTIME /* the mcstatic variables (from mccode-r.c) */
extern FILE * siminfo_file;     /* handle to the output siminfo file */
extern int    mcgravitation;      /* flag to enable gravitation */
extern int    mcdotrace;          /* flag to print MCDISPLAY messages */
#else
mcstatic FILE *siminfo_file        = NULL;
#endif

/* I/O function prototypes ================================================== */

// from msysgit: https://code.google.com/p/msysgit/source/browse/compat/strcasestr.c
char *strcasestr(const char *haystack, const char *needle);

/* output functions */
MCDETECTOR mcdetector_out_0D(char *t, double p0, double p1, double p2, char *c, Coords pos, Rotation rot, int index);
MCDETECTOR mcdetector_out_1D(char *t, char *xl, char *yl,
                  char *xvar, double x1, double x2, long n,
                  double *p0, double *p1, double *p2, char *f, char *c, Coords pos, Rotation rot, int index);
MCDETECTOR mcdetector_out_2D(char *t, char *xl, char *yl,
                  double x1, double x2, double y1, double y2, long m,
                  long n, double *p0, double *p1, double *p2, char *f,
                  char *c, Coords pos, Rotation rot, int index);
MCDETECTOR mcdetector_out_list(char *t, char *xl, char *yl,
                  long m, long n,
                  double *p1, char *f,
	          char *c, Coords posa, Rotation rot,char* options, int index);

/* wrappers to output functions, that automatically set NAME and POSITION */
#define DETECTOR_OUT(p0,p1,p2) mcdetector_out_0D(NAME_CURRENT_COMP,p0,p1,p2,NAME_CURRENT_COMP,POS_A_CURRENT_COMP,ROT_A_CURRENT_COMP,INDEX_CURRENT_COMP)
#define DETECTOR_OUT_0D(t,p0,p1,p2) mcdetector_out_0D(t,p0,p1,p2,NAME_CURRENT_COMP,POS_A_CURRENT_COMP,ROT_A_CURRENT_COMP,INDEX_CURRENT_COMP)
#define DETECTOR_OUT_1D(t,xl,yl,xvar,x1,x2,n,p0,p1,p2,f) \
     mcdetector_out_1D(t,xl,yl,xvar,x1,x2,n,p0,p1,p2,f,NAME_CURRENT_COMP,POS_A_CURRENT_COMP,ROT_A_CURRENT_COMP,INDEX_CURRENT_COMP)
#define DETECTOR_OUT_2D(t,xl,yl,x1,x2,y1,y2,m,n,p0,p1,p2,f) \
     mcdetector_out_2D(t,xl,yl,x1,x2,y1,y2,m,n,p0,p1,p2,f,NAME_CURRENT_COMP,POS_A_CURRENT_COMP,ROT_A_CURRENT_COMP,INDEX_CURRENT_COMP)

#ifdef USE_NEXUS
#include "napi.h"
NXhandle nxhandle;
#endif

#endif /* ndef MCCODE_R_IO_H */

#endif /* MCCODE_R_H */
/* End of file "mccode-r.h". */

/* embedding file "mcstas-r.h" */

/*******************************************************************************
*
* McStas, neutron ray-tracing package
*         Copyright (C) 1997-2009, All rights reserved
*         Risoe National Laboratory, Roskilde, Denmark
*         Institut Laue Langevin, Grenoble, France
*
* Runtime: share/mcstas-r.h
*
* %Identification
* Written by: KN
* Date:    Aug 29, 1997
* Release: McStas X.Y
* Version: $Revision$
*
* Runtime system header for McStas.
*
* In order to use this library as an external library, the following variables
* and macros must be declared (see details in the code)
*
*   struct mcinputtable_struct mcinputtable[];
*   int mcnumipar;
*   char instrument_name[], instrument_source[];
*   int traceenabled, defaultmain;
*   extern MCNUM  mccomp_storein[];
*   extern MCNUM  instrument.counter_AbsorbProp[];
*   extern MCNUM  mcScattered;
*   #define MCCODE_STRING "the McStas version"
*
* Usage: Automatically embbeded in the c code.
*
* $Id$
*
*******************************************************************************/

#ifndef MCSTAS_R_H
#define MCSTAS_R_H "$Revision$"

/* Following part is only embedded when not redundent with mcstas.h */

#ifndef MCCODE_H

#define AA2MS    629.622368        /* Convert k[1/AA] to v[m/s] */
#define MS2AA    1.58825361e-3     /* Convert v[m/s] to k[1/AA] */
#define K2V      AA2MS
#define V2K      MS2AA
#define Q2V      AA2MS
#define V2Q      MS2AA
#define SE2V     437.393377        /* Convert sqrt(E)[meV] to v[m/s] */
#define VS2E     5.22703725e-6     /* Convert (v[m/s])**2 to E[meV] */

#define SCATTER0 do {DEBUG_SCATTER(); SCATTERED++;} while(0)
#define SCATTER SCATTER0

void SCATTER_func(_class_particle *_particle); /* provides function to SCATTER from within libaries */

#define JUMPTOCOMP(comp) mcneutron->_index = INDEX_COMP(comp);

#define MAGNET_ON \
  do { \
    mcMagnet = 1; \
  } while(0)

#define MAGNET_OFF \
  do { \
    mcMagnet = 0; \
  } while(0)

#define ALLOW_BACKPROP \
  do { \
    allow_backprop = 1; \
  } while(0)

#define DISALLOW_BACKPROP \
  do { \
    allow_backprop = 0; \
  } while(0)

#define PROP_MAGNET(dt) \
  do { \
  } while (0)
    /* change coordinates from local system to magnet system */
/*    Rotation rotLM, rotTemp; \
      Coords   posLM = coords_sub(POS_A_CURRENT_COMP, mcMagnetPos); \
      rot_transpose(ROT_A_CURRENT_COMP, rotTemp); \
      rot_mul(rotTemp, mcMagnetRot, rotLM); \
      mcMagnetPrecession(x, y, z, t, vx, vy, vz, \
               &sx, &sy, &sz, dt, posLM, rotLM); \
      } while(0)
*/

#define mcPROP_DT(dt) \
  do { \
    if (mcMagnet && dt > 0) PROP_MAGNET(dt);\
    x += vx*(dt); \
    y += vy*(dt); \
    z += vz*(dt); \
    t += (dt); \
    if (isnan(p) || isinf(p)) { ABSORB; }\
  } while(0)

/* ADD: E. Farhi, Aug 6th, 2001 PROP_GRAV_DT propagation with acceleration */
#define PROP_GRAV_DT(dt, Ax, Ay, Az) \
  do { \
    if(dt < 0 && allow_backprop == 0) { ABSORB; }\
    if (mcMagnet) /*printf("Spin precession gravity\n")*/; \
    x  += vx*(dt) + (Ax)*(dt)*(dt)/2; \
    y  += vy*(dt) + (Ay)*(dt)*(dt)/2; \
    z  += vz*(dt) + (Az)*(dt)*(dt)/2; \
    vx += (Ax)*(dt); \
    vy += (Ay)*(dt); \
    vz += (Az)*(dt); \
    t  += (dt); \
    DISALLOW_BACKPROP;\
  } while(0)


#define PROP_DT(dt) \
  do { \
    if(dt < 0 && allow_backprop == 0) { RESTORE=1; ABSORB; }; \
    if (mcgravitation) { Coords mcLocG; double mc_gx, mc_gy, mc_gz; \
    mcLocG = rot_apply(ROT_A_CURRENT_COMP, coords_set(0,-GRAVITY,0)); \
    coords_get(mcLocG, &mc_gx, &mc_gy, &mc_gz); \
    PROP_GRAV_DT(dt, mc_gx, mc_gy, mc_gz); } \
    else mcPROP_DT(dt); \
    DISALLOW_BACKPROP;\
  } while(0)


#define PROP_Z0 \
  do { \
    if (mcgravitation) { Coords mcLocG; int mc_ret; \
    double mc_dt, mc_gx, mc_gy, mc_gz; \
    mcLocG = rot_apply(ROT_A_CURRENT_COMP, coords_set(0,-GRAVITY,0)); \
    coords_get(mcLocG, &mc_gx, &mc_gy, &mc_gz); \
    mc_ret = solve_2nd_order(&mc_dt, NULL, -mc_gz/2, -vz, -z); \
    if (mc_ret) {PROP_GRAV_DT(mc_dt, mc_gx, mc_gy, mc_gz); z=0;}\
    else if (allow_backprop == 0 && mc_dt < 0) { ABSORB; }; } \
    else mcPROP_Z0; \
    DISALLOW_BACKPROP;\
  } while(0)

#define mcPROP_Z0 \
  do { \
    double mc_dt; \
    if(vz == 0) { ABSORB; }; \
    mc_dt = -z/vz; \
    if(mc_dt < 0 && allow_backprop == 0) { ABSORB; }; \
    mcPROP_DT(mc_dt); \
    z = 0; \
    DISALLOW_BACKPROP;\
  } while(0)

#define PROP_X0 \
  do { \
    if (mcgravitation) { Coords mcLocG; int mc_ret; \
    double mc_dt, mc_gx, mc_gy, mc_gz; \
    mcLocG = rot_apply(ROT_A_CURRENT_COMP, coords_set(0,-GRAVITY,0)); \
    coords_get(mcLocG, &mc_gx, &mc_gy, &mc_gz); \
    mc_ret = solve_2nd_order(&mc_dt, NULL, -mc_gx/2, -vx, -x); \
    if (mc_ret) {PROP_GRAV_DT(mc_dt, mc_gx, mc_gy, mc_gz); x=0;}\
    else if (allow_backprop == 0 && mc_dt < 0) { ABSORB; }; } \
    else mcPROP_X0; \
    DISALLOW_BACKPROP;\
  } while(0)

#define mcPROP_X0 \
  do { \
    double mc_dt; \
    if(vx == 0) { ABSORB; }; \
    mc_dt = -x/vx; \
    if(mc_dt < 0 && allow_backprop == 0) { ABSORB; }; \
    mcPROP_DT(mc_dt); \
    x = 0; \
    DISALLOW_BACKPROP;\
  } while(0)

#define PROP_Y0 \
  do { \
    if (mcgravitation) { Coords mcLocG; int mc_ret; \
    double mc_dt, mc_gx, mc_gy, mc_gz; \
    mcLocG = rot_apply(ROT_A_CURRENT_COMP, coords_set(0,-GRAVITY,0)); \
    coords_get(mcLocG, &mc_gx, &mc_gy, &mc_gz); \
    mc_ret = solve_2nd_order(&mc_dt, NULL, -mc_gy/2, -vy, -y); \
    if (mc_ret) {PROP_GRAV_DT(mc_dt, mc_gx, mc_gy, mc_gz); y=0;}\
    else if (allow_backprop == 0 && mc_dt < 0) { ABSORB; }; } \
    else mcPROP_Y0; \
    DISALLOW_BACKPROP;\
  } while(0)


#define mcPROP_Y0 \
  do { \
    double mc_dt; \
    if(vy == 0) { ABSORB; }; \
    mc_dt = -y/vy; \
    if(mc_dt < 0 && allow_backprop == 0) { ABSORB; }; \
    mcPROP_DT(mc_dt); \
    y = 0; \
    DISALLOW_BACKPROP; \
  } while(0)


#ifdef DEBUG

#define DEBUG_STATE() if(!mcdotrace); else \
  printf("STATE: %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g\n", \
         x,y,z,vx,vy,vz,t,sx,sy,sz,p);
#define DEBUG_SCATTER() if(!mcdotrace); else \
  printf("SCATTER: %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g\n", \
         x,y,z,vx,vy,vz,t,sx,sy,sz,p);

#else

#define DEBUG_STATE()
#define DEBUG_SCATTER()

#endif

#endif /* !MCCODE_H */

#endif /* MCSTAS_R_H */
/* End of file "mcstas-r.h". */

/* embedding file "mccode-r.c" */

/*******************************************************************************
*
* McCode, neutron/xray ray-tracing package
*         Copyright (C) 1997-2009, All rights reserved
*         Risoe National Laboratory, Roskilde, Denmark
*         Institut Laue Langevin, Grenoble, France
*
* Runtime: share/mccode-r.c
*
* %Identification
* Written by: KN
* Date:    Aug 29, 1997
* Release: McStas X.Y/McXtrace X.Y
* Version: $Revision$
*
* Runtime system for McStas and McXtrace.
* Embedded within instrument in runtime mode.
* Contains SECTIONS:
*   MPI handling (sum, send, recv)
*   format definitions
*   I/O
*   mcdisplay support
*   random numbers
*   coordinates handling
*   vectors math (solve 2nd order, normals, randvec...)
*   parameter handling
*   signal and main handlers
*
* Usage: Automatically embbeded in the c code whenever required.
*
* $Id$
*
*******************************************************************************/

/*******************************************************************************
* The I/O format definitions and functions
*******************************************************************************/


/** Include header files to avoid implicit declarations (not allowed on LLVM) */
#include <ctype.h>
#include <sys/types.h>
#ifndef _MSC_EXTENSIONS
#include <dirent.h>
#else
/* McCode includes its own 'dirent' for use with MSVC on Windows */
#include <windirent.h>
#define popen _popen
#define pclose _pclose
#endif
#include <errno.h>

// UNIX specific headers (non-Windows)
#if defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#include <sys/stat.h>
#endif


#ifndef DANSE
#ifdef MC_ANCIENT_COMPATIBILITY
int traceenabled = 0;
int defaultmain  = 0;
#endif
/* else defined directly in the McCode generated C code */

static   long mcseed                 = 0; /* seed for random generator */
#pragma acc declare create ( mcseed )
static   long mcstartdate            = 0; /* start simulation time */
static   int  mcdisable_output_files = 0; /* --no-output-files */
mcstatic int  mcgravitation          = 0; /* use gravitation flag, for PROP macros */
mcstatic int  mcusedefaults          = 0; /* assume default value for all parameters */
mcstatic int  mcappend               = 0; /* flag to allow append mode on datasets/directories */
mcstatic int  mcdotrace              = 0; /* flag for --trace and messages for DISPLAY */
mcstatic int  mcnexus_embed_idf      = 0; /* flag to embed xml-formatted IDF file for Mantid */
#pragma acc declare create ( mcdotrace )
int      mcallowbackprop             = 0;         /* flag to enable negative/backprop */

/* OpenACC-related segmentation parameters: */
int vecsize = 128;
int numgangs = 7813;
long gpu_innerloop = 2147483647;

/* Monitor_nD list/buffer-size default */
/* Starting value may be defined using -DND_BUFFER=N */
/* Can further be controlled dynamically using --bufsiz input */
long MONND_BUFSIZ = 10000000;
#ifdef ND_BUFFER
MONND_BUFSIZ = ND_BUFFER;
#endif
 

/* Number of particle histories to simulate. */
#ifdef NEUTRONICS
mcstatic unsigned long long int mcncount             = 1;
mcstatic unsigned long long int mcrun_num            = 0;
#else
#ifdef MCDEFAULT_NCOUNT
mcstatic unsigned long long int mcncount             = MCDEFAULT_NCOUNT;
#else
mcstatic unsigned long long int mcncount             = 1000000;
#endif
#pragma acc declare create ( mcncount )
mcstatic unsigned long long int mcrun_num            = 0;
#pragma acc declare create ( mcrun_num )
#endif /* NEUTRONICS */

#else
#include "mcstas-globals.h"
#endif /* !DANSE */

#ifndef NX_COMPRESSION
#define NX_COMPRESSION NX_COMP_NONE
#endif

/* String nullification on GPU and other replacements */
#ifdef OPENACC
int noprintf() {
  return 0;
}

int str_comp(char *str1, char *str2) {
  while (*str1 && *str1 == *str2) {
    str1++;
    str2++;
  }
  return (*str1 - *str2);
}

size_t str_len(const char *s)
{
  size_t len = 0;
  if(s != NULL)
  {
    while(*s != '\0')
    {
      ++len;
      ++s;
    }
  }
  return len;
}

#endif

/* SECTION: Predefine (component) parameters ================================= */

int nans_match(double a, double b){
  return (*(uint64_t*)&a == *(uint64_t*)&b);
}
int is_unset(double x){
  return nans_match(x, UNSET);
}
int is_set(double x){
  return !nans_match(x, UNSET);
}
int is_valid(double x){
  return !isnan(x)||is_unset(x);
}
int all_unset(int n, ...){
  va_list ptr;
  va_start(ptr, n);
  int ret=1;
  for (int i=0; i<n; ++i) if(is_set(va_arg(ptr, double))) ret=0;
  va_end(ptr);
  return ret;
}
int all_set(int n, ...){
  va_list ptr;
  va_start(ptr, n);
  int ret=1;
  for (int i=0; i<n; ++i) if(is_unset(va_arg(ptr, double))) ret=0;
  va_end(ptr);
  return ret;
}
int any_unset(int n, ...){
  va_list ptr;
  va_start(ptr, n);
  int ret=0;
  for (int i=0; i<n; ++i) if(is_unset(va_arg(ptr, double))) ret=1;
  va_end(ptr);
  return ret;
}
int any_set(int n, ...){
  va_list ptr;
  va_start(ptr, n);
  int ret=0;
  for (int i=0; i<n; ++i) if(is_set(va_arg(ptr, double))) ret=1;
  va_end(ptr);
  return ret;
}


/* SECTION: Dynamic Arrays ================================================== */
IArray1d create_iarr1d(int n){
  IArray1d arr1d;
  arr1d = calloc(n, sizeof(int));
  if (!arr1d) {
    fprintf(stderr, "Error allocating IArray1d of dimension %i\n",n);
    exit(-1);
  }
  return arr1d;
}

void destroy_iarr1d(IArray1d a){
  free(a);
}

IArray2d create_iarr2d(int nx, int ny){
  IArray2d arr2d;
  arr2d = calloc(nx, sizeof(int *));
  if (!arr2d) {
    fprintf(stderr, "Error allocating IArray2d of dimension %i x %i\n",nx,ny);
    exit(-1);
  }

  int *p1;
  p1 = calloc(nx*ny, sizeof(int));

  if (!p1) {
    fprintf(stderr, "Error allocating int* array of dimension %i\n",nx*ny);
    exit(-1);
  }
  
  int i;
  for (i=0; i<nx; i++){
    arr2d[i] = &(p1[i*ny]);
  }
  return arr2d;
}

void destroy_iarr2d(IArray2d a){
  free(a[0]);
  free(a);
}

IArray3d create_iarr3d(int nx, int ny, int nz){
  IArray3d arr3d;
  int i, j;

  // 1d
  arr3d = calloc(nx, sizeof(int **));
  if (!arr3d) {
    fprintf(stderr, "Error allocating IArray3d of dimension %i x %i x %i\n",nx,ny,nz);
    exit(-1);
  }

  // d2
  int **p1;
  p1 = calloc(nx*ny, sizeof(int *));

  if (!p1) {
    fprintf(stderr, "Error allocating int** array of dimension %i\n",nx*ny);
    exit(-1);
  }
  
  for (i=0; i<nx; i++){
    arr3d[i] = &(p1[i*ny]);
  }

  // 3d
  int *p2;
  p2 = calloc(nx*ny*nz, sizeof(int));
  if (!p2) {
    fprintf(stderr, "Error allocating int* array of dimension %i\n",nx*ny*nz);
    exit(-1);
  }
  for (i=0; i<nx; i++){
    for (j=0; j<ny; j++){
      arr3d[i][j] = &(p2[(i*ny+j)*nz]);
    }
  }
  return arr3d;
}

void destroy_iarr3d(IArray3d a){
  free(a[0][0]);
  free(a[0]);
  free(a);
}

DArray1d create_darr1d(int n){
  DArray1d arr1d;
  arr1d = calloc(n, sizeof(double));
  if (!arr1d) {
    fprintf(stderr, "Error allocating DArray1d of dimension %i\n",n);
    exit(-1);
  }
  return arr1d;
}

void destroy_darr1d(DArray1d a){
  free(a);
}

DArray2d create_darr2d(int nx, int ny){
  DArray2d arr2d;
  arr2d = calloc(nx, sizeof(double *));
  if (!arr2d) {
    fprintf(stderr, "Error allocating DArray2d of dimension %i x %i\n",nx,ny);
    exit(-1);
  }
  double *p1;
  p1 = calloc(nx*ny, sizeof(double));
  if (!p1) {
    fprintf(stderr, "Error allocating double* array of dimension %i\n",nx*ny);
    exit(-1);
  }
  int i;
  for (i=0; i<nx; i++){
    arr2d[i] = &(p1[i*ny]);
  }
  return arr2d;
}

void destroy_darr2d(DArray2d a){
  free(a[0]);
  free(a);
}

DArray3d create_darr3d(int nx, int ny, int nz){
  DArray3d arr3d;

  int i, j;

  // 1d
  arr3d = calloc(nx, sizeof(double **));
  if (!arr3d) {
    fprintf(stderr, "Error allocating DArray3d of dimension %i x %i x %i\n",nx,ny,nz);
    exit(-1);
  }
  // d2
  double **p1;
  p1 = calloc(nx*ny, sizeof(double *));
  if (!p1) {
    fprintf(stderr, "Error allocating double** array of dimension %i\n",nx*ny);
    exit(-1);
  }
  for (i=0; i<nx; i++){
    arr3d[i] = &(p1[i*ny]);
  }

  // 3d
  double *p2;
  p2 = calloc(nx*ny*nz, sizeof(double));
  if (!p2) {
    fprintf(stderr, "Error allocating double* array of dimension %i\n",nx*ny*nz);
    exit(-1);
  }
  for (i=0; i<nx; i++){
    for (j=0; j<ny; j++){
      arr3d[i][j] = &(p2[(i*ny+j)*nz]);
    }
  }
  return arr3d;
}

void destroy_darr3d(DArray3d a){
  free(a[0][0]);
  free(a[0]);
  free(a);
}


/* SECTION: MPI handling ==================================================== */

#ifdef USE_MPI
/* MPI rank */
static int mpi_node_rank;
static int mpi_node_root = 0;


/*******************************************************************************
* mc_MPI_Reduce: Gathers arrays from MPI nodes using Reduce function.
*******************************************************************************/
int mc_MPI_Sum(double *sbuf, long count)
{
  if (!sbuf || count <= 0) return(MPI_SUCCESS); /* nothing to reduce */
  else {
    /* we must cut the buffer into blocks not exceeding the MPI max buffer size of 32000 */
    long   offset=0;
    double *rbuf=NULL;
    int    length=MPI_REDUCE_BLOCKSIZE; /* defined in mccode-r.h */
    int    i=0;
    rbuf = calloc(count, sizeof(double));
    if (!rbuf)
      exit(-fprintf(stderr, "Error: Out of memory %zi (mc_MPI_Sum)\n", count*sizeof(double)));
    while (offset < count) {
      if (!length || offset+length > count-1) length=count-offset;
      else length=MPI_REDUCE_BLOCKSIZE;
      if (MPI_Allreduce((double*)(sbuf+offset), (double*)(rbuf+offset),
              length, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD) != MPI_SUCCESS)
        return MPI_ERR_COUNT;
      offset += length;
    }

    for (i=0; i<count; i++) sbuf[i] = rbuf[i];
    free(rbuf);
  }
  return MPI_SUCCESS;
} /* mc_MPI_Sum */

/*******************************************************************************
* mc_MPI_Send: Send array to MPI node by blocks to avoid buffer limit
*******************************************************************************/
int mc_MPI_Send(void *sbuf,
                  long count, MPI_Datatype dtype,
                  int dest)
{
  int dsize;
  long offset=0;
  int  tag=1;
  int  length=MPI_REDUCE_BLOCKSIZE; /* defined in mccode-r.h */

  if (!sbuf || count <= 0) return(MPI_SUCCESS); /* nothing to send */
  MPI_Type_size(dtype, &dsize);

  while (offset < count) {
    if (offset+length > count-1) length=count-offset;
    else length=MPI_REDUCE_BLOCKSIZE;
    if (MPI_Send((void*)((char*)sbuf+offset*dsize), length, dtype, dest, tag++, MPI_COMM_WORLD) != MPI_SUCCESS)
      return MPI_ERR_COUNT;
    offset += length;
  }

  return MPI_SUCCESS;
} /* mc_MPI_Send */

/*******************************************************************************
* mc_MPI_Recv: Receives arrays from MPI nodes by blocks to avoid buffer limit
*             the buffer must have been allocated previously.
*******************************************************************************/
int mc_MPI_Recv(void *sbuf,
                  long count, MPI_Datatype dtype,
                  int source)
{
  int dsize;
  long offset=0;
  int  tag=1;
  int  length=MPI_REDUCE_BLOCKSIZE; /* defined in mccode-r.h */

  if (!sbuf || count <= 0) return(MPI_SUCCESS); /* nothing to recv */
  MPI_Type_size(dtype, &dsize);

  while (offset < count) {
    if (offset+length > count-1) length=count-offset;
    else length=MPI_REDUCE_BLOCKSIZE;
    if (MPI_Recv((void*)((char*)sbuf+offset*dsize), length, dtype, source, tag++,
            MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS)
      return MPI_ERR_COUNT;
    offset += length;
  }

  return MPI_SUCCESS;
} /* mc_MPI_Recv */

#endif /* USE_MPI */

/* SECTION: parameters handling ============================================= */

/* Instrument input parameter type handling. */
/*******************************************************************************
* mcparm_double: extract double value from 's' into 'vptr'
*******************************************************************************/
static int
mcparm_double(char *s, void *vptr)
{
  char *p;
  double *v = (double *)vptr;

  if (!s) { *v = 0; return(1); }
  *v = strtod(s, &p);
  if(*s == '\0' || (p != NULL && *p != '\0') || errno == ERANGE)
    return 0;                        /* Failed */
  else
    return 1;                        /* Success */
}

/*******************************************************************************
* mcparminfo_double: display parameter type double
*******************************************************************************/
static char *
mcparminfo_double(char *parmname)
{
  return "double";
}

/*******************************************************************************
* mcparmerror_double: display error message when failed extract double
*******************************************************************************/
static void
mcparmerror_double(char *parm, char *val)
{
  fprintf(stderr, "Error: Invalid value '%s' for floating point parameter %s (mcparmerror_double)\n",
          val, parm);
}

/*******************************************************************************
* mcparmprinter_double: convert double to string
*******************************************************************************/
static void
mcparmprinter_double(char *f, void *vptr)
{
  double *v = (double *)vptr;
  sprintf(f, "%g", *v);
}

/*******************************************************************************
* mcparm_int: extract int value from 's' into 'vptr'
*******************************************************************************/
static int
mcparm_int(char *s, void *vptr)
{
  char *p;
  int *v = (int *)vptr;
  long x;

  if (!s) { *v = 0; return(1); }
  *v = 0;
  x = strtol(s, &p, 10);
  if(x < INT_MIN || x > INT_MAX)
    return 0;                        /* Under/overflow */
  *v = x;
  if(*s == '\0' || (p != NULL && *p != '\0') || errno == ERANGE)
    return 0;                        /* Failed */
  else
    return 1;                        /* Success */
}

/*******************************************************************************
* mcparminfo_int: display parameter type int
*******************************************************************************/
static char *
mcparminfo_int(char *parmname)
{
  return "int";
}

/*******************************************************************************
* mcparmerror_int: display error message when failed extract int
*******************************************************************************/
static void
mcparmerror_int(char *parm, char *val)
{
  fprintf(stderr, "Error: Invalid value '%s' for integer parameter %s (mcparmerror_int)\n",
          val, parm);
}

/*******************************************************************************
* mcparmprinter_int: convert int to string
*******************************************************************************/
static void
mcparmprinter_int(char *f, void *vptr)
{
  int *v = (int *)vptr;
  sprintf(f, "%d", *v);
}

/*******************************************************************************
* mcparm_string: extract char* value from 's' into 'vptr' (copy)
*******************************************************************************/
static int
mcparm_string(char *s, void *vptr)
{
  char **v = (char **)vptr;
  if (!s) { *v = NULL; return(1); }
  *v = (char *)malloc(strlen(s) + 1);
  if(*v == NULL)
  {
    exit(-fprintf(stderr, "Error: Out of memory %li (mcparm_string).\n", (long)strlen(s) + 1));
  }
  strcpy(*v, s);
  return 1;                        /* Success */
}

/*******************************************************************************
* mcparminfo_string: display parameter type string
*******************************************************************************/
static char *
mcparminfo_string(char *parmname)
{
  return "string";
}

/*******************************************************************************
* mcparmerror_string: display error message when failed extract string
*******************************************************************************/
static void
mcparmerror_string(char *parm, char *val)
{
  fprintf(stderr, "Error: Invalid value '%s' for string parameter %s (mcparmerror_string)\n",
          val, parm);
}

/*******************************************************************************
* mcparmprinter_string: convert string to string (including esc chars)
*******************************************************************************/
static void
mcparmprinter_string(char *f, void *vptr)
{
  char **v = (char **)vptr;
  char *p;

  if (!*v) { *f='\0'; return; }
  strcpy(f, "");
  for(p = *v; *p != '\0'; p++)
  {
    switch(*p)
    {
      case '\n':
        strcat(f, "\\n");
        break;
      case '\r':
        strcat(f, "\\r");
        break;
      case '"':
        strcat(f, "\\\"");
        break;
      case '\\':
        strcat(f, "\\\\");
        break;
      default:
        strncat(f, p, 1);
    }
  }
  /* strcat(f, "\""); */
} /* mcparmprinter_string */

/* now we may define the parameter structure, using previous functions */
static struct
  {
    int (*getparm)(char *, void *);
    char * (*parminfo)(char *);
    void (*error)(char *, char *);
    void (*printer)(char *, void *);
} mcinputtypes[] = {
  {
    mcparm_int, mcparminfo_int, mcparmerror_int,
    mcparmprinter_int
  }, {
    mcparm_string, mcparminfo_string, mcparmerror_string,
    mcparmprinter_string
  }, {
    mcparm_string, mcparminfo_string, mcparmerror_string,
    mcparmprinter_string
  }, {
    mcparm_double, mcparminfo_double, mcparmerror_double,
    mcparmprinter_double
  }, {
    mcparm_double, mcparminfo_double, mcparmerror_double,
    mcparmprinter_double
  }
};

/*******************************************************************************
* mcestimate_error: compute sigma from N,p,p2 in Gaussian large numbers approx
*******************************************************************************/
double mcestimate_error(double N, double p1, double p2)
{
  double pmean, n1;
  if(N <= 1)
    return p1;
  pmean = p1 / N;
  n1 = N - 1;
  /* Note: underflow may cause p2 to become zero; the fabs() below guards
     against this. */
  return sqrt((N/n1)*fabs(p2 - pmean*pmean));
}

double (*mcestimate_error_p)
  (double V2, double psum, double p2sum)=mcestimate_error;

/* ========================================================================== */

/*                               MCCODE_R_IO_C                                */

/* ========================================================================== */

#ifndef MCCODE_R_IO_C
#define MCCODE_R_IO_C "$Revision$"

/* SECTION: file i/o handling ================================================ */

#ifndef HAVE_STRCASESTR
// from msysgit: https://code.google.com/p/msysgit/source/browse/compat/strcasestr.c
char *strcasestr(const char *haystack, const char *needle)
{
  int nlen = strlen(needle);
  int hlen = strlen(haystack) - nlen + 1;
  int i;

  for (i = 0; i < hlen; i++) {
    int j;
    for (j = 0; j < nlen; j++) {
            unsigned char c1 = haystack[i+j];
            unsigned char c2 = needle[j];
            if (toupper(c1) != toupper(c2))
                    goto next;
    }
    return (char *) haystack + i;
  next:
    ;
  }
  return NULL;
}


#endif
#ifndef HAVE_STRCASECMP
int strcasecmp( const char *s1, const char *s2 )
{
  int c1, c2;
  do {
    c1 = tolower( (unsigned char) *s1++ );
    c2 = tolower( (unsigned char) *s2++ );
  } while (c1 == c2 && c1 != 0);
  return c2 > c1 ? -1 : c1 > c2;
}
#endif

#ifndef STRACPY
/* this is a replacement to strncpy, but ensures that the copy ends with NULL */
/* http://stracpy.blogspot.fr/2011/04/stracpy-strncpy-replacement.html */
#define STRACPY
char *stracpy(char *destination, const char *source, size_t amount)
{
        if (!destination || !source || !amount) return(NULL);
        while(amount--)
          if((*destination++ = *source++) == '\0') break;
        *destination = '\0';
        return destination;
}
#endif

/*******************************************************************************
* mcfull_file: allocates a full file name=dirname+file. Catenate extension if missing.
*******************************************************************************/
char *mcfull_file(char *name, char *ext)
{
  int   dirlen=0;
  char *mem   =NULL;

  dirlen = dirname ? strlen(dirname) : 0;
  mem = (char*)malloc(dirlen + strlen(name) + CHAR_BUF_LENGTH);
  if(!mem) {
    exit(-fprintf(stderr, "Error: Out of memory %li (mcfull_file)\n", (long)(dirlen + strlen(name) + 256)));
  }
  strcpy(mem, "");

  /* prepend directory name to path if name does not contain a path */
  if (dirlen > 0 && !strchr(name, MC_PATHSEP_C)) {
    strcat(mem, dirname);
    strcat(mem, MC_PATHSEP_S);
  } /* dirlen */

  strcat(mem, name);
  if (!strchr(name, '.') && ext && strlen(ext))
  { /* add extension if not in file name already */
    strcat(mem, ".");
    strcat(mem, ext);
  }
  return(mem);
} /* mcfull_file */

/*******************************************************************************
* mcnew_file: opens a new file within dirname if non NULL
*             the file is opened in "a" (append, create if does not exist)
*             the extension 'ext' is added if the file name does not include one.
*             the last argument is set to 0 if file did not exist, else to 1.
*******************************************************************************/
FILE *mcnew_file(char *name, char *ext, int *exists)
{
  char *mem;
  FILE *file=NULL;

  if (!name || strlen(name) == 0 || mcdisable_output_files) return(NULL);

  mem  = mcfull_file(name, ext); /* create dirname/name.ext */

  /* check for existence */
  file = fopen(mem, "r"); /* for reading -> fails if does not exist */
  if (file) {
    fclose(file);
    *exists=1;
  } else
    *exists=0;

  /* open the file for writing/appending */
#ifdef USE_NEXUS
  if (mcformat && strcasestr(mcformat, "NeXus")) {
    /* NXhandle nxhandle is defined in the .h with USE_NEXUS */
    NXaccess mode = (*exists ? NXACC_CREATE5 | NXACC_RDWR : NXACC_CREATE5);

    if (NXopen(mem, mode, &nxhandle) != NX_OK)
      file = NULL;
    else
      file = (FILE*)&nxhandle; /* to make it non NULL */
  } else
#endif
    file = fopen(mem, "a+");

  if(!file)
    fprintf(stderr, "Warning: could not open output file '%s' for %s (mcnew_file)\n",
      mem, *exists ? "append" : "create");
  free(mem);

  return file;
} /* mcnew_file */

/*******************************************************************************
* mcdetector_statistics: compute detector statistics, error bars, [x I I_err N] 1D
* RETURN:            updated detector structure
* Used by: detector_import
*******************************************************************************/
MCDETECTOR mcdetector_statistics(
  MCDETECTOR detector)
{

  if (!detector.p1 || !detector.m)
    return(detector);

  /* compute statistics and update MCDETECTOR structure ===================== */
  double sum_z  = 0, min_z  = 0, max_z  = 0;
  double fmon_x =0,  smon_x = 0, fmon_y =0, smon_y=0, mean_z=0;
  double Nsum=0, P2sum=0;

  double sum_xz = 0, sum_yz = 0, sum_x = 0, sum_y = 0, sum_x2z = 0, sum_y2z = 0;
  int    i,j;
  char   hasnan=0, hasinf=0;
  char   israw = ((char*)strcasestr(detector.format,"raw") != NULL);
  double *this_p1=NULL; /* new 1D McCode array [x I E N]. Freed after writing data */

  /* if McCode/PGPLOT and rank==1 we create a new m*4 data block=[x I E N] */
  if (detector.rank == 1 && strcasestr(detector.format,"McCode")) {
    this_p1 = (double *)calloc(detector.m*detector.n*detector.p*4, sizeof(double));
    if (!this_p1)
      exit(-fprintf(stderr, "Error: Out of memory creating %zi 1D " MCCODE_STRING " data set for file '%s' (detector_import)\n",
        detector.m*detector.n*detector.p*4*sizeof(double*), detector.filename));
  }

  max_z = min_z = detector.p1[0];

  /* compute sum and moments (not for lists) */
  if (!strcasestr(detector.format,"list") && detector.m)
  for(j = 0; j < detector.n*detector.p; j++)
  {
    for(i = 0; i < detector.m; i++)
    {
      double x,y,z;
      double N, E;
      long   index= !detector.istransposed ? i*detector.n*detector.p + j : i+j*detector.m;
      char   hasnaninf=0;

      if (detector.m)
        x = detector.xmin + (i + 0.5)/detector.m*(detector.xmax - detector.xmin);
      else x = 0;
      if (detector.n && detector.p)
        y = detector.ymin + (j + 0.5)/detector.n/detector.p*(detector.ymax - detector.ymin);
      else y = 0;
      z = detector.p1[index];
      N = detector.p0 ? detector.p0[index] : 1;
      E = detector.p2 ? detector.p2[index] : 0;
      if (detector.p2 && !israw)
        detector.p2[index] = (*mcestimate_error_p)(detector.p0[index],detector.p1[index],detector.p2[index]); /* set sigma */

      if (detector.rank == 1 && this_p1 && strcasestr(detector.format,"McCode")) {
        /* fill-in 1D McCode array [x I E N] */
        this_p1[index*4]   = x;
        this_p1[index*4+1] = z;
        this_p1[index*4+2] = detector.p2 ? detector.p2[index] : 0;
        this_p1[index*4+3] = N;
      }

      if (isnan(z) || isnan(E) || isnan(N)) hasnaninf=hasnan=1;
      if (isinf(z) || isinf(E) || isinf(N)) hasnaninf=hasinf=1;

      /* compute stats integrals */
      if (!hasnaninf) {
        sum_xz += x*z;
        sum_yz += y*z;
        sum_x  += x;
        sum_y  += y;
        sum_z  += z;
        sum_x2z += x*x*z;
        sum_y2z += y*y*z;
        if (z > max_z) max_z = z;
        if (z < min_z) min_z = z;

        Nsum += N;
        P2sum += E;
      }

    }
  } /* for j */

  /* compute 1st and 2nd moments. For lists, sum_z=0 so this is skipped. */
  if (sum_z && detector.n*detector.m*detector.p)
  {
    fmon_x = sum_xz/sum_z;
    fmon_y = sum_yz/sum_z;
    smon_x = sum_x2z/sum_z-fmon_x*fmon_x; smon_x = smon_x > 0 ? sqrt(smon_x) : 0;
    smon_y = sum_y2z/sum_z-fmon_y*fmon_y; smon_y = smon_y > 0 ? sqrt(smon_y) : 0;
    mean_z = sum_z/detector.n/detector.m/detector.p;
  }
  /* store statistics into detector */
  detector.intensity = sum_z;
  detector.error     = Nsum ? (*mcestimate_error_p)(Nsum, sum_z, P2sum) : 0;
  detector.events    = Nsum;
  detector.min       = min_z;
  detector.max       = max_z;
  detector.mean      = mean_z;
  detector.centerX   = fmon_x;
  detector.halfwidthX= smon_x;
  detector.centerY   = fmon_y;
  detector.halfwidthY= smon_y;

  /* if McCode/PGPLOT and rank==1 replace p1 with new m*4 1D McCode and clear others */
  if (detector.rank == 1 && this_p1 && strcasestr(detector.format,"McCode")) {

    detector.p1 = this_p1;
    detector.n  = detector.m; detector.m  = 4;
    detector.p0 = detector.p2 = NULL;
    detector.istransposed = 1;
  }

  if (detector.n*detector.m*detector.p > 1)
    snprintf(detector.signal, CHAR_BUF_LENGTH,
      "Min=%g; Max=%g; Mean=%g;", detector.min, detector.max, detector.mean);
  else
    strcpy(detector.signal, "None");
  snprintf(detector.values, CHAR_BUF_LENGTH,
    "%g %g %g", detector.intensity, detector.error, detector.events);

  switch (detector.rank) {
    case 1:  snprintf(detector.statistics, CHAR_BUF_LENGTH, "X0=%g; dX=%g;",
      detector.centerX, detector.halfwidthX); break;
    case 2:
    case 3:  snprintf(detector.statistics, CHAR_BUF_LENGTH, "X0=%g; dX=%g; Y0=%g; dY=%g;",
      detector.centerX, detector.halfwidthX, detector.centerY, detector.halfwidthY);
      break;
    default: strcpy(detector.statistics, "None");
  }

  if (hasnan)
    printf("WARNING: Nan detected in component/file %s %s\n",
      detector.component, strlen(detector.filename) ? detector.filename : "");
  if (hasinf)
    printf("WARNING: Inf detected in component/file %s %s\n",
      detector.component, strlen(detector.filename) ? detector.filename : "");

  return(detector);

} /* mcdetector_statistics */

/*******************************************************************************
* detector_import: build detector structure, merge non-lists from MPI
*                    compute basic stat, write "Detector:" line
* RETURN:            detector structure. Invalid data if detector.p1 == NULL
*                    Invalid detector sets m=0 and filename=""
*                    Simulation data  sets m=0 and filename=siminfo_name
* This function is equivalent to the old 'mcdetector_out', returning a structure
*******************************************************************************/
MCDETECTOR detector_import(
  char *format,
  char *component, char *title,
  long m, long n,  long p,
  char *xlabel, char *ylabel, char *zlabel,
  char *xvar, char *yvar, char *zvar,
  double x1, double x2, double y1, double y2, double z1, double z2,
  char *filename,
  double *p0, double *p1, double *p2,
  Coords position, Rotation rotation, int index)
{
  time_t t;       /* for detector.date */
  long   date_l;  /* date as a long number */
  char   istransposed=0;
  char   c[CHAR_BUF_LENGTH]; /* temp var for signal label */

  MCDETECTOR detector;

  /* build MCDETECTOR structure ============================================= */
  /* make sure we do not have NULL for char fields */

  /* these also apply to simfile */
  strncpy (detector.filename,  filename ? filename : "",        CHAR_BUF_LENGTH);
  strncpy (detector.format,    format   ? format   : "McCode" , CHAR_BUF_LENGTH);
  /* add extension if missing */
  if (strlen(detector.filename) && !strchr(detector.filename, '.'))
  { /* add extension if not in file name already */
    strcat(detector.filename, ".dat");
  }
  strncpy (detector.component, component ? component : MCCODE_STRING " component", CHAR_BUF_LENGTH);
  #ifdef USE_NEXUS
  char pref[5];
  if (index-1 < 10) {
    sprintf(pref,"000");
  } else if (index-1 < 100) {
    sprintf(pref,"00");
  } else if (index-1 < 1000) {
    sprintf(pref,"0");
  } else if (index-1 < 10000) {
    sprintf(pref,"");
  } else {
    fprintf(stderr,"Error, no support for > 10000 comps at the moment!\n");
    exit(-1);
  }
  sprintf(detector.nexuscomp,"%s%d_%s",pref,index-1,detector.component);
  #endif

  snprintf(detector.instrument, CHAR_BUF_LENGTH, "%s (%s)", instrument_name, instrument_source);
  snprintf(detector.user, CHAR_BUF_LENGTH,      "%s on %s",
        getenv("USER") ? getenv("USER") : MCCODE_NAME,
        getenv("HOST") ? getenv("HOST") : "localhost");
  time(&t);         /* get current write time */
  date_l = (long)t; /* same but as a long */
  snprintf(detector.date, CHAR_BUF_LENGTH, "%s", ctime(&t));
  if (strlen(detector.date))   detector.date[strlen(detector.date)-1] = '\0'; /* remove last \n in date */
  detector.date_l = date_l;

  if (!mcget_run_num() || mcget_run_num() >= mcget_ncount())
    snprintf(detector.ncount, CHAR_BUF_LENGTH, "%llu", mcget_ncount()
#ifdef USE_MPI
*mpi_node_count
#endif
  );
  else
    snprintf(detector.ncount, CHAR_BUF_LENGTH, "%g/%g", (double)mcget_run_num(), (double)mcget_ncount());

  detector.p0         = p0;
  detector.p1         = p1;
  detector.p2         = p2;

  /* handle transposition (not for NeXus) */
  if (!strcasestr(detector.format, "NeXus")) {
    if (m<0 || n<0 || p<0)             istransposed = !istransposed;
    if (strcasestr(detector.format, "transpose")) istransposed = !istransposed;
    if (istransposed) { /* do the swap once for all */
      long i=m; m=n; n=i;
    }
  }

  m=labs(m); n=labs(n); p=labs(p); /* make sure dimensions are positive */
  detector.istransposed = istransposed;

  /* determine detector rank (dimensionality) */
  if (!m || !n || !p || !p1) detector.rank = 4; /* invalid: exit with m=0 filename="" */
  else if (m*n*p == 1)       detector.rank = 0; /* 0D */
  else if (n == 1 || m == 1) detector.rank = 1; /* 1D */
  else if (p == 1)           detector.rank = 2; /* 2D */
  else                       detector.rank = 3; /* 3D */

  /* from rank, set type */
  switch (detector.rank) {
    case 0:  strcpy(detector.type,  "array_0d"); m=n=p=1; break;
    case 1:  snprintf(detector.type, CHAR_BUF_LENGTH, "array_1d(%ld)", m*n*p); m *= n*p; n=p=1; break;
    case 2:  if(!strcasestr(detector.format,"list")) {
               snprintf(detector.type, CHAR_BUF_LENGTH, "array_2d(%ld, %ld)", m, n*p); n *= p; p=1;
             } else {
               snprintf(detector.type, CHAR_BUF_LENGTH, "list(%ld, %ld)", m, n*p); n *= p; p=1;
             } break;
    case 3:  snprintf(detector.type, CHAR_BUF_LENGTH, "array_3d(%ld, %ld, %ld)", m, n, p); break;
    default: m=0; strcpy(detector.type, ""); strcpy(detector.filename, "");/* invalid */
  }

  detector.m    = m;
  detector.n    = n;
  detector.p    = p;

  /* these only apply to detector files ===================================== */

  detector.Position[0]=position.x;
  detector.Position[1]=position.y;
  detector.Position[2]=position.z;
  rot_copy(detector.Rotation,rotation);
  snprintf(detector.position, CHAR_BUF_LENGTH, "%g %g %g", position.x, position.y, position.z);
  /* may also store actual detector orientation in the future */

  strncpy(detector.title,      title && strlen(title) ? title : component,       CHAR_BUF_LENGTH);
  strncpy(detector.xlabel,     xlabel && strlen(xlabel) ? xlabel : "X", CHAR_BUF_LENGTH); /* axis labels */
  strncpy(detector.ylabel,     ylabel && strlen(ylabel) ? ylabel : "Y", CHAR_BUF_LENGTH);
  strncpy(detector.zlabel,     zlabel && strlen(zlabel) ? zlabel : "Z", CHAR_BUF_LENGTH);
  strncpy(detector.xvar,       xvar && strlen(xvar) ? xvar :       "x", CHAR_BUF_LENGTH); /* axis variables */
  strncpy(detector.yvar,       yvar && strlen(yvar) ? yvar :       detector.xvar, CHAR_BUF_LENGTH);
  strncpy(detector.zvar,       zvar && strlen(zvar) ? zvar :       detector.yvar, CHAR_BUF_LENGTH);

  /* set "variables" as e.g. "I I_err N" */
  strcpy(c, "I ");
  if (strlen(detector.zvar))      strncpy(c, detector.zvar,32);
  else if (strlen(detector.yvar)) strncpy(c, detector.yvar,32);
  else if (strlen(detector.xvar)) strncpy(c, detector.xvar,32);

  if (detector.rank == 1)
    snprintf(detector.variables, CHAR_BUF_LENGTH, "%s %s %s_err N", detector.xvar, c, c);
  else
    snprintf(detector.variables, CHAR_BUF_LENGTH, "%s %s_err N", c, c);

  /* limits */
  detector.xmin = x1;
  detector.xmax = x2;
  detector.ymin = y1;
  detector.ymax = y2;
  detector.zmin = z1;
  detector.zmax = z2;
  if (abs(detector.rank) == 1)
    snprintf(detector.limits, CHAR_BUF_LENGTH, "%g %g", x1, x2);
  else if (detector.rank == 2)
    snprintf(detector.limits, CHAR_BUF_LENGTH, "%g %g %g %g", x1, x2, y1, y2);
  else
    snprintf(detector.limits, CHAR_BUF_LENGTH, "%g %g %g %g %g %g", x1, x2, y1, y2, z1, z2);

  /* if MPI and nodes_nb > 1: reduce data sets when using MPI =============== */
#ifdef USE_MPI
  if (!strcasestr(detector.format,"list") && mpi_node_count > 1 && m) {
    /* we save additive data: reduce everything into mpi_node_root */
    if (p0) mc_MPI_Sum(p0, m*n*p);
    if (p1) mc_MPI_Sum(p1, m*n*p);
    if (p2) mc_MPI_Sum(p2, m*n*p);
    if (!p0) {  /* additive signal must be then divided by the number of nodes */
      int i;
      for (i=0; i<m*n*p; i++) {
        p1[i] /= mpi_node_count;
        if (p2) p2[i] /= mpi_node_count;
      }
    }
  }
#endif /* USE_MPI */

  /* compute statistics, Nsum, intensity, Error bars */
  detector = mcdetector_statistics(detector);

#ifdef USE_MPI
  /* slaves are done */
  if(mpi_node_rank != mpi_node_root) {
    return detector;
  }
#endif

  /* output "Detector:" line ================================================ */
  /* when this is a detector written by a component (not the SAVE from instrument),
     not an event lists */
  if (!m) return(detector);
  if (!strcasestr(detector.format,"list")) {
    if (!strcmp(detector.component, instrument_name)) {
      if (strlen(detector.filename))  /* we name it from its filename, or from its title */
        strncpy(c, detector.filename, CHAR_BUF_LENGTH);
      else
        snprintf(c, CHAR_BUF_LENGTH, "%s", instrument_name);
    } else
      strncpy(c, detector.component, CHAR_BUF_LENGTH);  /* usual detectors written by components */

    printf("Detector: %s_I=%g %s_ERR=%g %s_N=%g",
           c, detector.intensity,
           c, detector.error,
           c, detector.events);
    printf(" \"%s\"\n", strlen(detector.filename) ? detector.filename : detector.component);
  }


  return(detector);
} /* detector_import */

/* end MCDETECTOR import section ============================================ */

















/* ========================================================================== */

/*                               ASCII output                                 */
/*     The SIM file is YAML based, the data files have '#' headers            */

/* ========================================================================== */


/*******************************************************************************
* mcinfo_out: output instrument tags/info (only in SIM)
* Used in: siminfo_init (ascii), mcinfo(stdout)
*******************************************************************************/
static void mcinfo_out(char *pre, FILE *f)
{
  char Parameters[CHAR_BUF_LENGTH] = "";
  int  i;

  if (!f || mcdisable_output_files) return;

  /* create parameter string ================================================ */
  for(i = 0; i < numipar; i++)
  {
    char ThisParam[CHAR_BUF_LENGTH];
    if (strlen(mcinputtable[i].name) > CHAR_BUF_LENGTH) break;
    snprintf(ThisParam, CHAR_BUF_LENGTH, " %s(%s)", mcinputtable[i].name,
            (*mcinputtypes[mcinputtable[i].type].parminfo)
                (mcinputtable[i].name));
    if (strlen(Parameters) + strlen(ThisParam) + 1 >= CHAR_BUF_LENGTH) break;
    strcat(Parameters, ThisParam);
  }

  /* output data ============================================================ */
  if (f != stdout)
    fprintf(f, "%sFile: %s%c%s\n",    pre, dirname, MC_PATHSEP_C, siminfo_name);
  else
    fprintf(f, "%sCreator: %s\n",     pre, MCCODE_STRING);

  fprintf(f, "%sSource: %s\n",   pre, instrument_source);
  fprintf(f, "%sParameters: %s\n",    pre, Parameters);

  fprintf(f, "%sTrace_enabled: %s\n", pre, traceenabled ? "yes" : "no");
  fprintf(f, "%sDefault_main: %s\n",  pre, defaultmain ?  "yes" : "no");
#ifdef MC_EMBEDDED_RUNTIME
  fprintf(f, "%sEmbedded_runtime: %s\n", pre, "yes");
#else
  fprintf(f, "%sEmbedded_runtime: %s\n", pre, "no");
#endif

  fflush(f);
} /* mcinfo_out */

/*******************************************************************************
* mcruninfo_out: output simulation tags/info (both in SIM and data files)
* Used in: siminfo_init (ascii case), mcdetector_out_xD_ascii
*******************************************************************************/
static void mcruninfo_out(char *pre, FILE *f)
{
  int i;
  char Parameters[CHAR_BUF_LENGTH];

  if (!f || mcdisable_output_files) return;

  fprintf(f, "%sFormat: %s%s\n",      pre,
    mcformat && strlen(mcformat) ? mcformat : MCCODE_NAME,
    mcformat && strcasestr(mcformat,"McCode") ? " with text headers" : "");
  fprintf(f, "%sURL: %s\n",         pre, "http://www.mccode.org");
  fprintf(f, "%sCreator: %s\n",     pre, MCCODE_STRING);
  fprintf(f, "%sInstrument: %s\n", pre, instrument_source);
  fprintf(f, "%sNcount: %llu\n",        pre, mcget_ncount());
  fprintf(f, "%sTrace: %s\n",       pre, mcdotrace ? "yes" : "no");
  fprintf(f, "%sGravitation: %s\n", pre, mcgravitation ? "yes" : "no");
  snprintf(Parameters, CHAR_BUF_LENGTH, "%ld", mcseed);
  fprintf(f, "%sSeed: %s\n",        pre, Parameters);
  fprintf(f, "%sDirectory: %s\n",        pre, dirname ? dirname : ".");
#ifdef USE_MPI
  if (mpi_node_count > 1)
    fprintf(f, "%sNodes: %i\n",        pre, mpi_node_count);
#endif

  // TODO Consider replacing this by a a call to `mcparameterinfo_out(pre+"Param: ", f)`
  /* output parameter string ================================================ */
  for(i = 0; i < numipar; i++) {
      if (mcinputtable[i].par){
	/* Parameters with a default value */
	if(mcinputtable[i].val && strlen(mcinputtable[i].val)){
	  (*mcinputtypes[mcinputtable[i].type].printer)(Parameters, mcinputtable[i].par);
	  fprintf(f, "%sParam: %s=%s\n", pre, mcinputtable[i].name, Parameters);
        /* ... and those without */
	}else{
	  fprintf(f, "%sParam: %s=NULL\n", pre, mcinputtable[i].name);
	}
      }
  }
  fflush(f);
} /* mcruninfo_out */

/*******************************************************************************
 * @brief Print parameter information to the specified file
 * @param pre any beginning-of-line padding
 * @param f the output file
 */
static void mcparameterinfo_out(char * pre, FILE *f){
  if (!f || mcdisable_output_files) return;

  unsigned int nchar = 4;
  for (int i=0; i < numipar; ++i){
    if (mcinputtable[i].par && mcinputtable[i].val && strlen(mcinputtable[i].val) > nchar)
      nchar = strlen(mcinputtable[i].val);
  }
  char * buffer = calloc(nchar+1, sizeof(char));

  if (!buffer) {
    exit(1);
  }

  for (int i=0; i < numipar; ++i) {
    if (mcinputtable[i].par) {
      char * name = mcinputtable[i].name;
      if (mcinputtable[i].val && strlen(mcinputtable[i].val)) {
        mcinputtypes[mcinputtable[i].type].printer(buffer, mcinputtable[i].par);
      } else {
        strcpy(buffer, "NULL");
      }
      if (strlen(mcinputtable[i].unit)){
        //fprintf(f, "%s%s %s (\"%s\") = %s\n", pre, mcinputtypes[mcinputtable[i].type].parminfo(name), name, mcinputtable[i].unit, buffer);
        fprintf(f, "%s%s %s/\"%s\" = %s\n", pre, mcinputtypes[mcinputtable[i].type].parminfo(name), name, mcinputtable[i].unit, buffer);
      } else {
        fprintf(f, "%s%s %s = %s\n", pre, mcinputtypes[mcinputtable[i].type].parminfo(name), name, buffer);
      }
    }
  }

  free(buffer);
}

/*******************************************************************************
* siminfo_out:    wrapper to fprintf(siminfo_file)
*******************************************************************************/
void siminfo_out(char *format, ...)
{
  va_list ap;

  if(siminfo_file && !mcdisable_output_files)
  {
    va_start(ap, format);
    vfprintf(siminfo_file, format, ap);
    va_end(ap);
  }
} /* siminfo_out */


/*******************************************************************************
* mcdatainfo_out: output detector header
*   mcdatainfo_out(prefix, file_handle, detector) writes info to data file
*******************************************************************************/
static void
mcdatainfo_out(char *pre, FILE *f, MCDETECTOR detector)
{
  if (!f || !detector.m || mcdisable_output_files) return;

  /* output data ============================================================ */
  fprintf(f, "%sDate: %s (%li)\n",       pre, detector.date, detector.date_l);
  fprintf(f, "%stype: %s\n",       pre, detector.type);
  fprintf(f, "%sSource: %s\n",     pre, detector.instrument);
  fprintf(f, "%scomponent: %s\n",  pre, detector.component);
  fprintf(f, "%sposition: %s\n",   pre, detector.position);

  fprintf(f, "%stitle: %s\n",      pre, detector.title);
  fprintf(f, !mcget_run_num() || mcget_run_num() >= mcget_ncount() ?
             "%sNcount: %s\n" :
             "%sratio: %s\n",  pre, detector.ncount);

  if (strlen(detector.filename)) {
    fprintf(f, "%sfilename: %s\n", pre, detector.filename);
  }

  fprintf(f, "%sstatistics: %s\n", pre, detector.statistics);
  fprintf(f, "%ssignal: %s\n",     pre, detector.signal);
  fprintf(f, "%svalues: %s\n",     pre, detector.values);

  if (detector.rank >= 1)
  {
    fprintf(f, "%sxvar: %s\n",     pre, detector.xvar);
    fprintf(f, "%syvar: %s\n",     pre, detector.yvar);
    fprintf(f, "%sxlabel: %s\n",   pre, detector.xlabel);
    fprintf(f, "%sylabel: %s\n",   pre, detector.ylabel);
    if (detector.rank > 1) {
      fprintf(f, "%szvar: %s\n",   pre, detector.zvar);
      fprintf(f, "%szlabel: %s\n", pre, detector.zlabel);
    }
  }

  fprintf(f,
    abs(detector.rank)==1 ?
             "%sxlimits: %s\n" :
             "%sxylimits: %s\n", pre, detector.limits);
  fprintf(f, "%svariables: %s\n", pre,
    strcasestr(detector.format, "list") ? detector.ylabel : detector.variables);

  fflush(f);

} /* mcdatainfo_out */

/* mcdetector_out_array_ascii: output a single array to a file
 *   m: columns
 *   n: rows
 *   p: array
 *   f: file handle (already opened)
 */
static void mcdetector_out_array_ascii(long m, long n, double *p, FILE *f, char istransposed)
{
  if(f)
  {
    int i,j;
    for(j = 0; j < n; j++)
    {
      for(i = 0; i < m; i++)
      {
          fprintf(f, "%.10g ", p[!istransposed ? i*n + j : j*m+i]);
      }
      fprintf(f,"\n");
    }
  }
} /* mcdetector_out_array_ascii */

/*******************************************************************************
* mcdetector_out_0D_ascii: called by mcdetector_out_0D for ascii output
*******************************************************************************/
MCDETECTOR mcdetector_out_0D_ascii(MCDETECTOR detector)
{
  int exists=0;
  FILE *outfile = NULL;

  /* Write data set information to simulation description file. */
  MPI_MASTER(
    siminfo_out("\nbegin data\n"); // detector.component
    mcdatainfo_out("  ", siminfo_file, detector);
    siminfo_out("end data\n");
    /* Don't write if filename is NULL: mcnew_file handles this (return NULL) */
    outfile = mcnew_file(detector.component, "dat", &exists);
    if(outfile)
    {
      /* write data file header and entry in simulation description file */
      mcruninfo_out( "# ", outfile);
      mcdatainfo_out("# ", outfile, detector);
      /* write I I_err N */
      fprintf(outfile, "%g %g %g\n",
        detector.intensity, detector.error, detector.events);
      fclose(outfile);
    }
  ); /* MPI_MASTER */
  return(detector);
} /* mcdetector_out_0D_ascii */

/*******************************************************************************
* mcdetector_out_1D_ascii: called by mcdetector_out_1D for ascii output
*******************************************************************************/
MCDETECTOR mcdetector_out_1D_ascii(MCDETECTOR detector)
{
  int exists=0;
  FILE *outfile = NULL;

  MPI_MASTER(
    /* Write data set information to simulation description file. */
    siminfo_out("\nbegin data\n"); // detector.filename
    mcdatainfo_out("  ", siminfo_file, detector);
    siminfo_out("end data\n");
    /* Loop over array elements, writing to file. */
    /* Don't write if filename is NULL: mcnew_file handles this (return NULL) */
    outfile = mcnew_file(detector.filename, "dat", &exists);
    if(outfile)
    {
      /* write data file header and entry in simulation description file */
      mcruninfo_out( "# ", outfile);
      mcdatainfo_out("# ", outfile, detector);
      /* output the 1D array columns */
      mcdetector_out_array_ascii(detector.m, detector.n, detector.p1, outfile, detector.istransposed);

      fclose(outfile);
    }
  ); /* MPI_MASTER */
  return(detector);

}  /* mcdetector_out_1D_ascii */

/*******************************************************************************
* mcdetector_out_2D_ascii: called by mcdetector_out_2D for ascii output
*******************************************************************************/
MCDETECTOR mcdetector_out_2D_ascii(MCDETECTOR detector)
{
  int exists=0;
  FILE *outfile = NULL;

  MPI_MASTER(
    /* Loop over array elements, writing to file. */
    /* Don't write if filename is NULL: mcnew_file handles this (return NULL) */
    outfile = mcnew_file(detector.filename, "dat", &exists);
    if(outfile)
    {
      /* write header only if file has just been created (not appending) */
      if (!exists) {
        /* Write data set information to simulation description file. */
        siminfo_out("\nbegin data\n"); // detector.filename
        mcdatainfo_out("  ", siminfo_file, detector);
        siminfo_out("end data\n");

        mcruninfo_out( "# ", outfile);
        mcdatainfo_out("# ", outfile,   detector);
      }
      /* Add # Data entry for any write to the file (e.g. via -USR2, see GitHub issue #2174 ) */
      fprintf(outfile, "# Data [%s/%s] %s:\n", detector.component, detector.filename, detector.zvar);
      mcdetector_out_array_ascii(detector.m, detector.n*detector.p, detector.p1,
        outfile, detector.istransposed);
      if (detector.p2) {
        fprintf(outfile, "# Errors [%s/%s] %s_err:\n", detector.component, detector.filename, detector.zvar);
        mcdetector_out_array_ascii(detector.m, detector.n*detector.p, detector.p2,
          outfile, detector.istransposed);
      }
      if (detector.p0) {
        fprintf(outfile, "# Events [%s/%s] N:\n", detector.component, detector.filename);
        mcdetector_out_array_ascii(detector.m, detector.n*detector.p, detector.p0,
          outfile, detector.istransposed);
      }
      fclose(outfile);

      if (!exists) {
        if (strcasestr(detector.format, "list"))
          printf("Events:   \"%s\"\n",
            strlen(detector.filename) ? detector.filename : detector.component);
      }
    } /* if outfile */
  ); /* MPI_MASTER */
#ifdef USE_MPI
  if (strcasestr(detector.format, "list") && mpi_node_count > 1) {
    int node_i=0;
    /* loop along MPI nodes to write sequentially */
    for(node_i=0; node_i<mpi_node_count; node_i++) {
      /* MPI: slaves wait for the master to write its block, then append theirs */
      MPI_Barrier(MPI_COMM_WORLD);
      if (node_i != mpi_node_root && node_i == mpi_node_rank) {
        if(strlen(detector.filename) && !mcdisable_output_files)	/* Don't write if filename is NULL */
          outfile = mcnew_file(detector.filename, "dat", &exists);
        if (!exists)
          fprintf(stderr, "Warning: [MPI node %i] file '%s' does not exist yet, "
                          "MASTER should have opened it before.\n",
            mpi_node_rank, detector.filename);
        if(outfile) {
          mcdetector_out_array_ascii(detector.m, detector.n*detector.p, detector.p1,
            outfile, detector.istransposed);
          fclose(outfile);
        }
      }
    }
  } /* if strcasestr list */
#endif
  return(detector);
} /* mcdetector_out_2D_ascii */

/*******************************************************************************
* strcpy_valid: makes a valid string for variable names.
*   copy 'original' into 'valid', replacing invalid characters by '_'
*   char arrays must be pre-allocated
*******************************************************************************/
static char *strcpy_valid(char *valid, char *original)
{
  long i;
  int  n=CHAR_BUF_LENGTH; /* max length of valid names */

  if (original == NULL || !strlen(original)) return(NULL);

  if (n > strlen(original)) n = strlen(original);
  else original += strlen(original)-n;
  strncpy(valid, original, n);

  for (i=0; i < n; i++)
  {
    if ( (valid[i] > 122)
      || (valid[i] < 32)
      || (strchr("!\"#$%&'()*+,-.:;<=>?@[\\]^`/ \n\r\t", valid[i]) != NULL) )
    {
      if (i) valid[i] = '_'; else valid[i] = 'm';
    }
  }
  valid[i] = '\0';

  return(valid);
} /* strcpy_valid */

/* end ascii output section ================================================= */







#ifdef USE_NEXUS

/* ========================================================================== */

/*                               NeXus output                                 */

/* ========================================================================== */

#define nxprintf(...)    nxstr('d', __VA_ARGS__)
#define nxprintattr(...) nxstr('a', __VA_ARGS__)

/*******************************************************************************
* nxstr: output a tag=value data set (char) in NeXus/current group
*   when 'format' is larger that 1024 chars it is used as value for the 'tag'
*   else the value is assembled with format and following arguments.
*   type='d' -> data set
*        'a' -> attribute for current data set
*******************************************************************************/
static int nxstr(char type, NXhandle *f, char *tag, char *format, ...)
{
  va_list ap;
  char value[CHAR_BUF_LENGTH];
  int  i;
  int  ret=NX_OK;

  if (!tag || !format || !strlen(tag) || !strlen(format)) return(NX_OK);

  /* assemble the value string */
  if (strlen(format) < CHAR_BUF_LENGTH) {
    va_start(ap, format);
    ret = vsnprintf(value, CHAR_BUF_LENGTH, format, ap);
    va_end(ap);

    i = strlen(value);
  } else {
    i = strlen(format);
  }

  if (type == 'd') {
    /* open/put/close data set */
    if (NXmakedata (f, tag, NX_CHAR, 1, &i) != NX_OK) return(NX_ERROR);
    NXopendata (f, tag);
    if (strlen(format) < CHAR_BUF_LENGTH)
      ret = NXputdata  (f, value);
    else
      ret = NXputdata  (f, format);
    NXclosedata(f);
  } else {
    if (strlen(format) < CHAR_BUF_LENGTH)
      ret = NXputattr  (f, tag, value, strlen(value), NX_CHAR);
    else
      ret = NXputattr  (f, tag, format, strlen(format), NX_CHAR);
  }

  return(ret);

} /* nxstr */

/*******************************************************************************
* mcinfo_readfile: read a full file into a string buffer which is allocated
*   Think to free the buffer after use.
* Used in: mcinfo_out_nexus (nexus)
*******************************************************************************/
char *mcinfo_readfile(char *filename)
{
  FILE *f = fopen(filename, "rb");
  if (!f) return(NULL);
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  rewind(f);
  char *string = malloc(fsize + 1);
  if (string) {
    int n = fread(string, fsize, 1, f);
    fclose(f);

    string[fsize] = 0;
  }
  return(string);
}

/*******************************************************************************
* mcinfo_out: output instrument/simulation groups in NeXus file
* Used in: siminfo_init (nexus)
*******************************************************************************/
static void mcinfo_out_nexus(NXhandle f)
{
  FILE  *fid;     /* for intrument source code/C/IDF */
  char  *buffer=NULL;
  time_t t     =time(NULL); /* for date */
  char   entry0[CHAR_BUF_LENGTH];
  int    count=0;
  char   name[CHAR_BUF_LENGTH];
  char   class[CHAR_BUF_LENGTH];

  if (!f || mcdisable_output_files) return;

  /* write NeXus NXroot attributes */
  /* automatically added: file_name, HDF5_Version, file_time, NeXus_version */
  nxprintattr(f, "creator",   "%s generated with " MCCODE_STRING, instrument_name);

  /* count the number of existing NXentry and create the next one */
  NXgetgroupinfo(f, &count, name, class);
  sprintf(entry0, "entry%i", count+1);

  /* create the main NXentry (mandatory in NeXus) */
  if (NXmakegroup(f, entry0, "NXentry") == NX_OK)
  if (NXopengroup(f, entry0, "NXentry") == NX_OK) {
    nxprintf(nxhandle, "program_name", MCCODE_STRING);
    nxprintf(f, "start_time", ctime(&t));
    nxprintf(f, "title", "%s%s%s simulation generated by instrument %s",
      dirname && strlen(dirname) ? dirname : ".", MC_PATHSEP_S, siminfo_name,
      instrument_name);
    nxprintattr(f, "program_name", MCCODE_STRING);
    nxprintattr(f, "instrument",   instrument_name);
    nxprintattr(f, "simulation",   "%s%s%s",
        dirname && strlen(dirname) ? dirname : ".", MC_PATHSEP_S, siminfo_name);

    /* write NeXus instrument group */
    if (NXmakegroup(f, "instrument", "NXinstrument") == NX_OK)
    if (NXopengroup(f, "instrument", "NXinstrument") == NX_OK) {
      int   i;
      char *string=NULL;

      /* write NeXus parameters(types) data =================================== */
      string = (char*)malloc(CHAR_BUF_LENGTH);
      if (string) {
        strcpy(string, "");
        for(i = 0; i < numipar; i++)
        {
          char ThisParam[CHAR_BUF_LENGTH];
          snprintf(ThisParam, CHAR_BUF_LENGTH, " %s(%s)", mcinputtable[i].name,
                  (*mcinputtypes[mcinputtable[i].type].parminfo)
                      (mcinputtable[i].name));
          if (strlen(string) + strlen(ThisParam) < CHAR_BUF_LENGTH)
            strcat(string, ThisParam);
        }
        nxprintattr(f, "Parameters",    string);
        free(string);
      }

      nxprintattr(f, "name",          instrument_name);
      nxprintf   (f, "name",          instrument_name);
      nxprintattr(f, "Source",        instrument_source);

      nxprintattr(f, "Trace_enabled", traceenabled ? "yes" : "no");
      nxprintattr(f, "Default_main",  defaultmain ?  "yes" : "no");
#ifdef MC_EMBEDDED_RUNTIME
      nxprintattr(f, "Embedded_runtime", "yes");
#else
      nxprintattr(f, "Embedded_runtime", "no");
#endif

      /* add instrument source code when available */
      buffer = mcinfo_readfile(instrument_source);
      if (buffer && strlen(buffer)) {
        long length=strlen(buffer);
        nxprintf (f, "description", buffer);
        NXopendata(f,"description");
        nxprintattr(f, "file_name", instrument_source);
        nxprintattr(f, "file_size", "%li", length);
        nxprintattr(f, "MCCODE_STRING", MCCODE_STRING);
        NXclosedata(f);
        nxprintf (f,"instrument_source", "%s " MCCODE_NAME " " MCCODE_PARTICLE " Monte Carlo simulation", instrument_name);
        free(buffer);
      } else
        nxprintf (f, "description", "File %s not found (instrument description %s is missing)",
          instrument_source, instrument_name);

      if (mcnexus_embed_idf) {
        /* add Mantid/IDF.xml when available */
        char *IDFfile=NULL;
        IDFfile = (char*)malloc(CHAR_BUF_LENGTH);
        sprintf(IDFfile,"%s%s",instrument_source,".xml");
        buffer = mcinfo_readfile(IDFfile);
        if (buffer && strlen(buffer)) {
          NXmakegroup (nxhandle, "instrument_xml", "NXnote");
          NXopengroup (nxhandle, "instrument_xml", "NXnote");
          nxprintf(f, "data", buffer);
          nxprintf(f, "description", "IDF.xml file found with instrument %s", instrument_source);
          nxprintf(f, "type", "text/xml");
          NXclosegroup(f); /* instrument_xml */
          free(buffer);
        }
        free(IDFfile);
      }

      /* Add "components" entry */
      if (NXmakegroup(f, "components", "NXdata") == NX_OK) {
        NXopengroup(f, "components", "NXdata");
        nxprintattr(f, "description", "Component list for instrument %s",  instrument_name);
	NXclosegroup(f); /* components */
      } else {
	printf("Failed to create NeXus component hierarchy\n");
      }
      NXclosegroup(f); /* instrument */
    } /* NXinstrument */

    /* write NeXus simulation group */
    if (NXmakegroup(f, "simulation", "NXnote") == NX_OK)
    if (NXopengroup(f, "simulation", "NXnote") == NX_OK) {

      nxprintattr(f, "name",   "%s%s%s",
        dirname && strlen(dirname) ? dirname : ".", MC_PATHSEP_S, siminfo_name);

      nxprintf   (f, "name",      "%s",     siminfo_name);
      nxprintattr(f, "Format",    mcformat && strlen(mcformat) ? mcformat : MCCODE_NAME);
      nxprintattr(f, "URL",       "http://www.mccode.org");
      nxprintattr(f, "program",   MCCODE_STRING);
      nxprintattr(f, "Instrument",instrument_source);
      nxprintattr(f, "Trace",     mcdotrace ?     "yes" : "no");
      nxprintattr(f, "Gravitation",mcgravitation ? "yes" : "no");
      nxprintattr(f, "Seed",      "%li", mcseed);
      nxprintattr(f, "Directory", dirname);
    #ifdef USE_MPI
      if (mpi_node_count > 1)
        nxprintf(f, "Nodes", "%i",        mpi_node_count);
    #endif

      /* output parameter string ================================================ */
      if (NXmakegroup(f, "Param", "NXparameters") == NX_OK) {
	NXopengroup(f,"Param", "NXparameters");
        int i;
        char string[CHAR_BUF_LENGTH];
        for(i = 0; i < numipar; i++) {
          if (mcget_run_num() || (mcinputtable[i].val && strlen(mcinputtable[i].val))) {
            if (mcinputtable[i].par == NULL)
              strncpy(string, (mcinputtable[i].val ? mcinputtable[i].val : ""), CHAR_BUF_LENGTH);
            else
              (*mcinputtypes[mcinputtable[i].type].printer)(string, mcinputtable[i].par);

            nxprintf(f,  mcinputtable[i].name, "%s", string);
            nxprintattr(f, mcinputtable[i].name, string);
          }
        }
        NXclosegroup(f); /* Param */
      } /* NXparameters */
      NXclosegroup(f); /* simulation */
    } /* NXsimulation */

    /* create a group to hold all links for all monitors */
    NXmakegroup(f, "data", "NXdetector");

    /* leave the NXentry opened (closed at exit) */
  } /* NXentry */
} /* mcinfo_out_nexus */

/*******************************************************************************
* mccomp_placement_type_nexus:
*   Places
*    - absolute (3x1) position
*    - absolute (3x3) rotation
*    - type / class of component instance into attributes under
*     entry<N>/instrument/compname
*   requires: NXentry to be opened
*******************************************************************************/
static void mccomp_placement_type_nexus(NXhandle nxhandle, char* component, Coords position, Rotation rotation, char* comptype)
{
  /* open NeXus instrument group */

  #ifdef USE_NEXUS
  if(nxhandle) {
    if (NXopengroup(nxhandle, "instrument", "NXinstrument") == NX_OK) {
      if (NXopengroup(nxhandle, "components", "NXdata") == NX_OK) {
	if (NXmakegroup(nxhandle, component, "NXdata") == NX_OK) {
	  if (NXopengroup(nxhandle, component, "NXdata") == NX_OK) {
	    int64_t pdims[3]; pdims[0]=3; pdims[1]=0; pdims[2]=0;
	    if (NXcompmakedata64(nxhandle, "Position", NX_FLOAT64, 1, pdims, NX_COMPRESSION, pdims) == NX_OK) {
	      if (NXopendata(nxhandle, "Position") == NX_OK) {
		double pos[3]; coords_get(position, &pos[0], &pos[1], &pos[2]);
		if (NXputdata (nxhandle, pos) == NX_OK) {
		  NXclosedata(nxhandle);
		} else {
		  fprintf(stderr, "COULD NOT PUT Position field for component %s\n",component);
		}
	      } else {
		fprintf(stderr, "Warning: could not open Position field for component %s\n",component);
	      }
	    }
	    int64_t rdims[3]; rdims[0]=3; rdims[1]=3; rdims[2]=0;
	    if (NXcompmakedata64(nxhandle, "Rotation", NX_FLOAT64, 2, rdims, NX_COMPRESSION, rdims) == NX_OK) {
	      if (NXopendata(nxhandle, "Rotation") == NX_OK) {
		if (NXputdata (nxhandle, rotation) == NX_OK) {
		  NXclosedata(nxhandle);
		} else {
		  fprintf(stderr, "COULD NOT PUT Rotation field for component %s\n",component);
		}
	      } else {
		fprintf(stderr, "Warning: could not open Rotation field for component %s\n",component);
	      }
	    }
	    nxprintf(nxhandle, "Component_type", comptype);
	    NXclosegroup(nxhandle); // component
	  } else {
	    printf("FAILED to open comp data group %s\n",component);
	  }
	} else {
	  printf("FAILED to create comp data group %s\n",component);
	}
	NXclosegroup(nxhandle); // components
      } else {
	printf("Failed to open NeXus component hierarchy\n");
      }
      NXclosegroup(nxhandle); // instrument
    } else {
      printf("Failed to open NeXus instrument hierarchy\n");
    }
  } else {
    fprintf(stderr,"NO NEXUS FILE\n");
  }
  #endif
} /* mccomp_placement_nexus */

/*******************************************************************************
* mccomp_param_nexus:
*   Output parameter/value pair for component instance into
*   the attribute
*     entry<N>/instrument/compname/parameter
*   requires: NXentry to be opened
*******************************************************************************/
static void mccomp_param_nexus(NXhandle nxhandle, char* component, char* parameter, char* defval, char* value, char* type)
{
  /* open NeXus instrument group */

  #ifdef USE_NEXUS
  if(nxhandle) {
    if (NXopengroup(nxhandle, "instrument", "NXinstrument") == NX_OK) {
      if (NXopengroup(nxhandle, "components", "NXdata") == NX_OK) {
	if (NXopengroup(nxhandle, component, "NXdata") == NX_OK) {
	  NXMDisableErrorReporting(); /* inactivate NeXus error messages, as creation may fail */
	  NXmakegroup(nxhandle, "parameters", "NXdata");
	  NXMEnableErrorReporting();  /* re-enable NeXus error messages */
	  if (NXopengroup(nxhandle, "parameters", "NXdata") == NX_OK) {
	    NXmakegroup(nxhandle, parameter, "NXnote");
	    if (NXopengroup(nxhandle, parameter, "NXnote") == NX_OK) {
	      nxprintattr(nxhandle, "type", type);
	      nxprintattr(nxhandle, "default",  defval);
	      nxprintattr(nxhandle, "value",  value);
	      NXclosegroup(nxhandle); // parameter
	    } else {
	      printf("FAILED to open parameters %s data group \n",parameter);
	    }
	    NXclosegroup(nxhandle); // "parameters"
	  } else {
	    printf("FAILED to open comp/parameters data group \n");
	  }
	  NXclosegroup(nxhandle); // component
	  } else {
	  printf("FAILED to open comp data group %s\n",component);
	}
	NXclosegroup(nxhandle); // components
      } else {
	printf("Failed to open NeXus component hierarchy\n");
      }
      NXclosegroup(nxhandle); // instrument
    } else {
      printf("Failed to open NeXus instrument hierarchy\n");
    }
  } else {
    fprintf(stderr,"NO NEXUS FILE\n");
  }
#endif
} /* mccomp_param_nexus */

/*******************************************************************************
* mcdatainfo_out_nexus: output detector header
*   mcdatainfo_out_nexus(detector) create group and write info to NeXus data file
*   open data:NXdetector then filename:NXdata and write headers/attributes
*   requires: NXentry to be opened
*******************************************************************************/
static void
mcdatainfo_out_nexus(NXhandle f, MCDETECTOR detector)
{
  char data_name[CHAR_BUF_LENGTH];
  if (!f || !detector.m || mcdisable_output_files) return;

  strcpy_valid(data_name,
    strlen(detector.filename) ?
      detector.filename : detector.component);

  /* the NXdetector group has been created in mcinfo_out_nexus (siminfo_init) */
  if (NXopengroup(f, "instrument", "NXinstrument") == NX_OK) {
    if (NXopengroup(f, "components", "NXdata") == NX_OK) {
      NXMDisableErrorReporting(); /* inactivate NeXus error messages, as creation may fail */
      NXmakegroup(f, detector.nexuscomp, "NXdata");
      if (NXopengroup(f, detector.nexuscomp, "NXdata") == NX_OK) {
	NXmakegroup(f, "output", "NXdetector");
	if (NXopengroup(f, "output", "NXdetector") == NX_OK) {
	  if (NXmakegroup(f, data_name, "NXdata") == NX_OK) {
	    if (NXopengroup(f, data_name, "NXdata") == NX_OK) {
	      /* output metadata (as attributes) ======================================== */
	      nxprintattr(f, "Date",       detector.date);
	      nxprintattr(f, "type",       detector.type);
	      nxprintattr(f, "Source",     detector.instrument);
	      nxprintattr(f, "component",  detector.component);
	      nxprintattr(f, "position",   detector.position);

	      nxprintattr(f, "title",      detector.title);
	      nxprintattr(f, !mcget_run_num() || mcget_run_num() >= mcget_ncount() ?
			  "Ncount" :
			  "ratio",  detector.ncount);

	      if (strlen(detector.filename)) {
		nxprintattr(f, "filename", detector.filename);
	      }

	      nxprintattr(f, "statistics", detector.statistics);
	      nxprintattr(f, "signal",     detector.signal);
	      nxprintattr(f, "values",     detector.values);

	      if (detector.rank >= 1)
		{
		  nxprintattr(f, "xvar",     detector.xvar);
		  nxprintattr(f, "yvar",     detector.yvar);
		  nxprintattr(f, "xlabel",   detector.xlabel);
		  nxprintattr(f, "ylabel",   detector.ylabel);
		  if (detector.rank > 1) {
		    nxprintattr(f, "zvar",   detector.zvar);
		    nxprintattr(f, "zlabel", detector.zlabel);
		  }
		}

	      nxprintattr(f, abs(detector.rank)==1 ?
			  "xlimits" :
			  "xylimits", detector.limits);
	      nxprintattr(f, "variables",
			  strcasestr(detector.format, "list") ? detector.ylabel : detector.variables);

	      NXclosegroup(f); // data_name
	    }
	  }
	}
	NXclosegroup(f); // output
	NXclosegroup(f); // detector.nexuscomp
      }
      NXclosegroup(f); // components
    }
    NXMEnableErrorReporting();  /* re-enable NeXus error messages */
    NXclosegroup(f); // instrument
  } /* NXdetector (instrument) */ 
} /* mcdatainfo_out_nexus */

/*******************************************************************************
* mcdetector_out_axis_nexus: write detector axis into current NXdata
*   requires: NXdata to be opened
*******************************************************************************/
int mcdetector_out_axis_nexus(NXhandle f, char *label, char *var, int rank, long length, double min, double max)
{
  if (!f || length <= 1 || mcdisable_output_files || max == min) return(NX_OK);
  else {
    double *axis;
    axis=malloc(sizeof(double)*length);
    if (!axis ) {
      printf("Fatal memory error allocating NeXus axis of length %li, exiting!\n", length);
      return(NX_ERROR);
    }
    char *valid;
    valid=malloc(sizeof(char)*CHAR_BUF_LENGTH);
    if (!valid ) {
      printf("Fatal memory error allocating label axis of length %i, exiting!\n", CHAR_BUF_LENGTH);
      free(axis);
      return(NX_ERROR);
    }
    int dim=(int)length;
    int i;
    int nprimary=1;
    /* create an axis from [min:max] */
    for(i = 0; i < length; i++)
      axis[i] = min+(max-min)*(i+0.5)/length;
    /* create the data set */
    strcpy_valid(valid, label);
    NXcompmakedata(f, valid, NX_FLOAT64, 1, &dim, NX_COMPRESSION, &dim);
    /* open it */
    if (NXopendata(f, valid) != NX_OK) {
      fprintf(stderr, "Warning: could not open axis rank %i '%s' (NeXus)\n",
        rank, valid);
      free(axis);
      free(valid);
      return(NX_ERROR);
    }
    /* put the axis and its attributes */
    NXputdata  (f, axis);
    nxprintattr(f, "long_name",  label);
    nxprintattr(f, "short_name", var);
    NXputattr  (f, "axis",       &rank,     1, NX_INT32);
    nxprintattr(f, "units",      var);
    NXputattr  (f, "primary",    &nprimary, 1, NX_INT32);
    NXclosedata(f);
    free(axis);
    free(valid);
    return(NX_OK);
  }
} /* mcdetector_out_axis_nexus */

/*******************************************************************************
* mcdetector_out_array_nexus: write detector array into current NXdata (1D,2D)
*   requires: NXdata to be opened
*******************************************************************************/
int mcdetector_out_array_nexus(NXhandle f, char *part, double *data, MCDETECTOR detector)
{

  int64_t dims[3]={detector.m,detector.n,detector.p};  /* number of elements to write */
  int64_t fulldims[3]={detector.m,detector.n,detector.p};
  int signal=1;
  int exists=0;
  int64_t current_dims[3]={0,0,0};
  int ret=NX_OK;

  if (!f || !data || !detector.m || mcdisable_output_files) return(NX_OK);

  /* when this is a list, we set 1st dimension to NX_UNLIMITED for creation */
  if (strcasestr(detector.format, "list")) fulldims[0] = NX_UNLIMITED;

  /* create the data set in NXdata group */
  NXMDisableErrorReporting(); /* inactivate NeXus error messages, as creation may fail */
  ret = NXcompmakedata64(f, part, NX_FLOAT64, detector.rank, fulldims, NX_COMPRESSION, dims);
  if (ret != NX_OK) {
    /* failed: data set already exists */
    int datatype=0;
    int rank=0;
    exists=1;
    /* inquire current size of data set (nb of events stored) */
    NXopendata(f, part);
    NXgetinfo64(f, &rank, current_dims, &datatype);
    NXclosedata(f);
  }
  NXMEnableErrorReporting();  /* re-enable NeXus error messages */

  /* open the data set */
  if (NXopendata(f, part) == NX_ERROR) {
    fprintf(stderr, "Warning: could not open DataSet %s '%s' (NeXus)\n",
      part, detector.title);
    return(NX_ERROR);
  }
  if (strcasestr(detector.format, "list")) {
    current_dims[1] = current_dims[2] = 0; /* set starting location for writing slab */
    NXputslab64(f, data, current_dims, dims);
    if (!exists)
      printf("Events:   \"%s\"\n",
        strlen(detector.filename) ? detector.filename : detector.component);
    else
      printf("Append:   \"%s\"\n",
	     strlen(detector.filename) ? detector.filename : detector.component);
  } else {
    NXputdata (f, data);
  }

  if (strstr(part,"data") || strstr(part, "events")) {
    NXputattr(f, "signal", &signal, 1, NX_INT32);
    nxprintattr(f, "short_name", strlen(detector.filename) ?
      detector.filename : detector.component);
  }
  nxprintattr(f, "long_name", "%s '%s'", part, detector.title);
  NXclosedata(f);

  return(NX_OK);
} /* mcdetector_out_array_nexus */

/*******************************************************************************
* mcdetector_out_data_nexus: write detector axes+data into current NXdata
*   The data:NXdetector is opened, then filename:NXdata
*   requires: NXentry to be opened
*******************************************************************************/
int mcdetector_out_data_nexus(NXhandle f, MCDETECTOR detector)
{
  char data_name[CHAR_BUF_LENGTH];

  if (!f || !detector.m || mcdisable_output_files) return(NX_OK);

  strcpy_valid(data_name,
    strlen(detector.filename) ?
      detector.filename : detector.component);
  NXlink pLink;
  /* the NXdetector group has been created in mcinfo_out_nexus (siminfo_init) */
  if (NXopengroup(f, "instrument", "NXinstrument") == NX_OK) {
    if (NXopengroup(f, "components", "NXdata") == NX_OK) {
      if (NXopengroup(f, detector.nexuscomp, "NXdata") == NX_OK) {
	if (NXopengroup(f, "output", "NXdetector") == NX_OK) {

	  /* the NXdata group has been created in mcdatainfo_out_nexus */
	  if (NXopengroup(f, data_name, "NXdata") == NX_OK) {
	    
	    MPI_MASTER(
		       nxprintattr(f, "options",
				   strlen(detector.options) ? detector.options : "None");
		       );
	    /* write axes, for histogram data sets, not for lists */
	    if (!strcasestr(detector.format, "list")) {
	      mcdetector_out_axis_nexus(f, detector.xlabel, detector.xvar,
					1, detector.m, detector.xmin, detector.xmax);
	      mcdetector_out_axis_nexus(f, detector.ylabel, detector.yvar,
					2, detector.n, detector.ymin, detector.ymax);
	      mcdetector_out_axis_nexus(f, detector.zlabel, detector.zvar,
					3, detector.p, detector.zmin, detector.zmax); 
	    } else {
	      	    MPI_MASTER(
			       nxprintattr(f, "dataset columns",
					   strlen(detector.ylabel) ? detector.ylabel : "None");
		    );
	    }

	    /* write the actual data (appended if already exists) */
	    if (!strcasestr(detector.format, "list") && !strcasestr(detector.format, "pixels")) {
	      mcdetector_out_array_nexus(f, "data", detector.p1, detector);
	      mcdetector_out_array_nexus(f, "errors", detector.p2, detector);
	      mcdetector_out_array_nexus(f, "ncount", detector.p0, detector);
	    } else if (strcasestr(detector.format, "pixels")) {
	      mcdetector_out_array_nexus(  f, "pixels", detector.p1, detector);
	    } else {
	      mcdetector_out_array_nexus(  f, "events", detector.p1, detector);
	    }
	    NXclosegroup(f);
	    NXopengroup(f, data_name, "NXdata");
	    NXgetgroupID(nxhandle, &pLink);
	    NXclosegroup(f);
	  } /* NXdata data_name*/
	  NXclosegroup(f);
	} /* NXdetector output */
	NXclosegroup(f);
      } /* NXdata detector.nexuscomp */
      NXclosegroup(f);
    } /* NXdata components */
    NXclosegroup(f);
  } /* NXdata instrument */
  
  if (!strcasestr(detector.format, "pixels")) {
    if (NXopengroup(f, "data", "NXdetector") == NX_OK) {
      NXmakelink(nxhandle, &pLink);
      NXclosegroup(f);
    }
  }
  return(NX_OK);
} /* mcdetector_out_array_nexus */

#ifdef USE_MPI
/*******************************************************************************
* mcdetector_out_list_slaves: slaves send their list data to master which writes
*   requires: NXentry to be opened
* WARNING: this method has a flaw: it requires all nodes to flush the lists
*   the same number of times. In case one node is just below the buffer size
*   when finishing (e.g. monitor_nd), it may not trigger save but others may.
*   Then the number of recv/send is not constant along nodes, and simulation stalls.
*******************************************************************************/
MCDETECTOR mcdetector_out_list_slaves(MCDETECTOR detector)
{
  int     node_i=0;
  MPI_MASTER(
	     printf("\n** MPI master gathering slave node list data ** \n");
  );

  if (mpi_node_rank != mpi_node_root) {
    /* MPI slave: slaves send their data to master: 2 MPI_Send calls */
    /* m, n, p must be sent first, since all slaves do not have the same number of events */
    int mnp[3]={detector.m,detector.n,detector.p};

    if (mc_MPI_Send(mnp, 3, MPI_INT, mpi_node_root)!= MPI_SUCCESS)
      fprintf(stderr, "Warning: proc %i to master: MPI_Send mnp list error (mcdetector_out_list_slaves)\n", mpi_node_rank);
    if (!detector.p1
     || mc_MPI_Send(detector.p1, mnp[0]*mnp[1]*mnp[2], MPI_DOUBLE, mpi_node_root) != MPI_SUCCESS)
      fprintf(stderr, "Warning: proc %i to master: MPI_Send p1 list error: mnp=%i (mcdetector_out_list_slaves)\n", mpi_node_rank, abs(mnp[0]*mnp[1]*mnp[2]));
    /* slaves are done: sent mnp and p1 */
  } /* end slaves */

  /* MPI master: receive data from slaves sequentially: 2 MPI_Recv calls */

  if (mpi_node_rank == mpi_node_root) {
    for(node_i=0; node_i<mpi_node_count; node_i++) {
      double *this_p1=NULL;                               /* buffer to hold the list from slaves */
      int     mnp[3]={0,0,0};  /* size of this buffer */
      if (node_i != mpi_node_root) { /* get data from slaves */
	if (mc_MPI_Recv(mnp, 3, MPI_INT, node_i) != MPI_SUCCESS)
	  fprintf(stderr, "Warning: master from proc %i: "
		  "MPI_Recv mnp list error (mcdetector_write_data)\n", node_i);
	if (mnp[0]*mnp[1]*mnp[2]) {
	  this_p1 = (double *)calloc(mnp[0]*mnp[1]*mnp[2], sizeof(double));
	  if (!this_p1 || mc_MPI_Recv(this_p1, abs(mnp[0]*mnp[1]*mnp[2]), MPI_DOUBLE, node_i)!= MPI_SUCCESS)
	    fprintf(stderr, "Warning: master from proc %i: "
		    "MPI_Recv p1 list error: mnp=%i (mcdetector_write_data)\n", node_i, mnp[0]*mnp[1]*mnp[2]);
	  else {
	    printf(". MPI master writing data for slave node %i\n",node_i);
	    detector.p1 = this_p1;
	    detector.m  = mnp[0]; detector.n  = mnp[1]; detector.p  = mnp[2];

	    mcdetector_out_data_nexus(nxhandle, detector);
	  }
	}
      } /* if not master */
      free(this_p1);
    } /* for */
  MPI_MASTER(
	     printf("\n** Done ** \n");
  );
  }
  // Common return statement for slaves / master alike
  return(detector);
}
#endif

MCDETECTOR mcdetector_out_0D_nexus(MCDETECTOR detector)
{
  /* Write data set information to NeXus file. */
  MPI_MASTER(
    mcdatainfo_out_nexus(nxhandle, detector);
  );

  return(detector);
} /* mcdetector_out_0D_ascii */

MCDETECTOR mcdetector_out_1D_nexus(MCDETECTOR detector)
{
  MPI_MASTER(
  mcdatainfo_out_nexus(nxhandle, detector);
  mcdetector_out_data_nexus(nxhandle, detector);
  );
  return(detector);
} /* mcdetector_out_1D_ascii */

MCDETECTOR mcdetector_out_2D_nexus(MCDETECTOR detector)
{
  MPI_MASTER(
  mcdatainfo_out_nexus(nxhandle, detector);
  mcdetector_out_data_nexus(nxhandle, detector);
  );

#ifdef USE_MPI // and USE_NEXUS
  /* NeXus: slave nodes have master write their lists */
  if (strcasestr(detector.format, "list") && mpi_node_count > 1) {
    mcdetector_out_list_slaves(detector);
  }
#endif /* USE_MPI */

  return(detector);
} /* mcdetector_out_2D_nexus */

MCDETECTOR mcdetector_out_3D_nexus(MCDETECTOR detector)
{
  printf("Received detector from %s\n",detector.component);
  MPI_MASTER(
  mcdatainfo_out_nexus(nxhandle, detector);
  mcdetector_out_data_nexus(nxhandle, detector);
  );
  return(detector);
} /* mcdetector_out_3D_nexus */


#endif /* USE_NEXUS*/








/* ========================================================================== */

/*                            Main input functions                            */
/*            DETECTOR_OUT_xD function calls -> ascii or NeXus                */

/* ========================================================================== */

/*******************************************************************************
* siminfo_init:   open SIM and write header
*******************************************************************************/
FILE *siminfo_init(FILE *f)
{
  int exists=0;

  /* check format */
  if (!mcformat || !strlen(mcformat)
   || !strcasecmp(mcformat, "MCSTAS") || !strcasecmp(mcformat, "MCXTRACE")
   || !strcasecmp(mcformat, "PGPLOT") || !strcasecmp(mcformat, "GNUPLOT") || !strcasecmp(mcformat, "MCCODE")
   || !strcasecmp(mcformat, "MATLAB")) {
    mcformat="McCode";
#ifdef USE_NEXUS
  } else if (strcasestr(mcformat, "NeXus")) {
    /* Do nothing */
#endif
  } else {
    fprintf(stderr,
	    "Warning: You have requested the output format %s which is unsupported by this binary. Resetting to standard %s format.\n",mcformat ,"McCode");
    mcformat="McCode";
  }

  /* open the SIM file if not defined yet */
  if (siminfo_file || mcdisable_output_files)
    return (siminfo_file);

#ifdef USE_NEXUS
  /* only master writes NeXus header: calls NXopen(nxhandle) */
  if (mcformat && strcasestr(mcformat, "NeXus")) {
	  MPI_MASTER(
	  siminfo_file = mcnew_file(siminfo_name, "h5", &exists);
    if(!siminfo_file)
      fprintf(stderr,
	      "Warning: could not open simulation description file '%s'\n",
	      siminfo_name);
	  else
	    mcinfo_out_nexus(nxhandle);
	  );
    return(siminfo_file); /* points to nxhandle */
  }
#endif

  /* write main description file (only MASTER) */
  MPI_MASTER(

  siminfo_file = mcnew_file(siminfo_name, "sim", &exists);
  if(!siminfo_file)
    fprintf(stderr,
	    "Warning: could not open simulation description file '%s'\n",
	    siminfo_name);
  else
  {
    /* write SIM header */
    time_t t=time(NULL);
    siminfo_out("%s simulation description file for %s.\n",
      MCCODE_NAME, instrument_name);
    siminfo_out("Date:    %s", ctime(&t)); /* includes \n */
    siminfo_out("Program: %s\n\n", MCCODE_STRING);

    siminfo_out("begin instrument: %s\n", instrument_name);
    mcinfo_out(   "  ", siminfo_file);
    siminfo_out("end instrument\n");

    siminfo_out("\nbegin simulation: %s\n", dirname);
    mcruninfo_out("  ", siminfo_file);
    siminfo_out("end simulation\n");

  }
  ); /* MPI_MASTER */
  return (siminfo_file);

} /* siminfo_init */

/*******************************************************************************
*   siminfo_close:  close SIM
*******************************************************************************/
void siminfo_close()
{
#ifdef USE_MPI
  if(mpi_node_rank == mpi_node_root) {
#endif
  if(siminfo_file && !mcdisable_output_files) {
#ifdef USE_NEXUS
    if (mcformat && strcasestr(mcformat, "NeXus")) {
      time_t t=time(NULL);
      nxprintf(nxhandle, "end_time", ctime(&t));
      nxprintf(nxhandle, "duration", "%li", (long)t-mcstartdate);
      NXclosegroup(nxhandle); /* NXentry */
      NXclose(&nxhandle);
    } else {
#endif
      fclose(siminfo_file);
#ifdef USE_NEXUS
    }
#endif
#ifdef USE_MPI
  }
#endif
    siminfo_file = NULL;
  }
} /* siminfo_close */

/*******************************************************************************
* mcdetector_out_0D: wrapper for 0D (single value).
*   Output single detector/monitor data (p0, p1, p2).
*   Title is t, component name is c.
*******************************************************************************/
MCDETECTOR mcdetector_out_0D(char *t, double p0, double p1, double p2,
			     char *c, Coords posa, Rotation rota, int index)
{
  /* import and perform basic detector analysis (and handle MPI reduce) */
  MCDETECTOR detector = detector_import(mcformat,
    c, (t ? t : MCCODE_STRING " data"),
    1, 1, 1,
    "I", "", "",
    "I", "", "",
    0, 0, 0, 0, 0, 0, c,
    &p0, &p1, &p2, posa, rota, index); /* write Detector: line */

#ifdef USE_NEXUS
  if (strcasestr(detector.format, "NeXus"))
    return(mcdetector_out_0D_nexus(detector));
  else
#endif
    return(mcdetector_out_0D_ascii(detector));

} /* mcdetector_out_0D */



/*******************************************************************************
* mcdetector_out_1D: wrapper for 1D.
*   Output 1d detector data (p0, p1, p2) for n bins linearly
*   distributed across the range x1..x2 (x1 is lower limit of first
*   bin, x2 is upper limit of last bin). Title is t, axis labels are xl
*   and yl. File name is f, component name is c.
*
*   t:    title
*   xl:   x-label
*   yl:   y-label
*   xvar: measured variable length
*   x1:   x axus min
*   x2:   x axis max
*   n:    1d data vector lenght
*   p0:   pntr to start of data block#0
*   p1:   pntr to start of data block#1
*   p2:   pntr to start of data block#2
*   f:    filename
*
*   Not included in the macro, and here forwarded to detector_import:
*   c:    ?
*   posa: ?
*******************************************************************************/
MCDETECTOR mcdetector_out_1D(char *t, char *xl, char *yl,
        char *xvar, double x1, double x2,
        long n,
        double *p0, double *p1, double *p2, char *f,
        char *c, Coords posa, Rotation rota, int index)
{
  /* import and perform basic detector analysis (and handle MPI_Reduce) */
  // detector_import calls mcdetector_statistics, which will return different
  // MCDETECTOR versions for 1-D data based on the value of mcformat.
  //
  MCDETECTOR detector = detector_import(mcformat,
    c, (t ? t : MCCODE_STRING " 1D data"),
    n, 1, 1,
    xl, yl, (n > 1 ? "Signal per bin" : " Signal"),
    xvar, "(I,I_err)", "I",
    x1, x2, 0, 0, 0, 0, f,
    p0, p1, p2, posa, rota, index); /* write Detector: line */
  if (!detector.p1 || !detector.m) return(detector);

#ifdef USE_NEXUS
  if (strcasestr(detector.format, "NeXus"))
    detector = mcdetector_out_1D_nexus(detector);
  else
#endif
    detector = mcdetector_out_1D_ascii(detector);
  if (detector.p1 != p1 && detector.p1) {
    // mcdetector_statistics allocated memory but it hasn't been freed.
    free(detector.p1);
    // plus undo the other damage done there:
    detector.p0 = p0; // was set to NULL
    detector.p1 = p1; // was set to this_p1
    detector.p2 = p2; // was set to NULL
    detector.m = detector.n; // (e.g., labs(n))
    detector.n = 1;  // not (n x n)
    detector.istransposed = n < 0 ? 1 : 0;
  }
  return detector;

} /* mcdetector_out_1D */

/*******************************************************************************
* mcdetector_out_2D: wrapper for 2D.
*   Special case for list: master creates file first, then slaves append their
*   blocks without header-
*
*   t:    title
*   xl:   x-label
*   yl:   y-label
*   x1:   x axus min
*   x2:   x axis max
*   y1:   y axis min
*   y2:   y axis max
*   m:    dim 1 (x) size
*   n:    dim 2 (y) size
*   p0:   pntr to start of data block#0
*   p1:   pntr to start of data block#1
*   p2:   pntr to start of data block#2
*   f:    filename
*
*   Not included in the macro, and here forwarded to detector_import:
*   c:    ?
*   posa: ?
*   rota: ?
*******************************************************************************/
MCDETECTOR mcdetector_out_2D(char *t, char *xl, char *yl,
                  double x1, double x2, double y1, double y2,
                  long m, long n,
                  double *p0, double *p1, double *p2, char *f,
		  char *c, Coords posa, Rotation rota, int index)
{
  char xvar[CHAR_BUF_LENGTH];
  char yvar[CHAR_BUF_LENGTH];

  /* create short axes labels */
  if (xl && strlen(xl)) { strncpy(xvar, xl, CHAR_BUF_LENGTH); xvar[2]='\0'; }
  else strcpy(xvar, "x");
  if (yl && strlen(yl)) { strncpy(yvar, yl, CHAR_BUF_LENGTH); yvar[2]='\0'; }
  else strcpy(yvar, "y");

  MCDETECTOR detector;

  /* import and perform basic detector analysis (and handle MPI_Reduce) */
  if (labs(m) == 1) {/* n>1 on Y, m==1 on X: 1D, no X axis*/
    detector = detector_import(mcformat,
      c, (t ? t : MCCODE_STRING " 1D data"),
      n, 1, 1,
      yl, "", "Signal per bin",
      yvar, "(I,Ierr)", "I",
      y1, y2, x1, x2, 0, 0, f,
      p0, p1, p2, posa, rota, index); /* write Detector: line */
  } else if (labs(n)==1) {/* m>1 on X, n==1 on Y: 1D, no Y axis*/
    detector = detector_import(mcformat,
      c, (t ? t : MCCODE_STRING " 1D data"),
      m, 1, 1,
      xl, "", "Signal per bin",
      xvar, "(I,Ierr)", "I",
      x1, x2, y1, y2, 0, 0, f,
      p0, p1, p2, posa, rota, index); /* write Detector: line */
  }else {
    detector = detector_import(mcformat,
      c, (t ? t : MCCODE_STRING " 2D data"),
      m, n, 1,
      xl, yl, "Signal per bin",
      xvar, yvar, "I",
      x1, x2, y1, y2, 0, 0, f,
      p0, p1, p2, posa, rota, index); /* write Detector: line */
  }

  if (!detector.p1 || !detector.m) return(detector);

#ifdef USE_NEXUS
  if (strcasestr(detector.format, "NeXus"))
    return(mcdetector_out_2D_nexus(detector));
  else
#endif
    return(mcdetector_out_2D_ascii(detector));

} /* mcdetector_out_2D */

/*******************************************************************************
* mcdetector_out_2D_list: List mode 2D including forwarding "options" from
* Monitor_nD
*
*   Special case for list: master creates file first, then slaves append their
*   blocks without header-
*
*   t:    title
*   xl:   x-label
*   yl:   y-label
*   x1:   x axus min
*   x2:   x axis max
*   y1:   y axis min
*   y2:   y axis max
*   m:    dim 1 (x) size
*   n:    dim 2 (y) size
*   p0:   pntr to start of data block#0
*   p1:   pntr to start of data block#1
*   p2:   pntr to start of data block#2
*   f:    filename
*
*   Not included in the macro, and here forwarded to detector_import:
*   c:    ?
*   posa: ?
*   rota: ?
*******************************************************************************/
MCDETECTOR mcdetector_out_2D_list(char *t, char *xl, char *yl,
                  double x1, double x2, double y1, double y2,
                  long m, long n,
                  double *p0, double *p1, double *p2, char *f,
		  char *c, Coords posa, Rotation rota, char* options, int index)
{
  char xvar[CHAR_BUF_LENGTH];
  char yvar[CHAR_BUF_LENGTH];

  /* create short axes labels */
  if (xl && strlen(xl)) { strncpy(xvar, xl, CHAR_BUF_LENGTH); xvar[2]='\0'; }
  else strcpy(xvar, "x");
  if (yl && strlen(yl)) { strncpy(yvar, yl, CHAR_BUF_LENGTH); yvar[2]='\0'; }
  else strcpy(yvar, "y");

  MCDETECTOR detector;

  /* import and perform basic detector analysis (and handle MPI_Reduce) */
  if (labs(m) == 1) {/* n>1 on Y, m==1 on X: 1D, no X axis*/
    detector = detector_import(mcformat,
      c, (t ? t : MCCODE_STRING " 1D data"),
      n, 1, 1,
      yl, "", "Signal per bin",
      yvar, "(I,Ierr)", "I",
      y1, y2, x1, x2, 0, 0, f,
      p0, p1, p2, posa, rota, index); /* write Detector: line */
  } else if (labs(n)==1) {/* m>1 on X, n==1 on Y: 1D, no Y axis*/
    detector = detector_import(mcformat,
      c, (t ? t : MCCODE_STRING " 1D data"),
      m, 1, 1,
      xl, "", "Signal per bin",
      xvar, "(I,Ierr)", "I",
      x1, x2, y1, y2, 0, 0, f,
      p0, p1, p2, posa, rota, index); /* write Detector: line */
  }else {
    detector = detector_import(mcformat,
      c, (t ? t : MCCODE_STRING " 2D data"),
      m, n, 1,
      xl, yl, "Signal per bin",
      xvar, yvar, "I",
      x1, x2, y1, y2, 0, 0, f,
     p0, p1, p2, posa, rota, index); /* write Detector: line */
  }

  MPI_MASTER(
  if (strlen(options)) {
    strcpy(detector.options,options);
  } else {
    strcpy(detector.options,"None");
  }
  );

  if (!detector.p1 || !detector.m) return(detector);

#ifdef USE_NEXUS
  if (strcasestr(detector.format, "NeXus"))
    return(mcdetector_out_2D_nexus(detector));
  else
#endif
    return(mcdetector_out_2D_ascii(detector));

} /* mcdetector_out_2D_list */

/*******************************************************************************
* mcdetector_out_list: wrapper for list output (calls out_2D with mcformat+"list").
*   m=number of events, n=size of each event
*******************************************************************************/
MCDETECTOR mcdetector_out_list(char *t, char *xl, char *yl,
                  long m, long n,
                  double *p1, char *f,
			       char *c, Coords posa, Rotation rota, char* options, int index)
{
  char       format_new[CHAR_BUF_LENGTH];
  char      *format_org;
  MCDETECTOR detector;

  format_org = mcformat;
  strcpy(format_new, mcformat);
  strcat(format_new, " list");
  mcformat = format_new;
  detector = mcdetector_out_2D_list(t, xl, yl,
                  1,labs(m),1,labs(n),
                  m,n,
                  NULL, p1, NULL, f,
		  c, posa,rota,options, index);

  mcformat = format_org;
  return(detector);
}

/*******************************************************************************
 * mcuse_dir: set data/sim storage directory and create it,
 * or exit with error if exists
 ******************************************************************************/
static void
mcuse_dir(char *dir)
{
  if (!dir || !strlen(dir)) return;
#ifdef MC_PORTABLE
  fprintf(stderr, "Error: "
          "Directory output cannot be used with portable simulation (mcuse_dir)\n");
  exit(1);
#else  /* !MC_PORTABLE */
  /* handle file://directory URL type */
  if (strncmp(dir, "file://", strlen("file://")))
    dirname = dir;
  else
    dirname = dir+strlen("file://");


#ifdef USE_MPI
  if(mpi_node_rank == mpi_node_root) {
#endif
    int exists=0;
    DIR* handle = opendir(dirname);
    if (handle) {
      /* Directory exists. */
      closedir(handle);
      exists=1;
    }
    if(mkdir(dirname, 0777)) {
#ifndef DANSE
      if(!mcappend) {
	fprintf(stderr, "Error: unable to create directory '%s' (mcuse_dir)\n", dir);
	fprintf(stderr, "(Maybe the directory already exists?)\n");
#endif
#ifdef USE_MPI
	MPI_Abort(MPI_COMM_WORLD, -1);
#endif
	exit(-1);
      }
    }
#ifdef USE_MPI
    }
#endif

  /* remove trailing PATHSEP (if any) */
  while (strlen(dirname) && dirname[strlen(dirname) - 1] == MC_PATHSEP_C)
    dirname[strlen(dirname) - 1]='\0';
#endif /* !MC_PORTABLE */
} /* mcuse_dir */

/*******************************************************************************
* mcinfo: display instrument simulation info to stdout and exit
*******************************************************************************/
static void
mcinfo(void)
{
  fprintf(stdout, "begin instrument: %s\n", instrument_name);
  mcinfo_out("  ", stdout);
  fprintf(stdout, "end instrument\n");
  fprintf(stdout, "begin simulation: %s\n", dirname ? dirname : ".");
  mcruninfo_out("  ", stdout);
  fprintf(stdout, "end simulation\n");
  exit(0); /* includes MPI_Finalize in MPI mode */
} /* mcinfo */

/*******************************************************************************
* mcparameterinfo: display instrument parameter info to stdout and exit
*******************************************************************************/
static void
mcparameterinfo(void)
{
  mcparameterinfo_out("  ", stdout);
  exit(0); /* includes MPI_Finalize in MPI mode */
} /* mcparameterinfo */



#endif /* ndef MCCODE_R_IO_C */

/* end of the I/O section =================================================== */







/*******************************************************************************
* mcset_ncount: set total number of rays to generate
*******************************************************************************/
void mcset_ncount(unsigned long long int count)
{
  mcncount = count;
}

/* mcget_ncount: get total number of rays to generate */
unsigned long long int mcget_ncount(void)
{
  return mcncount;
}

/* mcget_run_num: get curent number of rays */
/* Within the TRACE scope we are now using _particle->uid directly */
unsigned long long int mcget_run_num() // shuld be (_class_particle* _particle) somehow
{
  /* This function only remains for the few cases outside TRACE where we need to know
     the number of simulated particles */
  return mcrun_num;
}

/* mcsetn_arg: get ncount from a string argument */
static void
mcsetn_arg(char *arg)
{
  mcset_ncount((long long int) strtod(arg, NULL));
}

/* mcsetseed: set the random generator seed from a string argument */
static void
mcsetseed(char *arg)
{
  mcseed = atol(arg);
  if(!mcseed) {
  //  srandom(mcseed);
  //} else {
    fprintf(stderr, "Error: seed must not be zero (mcsetseed)\n");
    exit(1);
  }
}

/* Following part is only embedded when not redundent with mccode-r.h ========= */

#ifndef MCCODE_H

/* SECTION: MCDISPLAY support. =============================================== */

/*******************************************************************************
* Just output MCDISPLAY keywords to be caught by an external plotter client.
*******************************************************************************/

void mcdis_magnify(char *what){
  // Do nothing here, better use interactive zoom from the tools
}

void mcdis_line(double x1, double y1, double z1,
                double x2, double y2, double z2){
  printf("MCDISPLAY: multiline(2,%g,%g,%g,%g,%g,%g)\n",
         x1,y1,z1,x2,y2,z2);
}

void mcdis_dashed_line(double x1, double y1, double z1,
		       double x2, double y2, double z2, int n){
  int i;
  const double dx = (x2-x1)/(2*n+1);
  const double dy = (y2-y1)/(2*n+1);
  const double dz = (z2-z1)/(2*n+1);

  for(i = 0; i < n+1; i++)
    mcdis_line(x1 + 2*i*dx,     y1 + 2*i*dy,     z1 + 2*i*dz,
	       x1 + (2*i+1)*dx, y1 + (2*i+1)*dy, z1 + (2*i+1)*dz);
}

void mcdis_multiline(int count, ...){
  va_list ap;
  double x,y,z;

  printf("MCDISPLAY: multiline(%d", count);
  va_start(ap, count);
  while(count--)
    {
    x = va_arg(ap, double);
    y = va_arg(ap, double);
    z = va_arg(ap, double);
    printf(",%g,%g,%g", x, y, z);
    }
  va_end(ap);
  printf(")\n");
}

void mcdis_rectangle(char* plane, double x, double y, double z,
		     double width, double height){
  /* draws a rectangle in the plane           */
  /* x is ALWAYS width and y is ALWAYS height */
  if (strcmp("xy", plane)==0) {
    mcdis_multiline(5,
		    x - width/2, y - height/2, z,
		    x + width/2, y - height/2, z,
		    x + width/2, y + height/2, z,
		    x - width/2, y + height/2, z,
		    x - width/2, y - height/2, z);
  } else if (strcmp("xz", plane)==0) {
    mcdis_multiline(5,
		    x - width/2, y, z - height/2,
		    x + width/2, y, z - height/2,
		    x + width/2, y, z + height/2,
		    x - width/2, y, z + height/2,
		    x - width/2, y, z - height/2);
  } else if (strcmp("yz", plane)==0) {
    mcdis_multiline(5,
		    x, y - height/2, z - width/2,
		    x, y - height/2, z + width/2,
		    x, y + height/2, z + width/2,
		    x, y + height/2, z - width/2,
		    x, y - height/2, z - width/2);
  } else {

    fprintf(stderr, "Error: Definition of plane %s unknown\n", plane);
    exit(1);
  }
}

void mcdis_circle(char *plane, double x, double y, double z, double r){
  printf("MCDISPLAY: mcdiscircle('%s',%g,%g,%g,%g)\n", plane, x, y, z, r);
}

void mcdis_new_circle(double x, double y, double z, double r, double nx, double ny, double nz){
  printf("MCDISPLAY: mcdisnew_circle(%g,%g,%g,%g,%g,%g,%g)\n", x, y, z, r, nx, ny, nz);
}


/* Draws a circle with center (x,y,z), radius (r), and in the plane
 * with normal (nx,ny,nz)*/
void mcdis_Circle(double x, double y, double z, double r, double nx, double ny, double nz){
    int i;
    if(nx==0 && ny && nz==0){
        for (i=0;i<24; i++){
            mcdis_line(x+r*sin(i*2*PI/24),y,z+r*cos(i*2*PI/24),
                    x+r*sin((i+1)*2*PI/24),y,z+r*cos((i+1)*2*PI/24));
        }
    }else{
        double mx,my,mz;
        /*generate perpendicular vector using (nx,ny,nz) and (0,1,0)*/
        vec_prod(mx,my,mz, 0,1,0, nx,ny,nz);
        NORM(mx,my,mz);
        /*draw circle*/
        for (i=0;i<24; i++){
            double ux,uy,uz;
            double wx,wy,wz;
            rotate(ux,uy,uz, mx,my,mz, i*2*PI/24, nx,ny,nz);
            rotate(wx,wy,wz, mx,my,mz, (i+1)*2*PI/24, nx,ny,nz);
            mcdis_line(x+ux*r,y+uy*r,z+uz*r,
                    x+wx*r,y+wy*r,z+wz*r);
        }
    }
}


/*  OLD IMPLEMENTATION
    draws a box with center at (x, y, z) and
    width (deltax), height (deltay), length (deltaz) */
void mcdis_legacy_box(double x, double y, double z,
	       double width, double height, double length){

  mcdis_rectangle("xy", x, y, z-length/2, width, height);
  mcdis_rectangle("xy", x, y, z+length/2, width, height);
  mcdis_line(x-width/2, y-height/2, z-length/2,
	     x-width/2, y-height/2, z+length/2);
  mcdis_line(x-width/2, y+height/2, z-length/2,
	     x-width/2, y+height/2, z+length/2);
  mcdis_line(x+width/2, y-height/2, z-length/2,
	     x+width/2, y-height/2, z+length/2);
  mcdis_line(x+width/2, y+height/2, z-length/2,
	     x+width/2, y+height/2, z+length/2);
}

/*  NEW 3D IMPLEMENTATION OF BOX SUPPORTS HOLLOW ALSO
    draws a box with center at (x, y, z) and
    width (deltax), height (deltay), length (deltaz) */
void mcdis_box(double x, double y, double z,
	       double width, double height, double length, double thickness, double nx, double ny, double nz){
  if (mcdotrace==2) {
    printf("MCDISPLAY: mcdisbox(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g)\n", x, y, z, width, height, length, thickness, nx, ny, nz);
  } else {
    mcdis_legacy_box(x, y, z, width, height, length);
    if (thickness)
      mcdis_legacy_box(x, y, z, width-thickness, height-thickness, length);
  }
}


/* OLD IMPLEMENTATION
Draws a cylinder with center at (x,y,z) with extent (r,height).
 * The cylinder axis is along the vector nx,ny,nz. */
void mcdis_legacy_cylinder( double x, double y, double z,
        double r, double height, int N, double nx, double ny, double nz){
    int i;
    /*no lines make little sense - so trigger the default*/
    if(N<=0) N=5;

    NORM(nx,ny,nz);
    double h_2=height/2.0;
    mcdis_Circle(x+nx*h_2,y+ny*h_2,z+nz*h_2,r,nx,ny,nz);
    mcdis_Circle(x-nx*h_2,y-ny*h_2,z-nz*h_2,r,nx,ny,nz);

    double mx,my,mz;
    /*generate perpendicular vector using (nx,ny,nz) and (0,1,0)*/
    if(nx==0 && ny && nz==0){
        mx=my=0;mz=1;
    }else{
        vec_prod(mx,my,mz, 0,1,0, nx,ny,nz);
        NORM(mx,my,mz);
    }
    /*draw circle*/
    for (i=0; i<24; i++){
        double ux,uy,uz;
        rotate(ux,uy,uz, mx,my,mz, i*2*PI/24, nx,ny,nz);
        mcdis_line(x+nx*h_2+ux*r, y+ny*h_2+uy*r, z+nz*h_2+uz*r,
                 x-nx*h_2+ux*r, y-ny*h_2+uy*r, z-nz*h_2+uz*r);
    }
}

/* NEW 3D IMPLEMENTATION ALSO SUPPORTING HOLLOW
Draws a cylinder with center at (x,y,z) with extent (r,height).
 * The cylinder axis is along the vector nx,ny,nz.*/
void mcdis_cylinder( double x, double y, double z,
        double r, double height, double thickness, double nx, double ny, double nz){
  if (mcdotrace==2) {
      printf("MCDISPLAY: mcdiscylinder(%g, %g, %g, %g, %g, %g, %g, %g, %g)\n",
         x, y, z, r, height, thickness, nx, ny, nz);
  } else {
    mcdis_legacy_cylinder(x, y, z,
			  r, height, 12, nx, ny, nz);
  }
}

/* Draws a cone with center at (x,y,z) with extent (r,height).
 * The cone axis is along the vector nx,ny,nz.*/
void mcdis_cone( double x, double y, double z,
        double r, double height, double nx, double ny, double nz){
  if (mcdotrace==2) {
    printf("MCDISPLAY: mcdiscone(%g, %g, %g, %g, %g, %g, %g, %g)\n",
       x, y, z, r, height, nx, ny, nz);
  } else {
    mcdis_Circle(x, y, z, r, nx, ny, nz);
    mcdis_Circle(x+0.25*height*nx, y+0.25*height*ny, z+0.25*height*nz, 0.75*r, nx, ny, nz);
    mcdis_Circle(x+0.5*height*nx, y+0.5*height*ny, z+0.5*height*nz, 0.5*r, nx, ny, nz);
    mcdis_Circle(x+0.75*height*nx, y+0.75*height*ny, z+0.75*height*nz, 0.25*r, nx, ny, nz);
    mcdis_line(x, y, z, x+height*nx, y+height*ny, z+height*nz);
  }
}

/* Draws a disc with center at (x,y,z) with extent (r).
 * The disc axis is along the vector nx,ny,nz.*/
void mcdis_disc( double x, double y, double z,
        double r, double nx, double ny, double nz){
  printf("MCDISPLAY: mcdisdisc(%g, %g, %g, %g, %g, %g, %g)\n",
     x, y, z, r, nx, ny, nz);
}

/* Draws a annulus with center at (x,y,z) with extent (outer_radius) and remove inner_radius.
 * The annulus axis is along the vector nx,ny,nz.*/
void mcdis_annulus( double x, double y, double z,
        double outer_radius, double inner_radius, double nx, double ny, double nz){
  printf("MCDISPLAY: mcdisannulus(%g, %g, %g, %g, %g, %g, %g, %g)\n",
     x, y, z, outer_radius, inner_radius, nx, ny, nz);
}

/* draws a sphere with center at (x,y,z) with extent (r)*/
void mcdis_sphere(double x, double y, double z, double r){
  if (mcdotrace==2) {
    printf("MCDISPLAY: mcdissphere(%g,%g,%g,%g)\n", x, y, z, r);
  } else {
    double nx,ny,nz;
    int i;
    int N=12;

    nx=0;ny=0;nz=1;
    mcdis_Circle(x,y,z,r,nx,ny,nz);
    for (i=1;i<N;i++){
        rotate(nx,ny,nz, nx,ny,nz, PI/N, 0,1,0);
        mcdis_Circle(x,y,z,r,nx,ny,nz);
    }
    /*lastly draw a great circle perpendicular to all N circles*/
    //mcdis_Circle(x,y,z,radius,1,0,0);

    for (i=1;i<=N;i++){
        double yy=-r+ 2*r*((double)i/(N+1));
        mcdis_Circle(x,y+yy ,z,  sqrt(r*r-yy*yy) ,0,1,0);
    }
  }
}
/* POLYHEDRON IMPLEMENTATION*/

void mcdis_polyhedron(char *vertices_faces){
  printf("MCDISPLAY: polyhedron %s\n", vertices_faces);
}

/* POLYGON IMPLEMENTATION */
void mcdis_polygon(int count, ...){
  va_list ap;
  double *x,*y,*z;

  double x0=0,y0=0,z0=0; /* Used for centre-of-mass in trace==2 */

  x=malloc(count*sizeof(double));
  y=malloc(count*sizeof(double));
  z=malloc(count*sizeof(double));
  if (!x || !y || !z) {
    fprintf(stderr,"Error initializing polygon set size %i\n",count);
    exit(-1);
  }
  va_start(ap, count);
  // Fallback for trace==1 is multiline, one rank higher
  if (mcdotrace==1) {
    printf("MCDISPLAY: multiline(%i,",count+1);
  }
  
  int j;
  for (j=0; j<count; j++) {
    x[j] = va_arg(ap, double);
    y[j] = va_arg(ap, double);
    z[j] = va_arg(ap, double);
    if (mcdotrace==1) {
      printf("%g,%g,%g,",x[j],y[j],z[j]);
    } else {
      // Calculation of polygon centre of mass
      x0 += x[j]; y0 += y[j]; z0 += z[j];
    }
  }
  va_end(ap);

  /* Patch data for multiline(count+1, ... use 0th point*/
  if (mcdotrace==1) {
    printf("%g,%g,%g)\n",x[0],y[0],z[0]);
  } else {
    x0 /= count; y0 /= count; z0 /= count;
    /* Build up a json string for a "polyhedron" */
    // Estimate size of the JSON string
    const int VERTEX_OVERHEAD = 30;
    const int FACE_OVERHEAD_BASE = 20;
    const int FACE_INDEX_OVERHEAD = 15;
    int estimated_size = 256; // Base size
    estimated_size += count * VERTEX_OVERHEAD;

    int faceSize;
    int vtxSize;
    if (count > 3) {
      /* Split in triangles - as many as polygon rank */
      faceSize=count;
      vtxSize=count+1;
    } else {
      faceSize=1;
      vtxSize=count;
    }
    
    for (int i = 0; i < faceSize;) {
        int num_indices = 3;
        estimated_size += FACE_OVERHEAD_BASE + num_indices * FACE_INDEX_OVERHEAD;
        i += num_indices + 1;
    }

    char *json_string = malloc(estimated_size);
    if (json_string == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    char *ptr = json_string;
    ptr += sprintf(ptr, "{ \"vertices\": [");

    if (count==3) { // Single, basic triangle
      ptr += sprintf(ptr, "[%g, %g, %g], [%g, %g, %g], [%g, %g, %g]", x[0], y[0], z[0], x[1], y[1], z[1], x[2], y[2], z[2]);
    } else {
      for (int i = 0; i < vtxSize-1; i++) {
        ptr += sprintf(ptr, "[%g, %g, %g]", x[i], y[i], z[i]);
        if (i < vtxSize - 2) {
	  ptr += sprintf(ptr, ", ");
        } else {
	  ptr += sprintf(ptr, ", [%g, %g, %g]", x0, y0, z0);
	}
      }
    }
    ptr += sprintf(ptr, "], \"faces\": [");
    if (count==3) { // Single, basic triangle, 1 face...
      ptr += sprintf(ptr, "{ \"face\": [");
      ptr += sprintf(ptr, "0, 1, 2");
      ptr += sprintf(ptr, "]}");
    } else {
      for (int i = 0; i < faceSize; i++) {
        int num = 3;
        ptr += sprintf(ptr, "{ \"face\": [");
	if (i < faceSize - 1) {
	  ptr += sprintf(ptr, "%d, %d, %d",i,i+1,count);
	} else {
	  ptr += sprintf(ptr, "%d, %d, %d",i,count,0);
	}
	ptr += sprintf(ptr, "]}");
	if (i < faceSize-1) {
	  ptr += sprintf(ptr, ", ");
	}
      }
    }
    ptr += sprintf(ptr, "]}");
    mcdis_polyhedron(json_string);

    free(json_string);
  }
  free(x);free(y);free(z);
}
/* END NEW POLYGON IMPLEMENTATION*/

/*
void polygon(double x1, double y1, double z1,
                double x2, double y2, double z2){
  printf("MCDISPLAY: polygon(2,%g,%g,%g,%g,%g,%g)\n",
         x1,y1,z1,x2,y2,z2);
}
*/

/* SECTION: coordinates handling ============================================ */

/*******************************************************************************
* Since we use a lot of geometric calculations using Cartesian coordinates,
* we collect some useful routines here. However, it is also permissible to
* work directly on the underlying struct coords whenever that is most
* convenient (that is, the type Coords is not abstract).
*
* Coordinates are also used to store rotation angles around x/y/z axis.
*
* Since coordinates are used much like a basic type (such as double), the
* structure itself is passed and returned, rather than a pointer.
*
* At compile-time, the values of the coordinates may be unknown (for example
* a motor position). Hence coordinates are general expressions and not simple
* numbers. For this we used the type Coords_exp which has three CExp
* fields. For runtime (or calculations possible at compile time), we use
* Coords which contains three double fields.
*******************************************************************************/

/* coords_set: Assign coordinates. */
Coords coords_set(MCNUM x, MCNUM y, MCNUM z)
{
  Coords a;

  a.x = x;
  a.y = y;
  a.z = z;
  return a;
}

/* coords_get: get coordinates. Required when 'x','y','z' are #defined as ray pars */
Coords coords_get(Coords a, MCNUM *x, MCNUM *y, MCNUM *z)
{
  *x = a.x;
  *y = a.y;
  *z = a.z;
  return a;
}

/* coords_add: Add two coordinates. */
Coords coords_add(Coords a, Coords b)
{
  Coords c;

  c.x = a.x + b.x;
  c.y = a.y + b.y;
  c.z = a.z + b.z;
  if (fabs(c.z) < 1e-14) c.z=0.0;
  return c;
}

/* coords_sub: Subtract two coordinates. */
Coords coords_sub(Coords a, Coords b)
{
  Coords c;

  c.x = a.x - b.x;
  c.y = a.y - b.y;
  c.z = a.z - b.z;
  if (fabs(c.z) < 1e-14) c.z=0.0;
  return c;
}

/* coords_neg: Negate coordinates. */
Coords coords_neg(Coords a)
{
  Coords b;

  b.x = -a.x;
  b.y = -a.y;
  b.z = -a.z;
  return b;
}

/* coords_scale: Scale a vector. */
Coords coords_scale(Coords b, double scale) {
  Coords a;

  a.x = b.x*scale;
  a.y = b.y*scale;
  a.z = b.z*scale;
  return a;
}

/* coords_sp: Scalar product: a . b */
double coords_sp(Coords a, Coords b) {
  double value;

  value = a.x*b.x + a.y*b.y + a.z*b.z;
  return value;
}

/* coords_xp: Cross product: a = b x c. */
Coords coords_xp(Coords b, Coords c) {
  Coords a;

  a.x = b.y*c.z - c.y*b.z;
  a.y = b.z*c.x - c.z*b.x;
  a.z = b.x*c.y - c.x*b.y;
  return a;
}

/* coords_len: Gives length of coords set. */
double coords_len(Coords a) {
  return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

/* coords_mirror: Mirror a in plane (through the origin) defined by normal n*/
Coords coords_mirror(Coords a, Coords n) {
  double t = scalar_prod(n.x, n.y, n.z, n.x, n.y, n.z);
  Coords b;
  if (t!=1) {
    t = sqrt(t);
    n.x /= t;
    n.y /= t;
    n.z /= t;
  }
  t=scalar_prod(a.x, a.y, a.z, n.x, n.y, n.z);
  b.x = a.x-2*t*n.x;
  b.y = a.y-2*t*n.y;
  b.z = a.z-2*t*n.z;
  return b;
}

/* coords_print: Print out vector values. */
void coords_print(Coords a) {
  #ifndef OPENACC
  fprintf(stdout, "(%f, %f, %f)\n", a.x, a.y, a.z);
  #endif
  return;
}

mcstatic void coords_norm(Coords* c) {
	double temp = coords_sp(*c,*c);

	// Skip if we will end dividing by zero
	if (temp == 0) return;

	temp = sqrt(temp);

	c->x /= temp;
	c->y /= temp;
	c->z /= temp;
}

/* coords_test_zero: check if zero vector*/
int coords_test_zero(Coords a){
  return ( a.x==0 && a.y==0 && a.z==0 );
}

/*******************************************************************************
* The Rotation type implements a rotation transformation of a coordinate
* system in the form of a double[3][3] matrix.
*
* Contrary to the Coords type in coords.c, rotations are passed by
* reference. Functions that yield new rotations do so by writing to an
* explicit result parameter; rotations are not returned from functions. The
* reason for this is that arrays cannot by returned from functions (though
* structures can; thus an alternative would have been to wrap the
* double[3][3] array up in a struct). Such are the ways of C programming.
*
* A rotation represents the tranformation of the coordinates of a vector when
* changing between coordinate systems that are rotated with respect to each
* other. For example, suppose that coordinate system Q is rotated 45 degrees
* around the Z axis with respect to coordinate system P. Let T be the
* rotation transformation representing a 45 degree rotation around Z. Then to
* get the coordinates of a vector r in system Q, apply T to the coordinates
* of r in P. If r=(1,0,0) in P, it will be (sqrt(1/2),-sqrt(1/2),0) in
* Q. Thus we should be careful when interpreting the sign of rotation angles:
* they represent the rotation of the coordinate systems, not of the
* coordinates (which has opposite sign).
*******************************************************************************/

/*******************************************************************************
* rot_set_rotation: Get transformation for rotation first phx around x axis,
* then phy around y, then phz around z.
*******************************************************************************/
void rot_set_rotation(Rotation t, double phx, double phy, double phz)
{
  if ((phx == 0) && (phy == 0) && (phz == 0)) {
    t[0][0] = 1.0;
    t[0][1] = 0.0;
    t[0][2] = 0.0;
    t[1][0] = 0.0;
    t[1][1] = 1.0;
    t[1][2] = 0.0;
    t[2][0] = 0.0;
    t[2][1] = 0.0;
    t[2][2] = 1.0;
  } else {
    double cx = cos(phx);
    double sx = sin(phx);
    double cy = cos(phy);
    double sy = sin(phy);
    double cz = cos(phz);
    double sz = sin(phz);

    t[0][0] = cy*cz;
    t[0][1] = sx*sy*cz + cx*sz;
    t[0][2] = sx*sz - cx*sy*cz;
    t[1][0] = -cy*sz;
    t[1][1] = cx*cz - sx*sy*sz;
    t[1][2] = sx*cz + cx*sy*sz;
    t[2][0] = sy;
    t[2][1] = -sx*cy;
    t[2][2] = cx*cy;
  }
}

/*******************************************************************************
* rot_test_identity: Test if rotation is identity
*******************************************************************************/
int rot_test_identity(Rotation t)
{
  return (t[0][0] + t[1][1] + t[2][2] == 3);
}

/*******************************************************************************
* rot_mul: Matrix multiplication of transformations (this corresponds to
* combining transformations). After rot_mul(T1, T2, T3), doing T3 is
* equal to doing first T2, then T1.
* Note that T3 must not alias (use the same array as) T1 or T2.
*******************************************************************************/
void rot_mul(Rotation t1, Rotation t2, Rotation t3)
{
  if (rot_test_identity(t1)) {
    rot_copy(t3, t2);
  } else if (rot_test_identity(t2)) {
    rot_copy(t3, t1);
  } else {
    int i,j;
    for(i = 0; i < 3; i++)
      for(j = 0; j < 3; j++)
	t3[i][j] = t1[i][0]*t2[0][j] + t1[i][1]*t2[1][j] + t1[i][2]*t2[2][j];
  }
}

/*******************************************************************************
* rot_copy: Copy a rotation transformation (arrays cannot be assigned in C).
*******************************************************************************/
void rot_copy(Rotation dest, Rotation src)
{
  int i,j;
  for(i = 0; i < 3; i++)
    for(j = 0; j < 3; j++)
      dest[i][j] = src[i][j];
}

/*******************************************************************************
* rot_transpose: Matrix transposition, which is inversion for Rotation matrices
*******************************************************************************/
void rot_transpose(Rotation src, Rotation dst)
{
  dst[0][0] = src[0][0];
  dst[0][1] = src[1][0];
  dst[0][2] = src[2][0];
  dst[1][0] = src[0][1];
  dst[1][1] = src[1][1];
  dst[1][2] = src[2][1];
  dst[2][0] = src[0][2];
  dst[2][1] = src[1][2];
  dst[2][2] = src[2][2];
}

/*******************************************************************************
* rot_apply: returns t*a
*******************************************************************************/
Coords rot_apply(Rotation t, Coords a)
{
  Coords b;
  if (rot_test_identity(t)) {
    return a;
  } else {
    b.x = t[0][0]*a.x + t[0][1]*a.y + t[0][2]*a.z;
    b.y = t[1][0]*a.x + t[1][1]*a.y + t[1][2]*a.z;
    b.z = t[2][0]*a.x + t[2][1]*a.y + t[2][2]*a.z;
    return b;
  }
}

/**
 * Pretty-printing of rotation matrices.
 */
void rot_print(Rotation rot) {
	printf("[ %4.2f %4.2f %4.2f ]\n",
			rot[0][0], rot[0][1], rot[0][2]);
	printf("[ %4.2f %4.2f %4.2f ]\n",
			rot[1][0], rot[1][1], rot[1][2]);
	printf("[ %4.2f %4.2f %4.2f ]\n\n",
			rot[2][0], rot[2][1], rot[2][2]);
}

/**
 * Vector product: used by vec_prod (mccode-r.h). Use coords_xp for Coords.
 */
void vec_prod_func(double *x, double *y, double *z,
		double x1, double y1, double z1,
		double x2, double y2, double z2) {
    *x = (y1)*(z2) - (y2)*(z1);
    *y = (z1)*(x2) - (z2)*(x1);
    *z = (x1)*(y2) - (x2)*(y1);
}

/**
 * Scalar product: use coords_sp for Coords.
 */
double scalar_prod(
		double x1, double y1, double z1,
		double x2, double y2, double z2) {
	return ((x1 * x2) + (y1 * y2) + (z1 * z2));
}

mcstatic void norm_func(double *x, double *y, double *z) {
	double temp = (*x * *x) + (*y * *y) + (*z * *z);
	if (temp != 0) {
		temp = sqrt(temp);
		*x /= temp;
		*y /= temp;
		*z /= temp;
	}
}


/* SECTION: GPU algorithms ================================================== */


/*
*  Divide-and-conquer strategy for parallelizing this task: Sort absorbed
*  particles last.
*
*   particles:  the particle array, required to checking _absorbed
*   pbuffer:    same-size particle buffer array required for parallel sort
*   len:        sorting area-of-interest size (e.g. from previous calls)
*   buffer_len: total array size
*   flag_split: if set, multiply live particles into absorbed slots, up to buffer_len
*   multiplier: output arg, becomes the  SPLIT multiplier if flag_split is set
*/
#ifdef FUNNEL
long sort_absorb_last(_class_particle* particles, _class_particle* pbuffer, long len, long buffer_len, long flag_split, long* multiplier) {
  #define SAL_THREADS 1024 // num parallel sections
  if (len<SAL_THREADS) return sort_absorb_last_serial(particles, len);

  if (multiplier != NULL) *multiplier = -1; // set default out value for multiplier
  long newlen = 0;
  long los[SAL_THREADS]; // target array startidxs
  long lens[SAL_THREADS]; // target array sublens
  long l = floor(len/(SAL_THREADS-1)); // subproblem_len
  long ll = len - l*(SAL_THREADS-1); // last_subproblem_len

  // TODO: The l vs ll is too simplistic, since ll can become much larger
  // than l, resulting in idling. We should distribute lengths more evenly.

  // step 1: sort sub-arrays
  #pragma acc parallel loop present(particles[0:buffer_len], pbuffer[0:buffer_len])
  for (unsigned long tidx=0; tidx<SAL_THREADS; tidx++) {
    long lo = l*tidx;
    long loclen = l;
    if (tidx==(SAL_THREADS-1)) loclen = ll; // last sub-problem special case
    long i = lo;
    long j = lo + loclen - 1;

    // write into pbuffer at i and j
    #pragma acc loop seq
    while (i < j) {
      #pragma acc loop seq
      while (!particles[i]._absorbed && i<j) {
        pbuffer[i] = particles[i];
        i++;
      }
      #pragma acc loop seq
      while (particles[j]._absorbed && i<j) {
        pbuffer[j] = particles[j];
        j--;
      }
      if (i < j) {
        pbuffer[j] = particles[i];
        pbuffer[i] = particles[j];
        i++;
        j--;
      }
    }
    // transfer edge case
    if (i==j)
      pbuffer[i] = particles[i];

    lens[tidx] = i - lo;
    if (i==j && !particles[i]._absorbed) lens[tidx]++;
  }

  // determine lo's
  long accumlen = 0;
  #pragma acc loop seq
  for (long idx=0; idx<SAL_THREADS; idx++) {
    los[idx] = accumlen;
    accumlen = accumlen + lens[idx];
  }

  // step 2: write non-absorbed sub-arrays to psorted/output from the left
  #pragma acc parallel loop present(pbuffer[0:buffer_len])
  for (unsigned long tidx=0; tidx<SAL_THREADS; tidx++) {
    long j, k;
    #pragma acc loop seq
    for (long i=0; i<lens[tidx]; i++) {
      j = i + l*tidx;
      k = i + los[tidx];
      particles[k] = pbuffer[j];
    }
  }
  //for (int ii=0;ii<accumlen;ii++) printf("%ld ", (psorted[ii]->_absorbed));

  // return (no SPLIT)
  if (flag_split != 1)
    return accumlen;

  // SPLIT - repeat the non-absorbed block N-1 times, where len % accumlen = N + R
  int mult = buffer_len / accumlen; // TODO: possibly use a new arg, bufferlen, rather than len

  // not enough space for full-block split, return
  if (mult <= 1)
    return accumlen;

  // copy non-absorbed block
  #pragma acc parallel loop present(particles[0:buffer_len])
  for (long tidx = 0; tidx < accumlen; tidx++) { // tidx: thread index
    randstate_t randstate[7];
    _class_particle sourcebuffer;
    _class_particle targetbuffer;
    // assign reduced weight to all particles
    particles[tidx].p=particles[tidx].p/mult;
    #pragma acc loop seq
    for (long bidx = 1; bidx < mult; bidx++) { // bidx: block index
      // preserve absorbed particle (for randstate)
      sourcebuffer = particles[bidx*accumlen + tidx];
      // buffer full particle struct
      targetbuffer = particles[tidx];
      // reassign previous randstate
      targetbuffer.randstate[0] = sourcebuffer.randstate[0];
      targetbuffer.randstate[1] = sourcebuffer.randstate[1];
      targetbuffer.randstate[2] = sourcebuffer.randstate[2];
      targetbuffer.randstate[3] = sourcebuffer.randstate[3];
      targetbuffer.randstate[4] = sourcebuffer.randstate[4];
      targetbuffer.randstate[5] = sourcebuffer.randstate[5];
      targetbuffer.randstate[6] = sourcebuffer.randstate[6];
      // apply
      particles[bidx*accumlen + tidx] = targetbuffer;
    }
  }

  // set out split multiplier value
  *multiplier = mult;

  // return expanded array size
  return accumlen * mult;
}

#endif

/*
*  Fallback serial version of the one above.
*/
long sort_absorb_last_serial(_class_particle* particles, long len) {
  long i = 0;
  long j = len - 1;
  _class_particle pbuffer;

  // bubble
  while (i < j) {
    while (!particles[i]._absorbed && i<j) i++;
    while (particles[j]._absorbed && i<j) j--;
    if (i < j) {
      pbuffer = particles[j];
      particles[j] = particles[i];
      particles[i] = pbuffer;
      i++;
      j--;
    }
  }

  // return new length
  if (i==j && !particles[i]._absorbed)
    return i + 1;
  else
    return i;
}

/*******************************************************************************
* mccoordschange: applies rotation to (x y z) and (vx vy vz) and Spin (sx,sy,sz)
*******************************************************************************/
void mccoordschange(Coords a, Rotation t, _class_particle *particle)
{
  Coords b, c;

  b.x = particle->x;
  b.y = particle->y;
  b.z = particle->z;
  c = rot_apply(t, b);
  b = coords_add(c, a);
  particle->x = b.x;
  particle->y = b.y;
  particle->z = b.z;

#if MCCODE_PARTICLE_CODE == 2112
    if (particle->vz != 0.0 || particle->vx != 0.0 || particle->vy != 0.0)
      mccoordschange_polarisation(t, &(particle->vx), &(particle->vy), &(particle->vz));

    if (particle->sz != 0.0 || particle->sx != 0.0 || particle->sy != 0.0)
      mccoordschange_polarisation(t, &(particle->sx), &(particle->sy), &(particle->sz));
#elif MCCODE_PARTICLE_CODE == 22
    if (particle->kz != 0.0 || particle->kx != 0.0 || particle->ky != 0.0)
      mccoordschange_polarisation(t, &(particle->kx), &(particle->ky), &(particle->kz));

    if (particle->Ez != 0.0 || particle->Ex != 0.0 || particle->Ey != 0.0)
      mccoordschange_polarisation(t, &(particle->Ex), &(particle->Ey), &(particle->Ez));
#endif
}

/*******************************************************************************
* mccoordschange_polarisation: applies rotation to vector (sx sy sz)
*******************************************************************************/
void mccoordschange_polarisation(Rotation t, double *sx, double *sy, double *sz)
{
  Coords b, c;

  b.x = *sx;
  b.y = *sy;
  b.z = *sz;
  c = rot_apply(t, b);
  *sx = c.x;
  *sy = c.y;
  *sz = c.z;
}

/* SECTION: vector math  ==================================================== */

/* normal_vec_func: Compute normal vector to (x,y,z). */
void normal_vec(double *nx, double *ny, double *nz,
                double x, double y, double z)
{
  double ax = fabs(x);
  double ay = fabs(y);
  double az = fabs(z);
  double l;
  if(x == 0 && y == 0 && z == 0)
  {
    *nx = 0;
    *ny = 0;
    *nz = 0;
    return;
  }
  if(ax < ay)
  {
    if(ax < az)
    {                           /* Use X axis */
      l = sqrt(z*z + y*y);
      *nx = 0;
      *ny = z/l;
      *nz = -y/l;
      return;
    }
  }
  else
  {
    if(ay < az)
    {                           /* Use Y axis */
      l = sqrt(z*z + x*x);
      *nx = z/l;
      *ny = 0;
      *nz = -x/l;
      return;
    }
  }
  /* Use Z axis */
  l = sqrt(y*y + x*x);
  *nx = y/l;
  *ny = -x/l;
  *nz = 0;
} /* normal_vec */

/*******************************************************************************
 * solve_2nd_order: second order equation solve: A*t^2 + B*t + C = 0
 * solve_2nd_order(&t1, NULL, A,B,C)
 *   returns 0 if no solution was found, or set 't1' to the smallest positive
 *   solution.
 * solve_2nd_order(&t1, &t2, A,B,C)
 *   same as with &t2=NULL, but also returns the second solution.
 * EXAMPLE usage for intersection of a trajectory with a plane in gravitation
 * field (gx,gy,gz):
 * The neutron starts at point r=(x,y,z) with velocityv=(vx vy vz). The plane
 * has a normal vector n=(nx,ny,nz) and contains the point W=(wx,wy,wz).
 * The problem consists in solving the 2nd order equation:
 *      1/2.n.g.t^2 + n.v.t + n.(r-W) = 0
 * so that A = 0.5 n.g; B = n.v; C = n.(r-W);
 * Without acceleration, t=-n.(r-W)/n.v
 ******************************************************************************/
int solve_2nd_order_old(double *t1, double *t2,
                  double A,  double B,  double C)
{
  int ret=0;

  if (!t1) return 0;
  *t1 = 0;
  if (t2) *t2=0;

  if (fabs(A) < 1E-10) /* approximate to linear equation: A ~ 0 */
  {
    if (B) {  *t1 = -C/B; ret=1; if (t2) *t2=*t1; }
    /* else no intersection: A=B=0 ret=0 */
  }
  else
  {
    double D;
    D = B*B - 4*A*C;
    if (D >= 0) /* Delta > 0: two solutions */
    {
      double sD, dt1, dt2;
      sD = sqrt(D);
      dt1 = (-B + sD)/2/A;
      dt2 = (-B - sD)/2/A;
      /* we identify very small values with zero */
      if (fabs(dt1) < 1e-10) dt1=0.0;
      if (fabs(dt2) < 1e-10) dt2=0.0;

      /* now we choose the smallest positive solution */
      if      (dt1<=0.0 && dt2>0.0) ret=2; /* dt2 positive */
      else if (dt2<=0.0 && dt1>0.0) ret=1; /* dt1 positive */
      else if (dt1> 0.0 && dt2>0.0)
      {  if (dt1 < dt2) ret=1; else ret=2; } /* all positive: min(dt1,dt2) */
      /* else two solutions are negative. ret=-1 */
      if (ret==1) { *t1 = dt1;  if (t2) *t2=dt2; }
      else        { *t1 = dt2;  if (t2) *t2=dt1; }
      ret=2;  /* found 2 solutions and t1 is the positive one */
    } /* else Delta <0: no intersection. ret=0 */
  }
  return(ret);
} /* solve_2nd_order */

int solve_2nd_order(double *t0, double *t1, double A, double B, double C){
  int retval=0;
  double sign=copysign(1.0,B);
  double dt0,dt1;

  dt0=0;
  dt1=0;
  if(t1){ *t1=0;}

  /*protect against rounding errors by locally equating DBL_EPSILON with 0*/
  if (fabs(A)<DBL_EPSILON){
    A=0;
  }
  if (fabs(B)<DBL_EPSILON){
    B=0;
  }
  if (fabs(C)<DBL_EPSILON){
    C=0;
  }

  /*check if coefficient are sane*/
  if( A==0  && B==0){
    retval=0;
  }else{
    if(A==0){
      /*equation is linear*/
      dt0=-C/B;
      retval=1;
    }else if (C==0){
      /*one root is 0*/
      if(sign<0){
        dt0=0;dt1=-B/A;
      }else{
        dt0=-B/A;dt1=0;
      }
      retval=2;
    }else{
      /*a regular 2nd order eq. Also works out fine for B==0.*/
      double D;
      D=B*B-4*A*C;
      if (D>=0){
        dt0=(-B - sign*sqrt(B*B-4*A*C))/(2*A);
        dt1=C/(A*dt0);
        retval=2;
      }else{
        /*no real roots*/
        retval=0;
      }
    }
    /*sort the solutions*/
    if (retval==1){
      /*put both solutions in t0 and t1*/
      *t0=dt0;
      if(t1) *t1=dt1;
    }else{
      /*we have two solutions*/
      /*swap if both are positive and t1 smaller than t0 or t1 the only positive*/
      int swap=0;
      if(dt1>0 && ( dt1<dt0 || dt0<=0) ){
        swap=1;
      }
      if (swap){
        *t0=dt1;
        if(t1) *t1=dt0;
      }else{
        *t0=dt0;
        if(t1) *t1=dt0;
      }
    }

  }
  return retval;

} /*solve_2nd_order_improved*/


/*******************************************************************************
 * randvec_target_circle: Choose random direction towards target at (x,y,z)
 * with given radius.
 * If radius is zero, choose random direction in full 4PI, no target.
 ******************************************************************************/
void _randvec_target_circle(double *xo, double *yo, double *zo, double *solid_angle,
        double xi, double yi, double zi, double radius,
        _class_particle* _particle)
{
  double l2, phi, theta, nx, ny, nz, xt, yt, zt, xu, yu, zu;

  if(radius == 0.0)
  {
    /* No target, choose uniformly a direction in full 4PI solid angle. */
    theta = acos(1 - rand0max(2));
    phi = rand0max(2 * PI);
    if(solid_angle)
      *solid_angle = 4*PI;
    nx = 1;
    ny = 0;
    nz = 0;
    yi = sqrt(xi*xi+yi*yi+zi*zi);
    zi = 0;
    xi = 0;
  }
  else
  {
    double costheta0;
    l2 = xi*xi + yi*yi + zi*zi; /* sqr Distance to target. */
    costheta0 = sqrt(l2/(radius*radius+l2));
    if (radius < 0) costheta0 *= -1;
    if(solid_angle)
    {
      /* Compute solid angle of target as seen from origin. */
        *solid_angle = 2*PI*(1 - costheta0);
    }

    /* Now choose point uniformly on circle surface within angle theta0 */
    theta = acos (1 - rand0max(1 - costheta0)); /* radius on circle */
    phi = rand0max(2 * PI); /* rotation on circle at given radius */
    /* Now, to obtain the desired vector rotate (xi,yi,zi) angle theta around a
       perpendicular axis u=i x n and then angle phi around i. */
    if(xi == 0 && zi == 0)
    {
      nx = 1;
      ny = 0;
      nz = 0;
    }
    else
    {
      nx = -zi;
      nz = xi;
      ny = 0;
    }
  }

  /* [xyz]u = [xyz]i x n[xyz] (usually vertical) */
  vec_prod(xu,  yu,  zu, xi, yi, zi,        nx, ny, nz);
  /* [xyz]t = [xyz]i rotated theta around [xyz]u */
  rotate  (xt,  yt,  zt, xi, yi, zi, theta, xu, yu, zu);
  /* [xyz]o = [xyz]t rotated phi around n[xyz] */
  rotate (*xo, *yo, *zo, xt, yt, zt, phi, xi, yi, zi);
}
/* randvec_target_circle */

/*******************************************************************************
 * randvec_target_rect_angular: Choose random direction towards target at
 * (xi,yi,zi) with given ANGULAR dimension height x width. height=phi_x=[0,PI],
 * width=phi_y=[0,2*PI] (radians)
 * If height or width is zero, choose random direction in full 4PI, no target.
 *******************************************************************************/
void _randvec_target_rect_angular(double *xo, double *yo, double *zo, double *solid_angle,
        double xi, double yi, double zi, double width, double height, Rotation A,
        _class_particle* _particle)
{
  double theta, phi, nx, ny, nz, xt, yt, zt, xu, yu, zu;
  Coords tmp;
  Rotation Ainverse;

  rot_transpose(A, Ainverse);

  if(height == 0.0 || width == 0.0)
  {
    randvec_target_circle(xo, yo, zo, solid_angle, xi, yi, zi, 0);
    return;
  }
  else
  {
    if(solid_angle)
    {
      /* Compute solid angle of target as seen from origin. */
      *solid_angle = 2*fabs(width*sin(height/2));
    }

    /* Go to global coordinate system */

    tmp = coords_set(xi, yi, zi);
    tmp = rot_apply(Ainverse, tmp);
    coords_get(tmp, &xi, &yi, &zi);

    /* Now choose point uniformly on the unit sphere segment with angle theta/phi */
    phi   = width*randpm1()/2.0;
    theta = asin(randpm1()*sin(height/2.0));
    /* Now, to obtain the desired vector rotate (xi,yi,zi) angle theta around
       n, and then phi around u. */
    if(xi == 0 && zi == 0)
    {
      nx = 1;
      ny = 0;
      nz = 0;
    }
    else
    {
      nx = -zi;
      nz = xi;
      ny = 0;
    }
  }

  /* [xyz]u = [xyz]i x n[xyz] (usually vertical) */
  vec_prod(xu,  yu,  zu, xi, yi, zi,        nx, ny, nz);
  /* [xyz]t = [xyz]i rotated theta around [xyz]u */
  rotate  (xt,  yt,  zt, xi, yi, zi, theta, nx, ny, nz);
  /* [xyz]o = [xyz]t rotated phi around n[xyz] */
  rotate (*xo, *yo, *zo, xt, yt, zt, phi, xu,  yu,  zu);

  /* Go back to local coordinate system */
  tmp = coords_set(*xo, *yo, *zo);
  tmp = rot_apply(A, tmp);
  coords_get(tmp, &*xo, &*yo, &*zo);
}
/* randvec_target_rect_angular */

/*******************************************************************************
 * randvec_target_rect_real: Choose random direction towards target at (xi,yi,zi)
 * with given dimension height x width (in meters !).
 *
 * Local emission coordinate is taken into account and corrected for 'order' times.
 * (See remarks posted to mcstas-users by George Apostolopoulus <gapost@ipta.demokritos.gr>)
 *
 * If height or width is zero, choose random direction in full 4PI, no target.
 *
 * Traditionally, this routine had the name randvec_target_rect - this is now a
 * a define (see mcstas-r.h) pointing here. If you use the old rouine, you are NOT
 * taking the local emmission coordinate into account.
*******************************************************************************/
void _randvec_target_rect_real(double *xo, double *yo, double *zo, double *solid_angle,
        double xi, double yi, double zi,
        double width, double height, Rotation A,
        double lx, double ly, double lz, int order,
        _class_particle* _particle)
{
  double dx, dy, dist, dist_p, nx, ny, nz, mx, my, mz, n_norm, m_norm;
  double cos_theta;
  Coords tmp;
  Rotation Ainverse;

  rot_transpose(A, Ainverse);

  if(height == 0.0 || width == 0.0)
  {
    randvec_target_circle(xo, yo, zo, solid_angle,
               xi, yi, zi, 0);
    return;
  }
  else
  {
    /* Now choose point uniformly on rectangle within width x height */
    dx = width*randpm1()/2.0;
    dy = height*randpm1()/2.0;

    /* Determine distance to target plane*/
    dist = sqrt(xi*xi + yi*yi + zi*zi);
    /* Go to global coordinate system */

    tmp = coords_set(xi, yi, zi);
    tmp = rot_apply(Ainverse, tmp);
    coords_get(tmp, &xi, &yi, &zi);

    /* Determine vector normal to trajectory axis (z) and gravity [0 1 0] */
    vec_prod(nx, ny, nz, xi, yi, zi, 0, 1, 0);

    /* This now defines the x-axis, normalize: */
    n_norm=sqrt(nx*nx + ny*ny + nz*nz);
    nx = nx/n_norm;
    ny = ny/n_norm;
    nz = nz/n_norm;

    /* Now, determine our y-axis (vertical in many cases...) */
    vec_prod(mx, my, mz, xi, yi, zi, nx, ny, nz);
    m_norm=sqrt(mx*mx + my*my + mz*mz);
    mx = mx/m_norm;
    my = my/m_norm;
    mz = mz/m_norm;

    /* Our output, random vector can now be defined by linear combination: */

    *xo = xi + dx * nx + dy * mx;
    *yo = yi + dx * ny + dy * my;
    *zo = zi + dx * nz + dy * mz;

    /* Go back to local coordinate system */
    tmp = coords_set(*xo, *yo, *zo);
    tmp = rot_apply(A, tmp);
    coords_get(tmp, &*xo, &*yo, &*zo);

    /* Go back to local coordinate system */
    tmp = coords_set(xi, yi, zi);
    tmp = rot_apply(A, tmp);
    coords_get(tmp, &xi, &yi, &zi);

    if (solid_angle) {
      /* Calculate vector from local point to remote random point */
      lx = *xo - lx;
      ly = *yo - ly;
      lz = *zo - lz;
      dist_p = sqrt(lx*lx + ly*ly + lz*lz);

      /* Adjust the 'solid angle' */
      /* 1/r^2 to the chosen point times cos(\theta) between the normal */
      /* vector of the target rectangle and direction vector of the chosen point. */
      cos_theta = (xi * lx + yi * ly + zi * lz) / (dist * dist_p);
      *solid_angle = width * height / (dist_p * dist_p);
      int counter;
      for (counter = 0; counter < order; counter++) {
        *solid_angle = *solid_angle * cos_theta;
      }
    }
  }
}
/* randvec_target_rect_real */


/* SECTION: random numbers ==================================================

  How to add a new RNG:

  - Use an rng with a manegable state vector, e.g. of lengt 4 or 7. The state
  will sit on the particle struct as a "randstate_t state[RANDSTATE_LEN]"
  - If the rng has a long state (as MT), set an empty "srandom" and initialize
  it explicitly using the appropriate define (RNG_ALG)
  - Add a seed and a random function (the transforms will be reused)
  - Write the proper defines in mccode-r.h, e.g. randstate_t and RANDSTATE_LEN,
  srandom and random.
  - Compile using -DRNG_ALG=<selector int value>

============================================================================= */


/* "Mersenne Twister", by Makoto Matsumoto and Takuji Nishimura. */
/* See http://www.math.keio.ac.jp/~matumoto/emt.html for original source. */
/*
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using mt_srandom(seed)
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.keio.ac.jp/matumoto/emt.html
   email: matumoto@math.keio.ac.jp
*/
#include <stdio.h>
#include <stdint.h>   // for uint32_t
#include <stddef.h>   // for size_t

/* Period parameters */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfU   /* constant vector a */
#define UPPER_MASK 0x80000000U /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffU /* least significant r bits */

static uint32_t mt[N]; /* the array for the state vector  */
static int mti = N + 1; /* mti==N+1 means mt[N] is not initialized */

// Required for compatibility with common RNG interface (e.g., kiss/mt polymorphism)
void mt_srandom_empty(void) {}

// Initializes mt[N] with a seed
void mt_srandom(uint32_t seed) {
    mt[0] = seed;
    for (mti = 1; mti < N; mti++) {
        mt[mti] = 1812433253U * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti;
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt[mti] &= 0xffffffffU;
        /* for >32 bit machines */
    }
}
/* Initialize by an array with array-length.
   Init_key is the array for initializing keys.
   key_length is its length. */
void init_by_array(uint32_t init_key[], size_t key_length) {
    size_t i = 1, j = 0, k;
    mt_srandom(19650218U);
    k = (N > key_length ? N : key_length);
    for (; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525U))
              + init_key[j] + (uint32_t)j;
        mt[i] &= 0xffffffffU;
        i++; j++;
        if (i >= N) { mt[0] = mt[N - 1]; i = 1; }
        if (j >= key_length) j = 0;
    }
    for (k = N - 1; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941U))
              - (uint32_t)i;
        mt[i] &= 0xffffffffU;
        i++;
        if (i >= N) { mt[0] = mt[N - 1]; i = 1; }
    }
    mt[0] = 0x80000000U; /* MSB is 1; ensuring non-zero initial array */
}

// Generates a random number on [0, 0xffffffff]-interval
uint32_t mt_random(void) {
    uint32_t y;
    static const uint32_t mag01[2] = { 0x0U, MATRIX_A };
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N) { /* generate N words at one time */
        int kk;

        if (mti == N + 1)   /* if mt_srandom() has not been called, */ 
            mt_srandom(5489U);  /* a default initial seed is used */

        for (kk = 0; kk < N - M; kk++) {
            y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
            mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1U];
        }
        for (; kk < N - 1; kk++) {
            y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
            mt[kk] = mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1U];
        }
        y = (mt[N - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
        mt[N - 1] = mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1U];

        mti = 0;
    }

    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680U;
    y ^= (y << 15) & 0xefc60000U;
    y ^= (y >> 18);

    return y;
}
#undef N
#undef M
#undef MATRIX_A
#undef UPPER_MASK
#undef LOWER_MASK
/* End of "Mersenne Twister". */


/*
KISS

 From: http://www.helsbreth.org/random/rng_kiss.html
 Scott Nelson 1999

 Based on Marsaglia's KISS or (KISS+SWB) <http://www.cs.yorku.ca/~oz/marsaglia-
rng.html>

 KISS - Keep it Simple Stupid PRNG

 the idea is to use simple, fast, individually promising
 generators to get a composite that will be fast, easy to code
 have a very long period and pass all the tests put to it.
 The three components of KISS are
        x(n)=a*x(n-1)+1 mod 2^32
        y(n)=y(n-1)(I+L^13)(I+R^17)(I+L^5),
        z(n)=2*z(n-1)+z(n-2) +carry mod 2^32
 The y's are a shift register sequence on 32bit binary vectors
 period 2^32-1;
 The z's are a simple multiply-with-carry sequence with period
 2^63+2^32-1.  The period of KISS is thus
      2^32*(2^32-1)*(2^63+2^32-1) > 2^127

 In 2025 adapted for consistent 64-bit behavior across platforms.
*/

/* the KISS state is stored as a vector of 7 uint64_t        */
/*   0  1  2  3  4      5  6   */
/* [ x, y, z, w, carry, k, m ] */

uint64_t *kiss_srandom(uint64_t state[7], uint64_t seed) {
    if (seed == 0) seed = 1ull;
    state[0] = seed | 1ull; // x
    state[1] = seed | 2ull; // y
    state[2] = seed | 4ull; // z
    state[3] = seed | 8ull; // w
    state[4] = 0ull;        // carry
    state[5] = 0ull;        // k
    state[6] = 0ull;        // m
    return state;
}

uint64_t kiss_random(uint64_t state[7]) {
    // Linear congruential generator
    state[0] = state[0] * 69069ull + 1ull;

    // Xorshift
    state[1] ^= state[1] << 13ull;
    state[1] ^= state[1] >> 17ull;
    state[1] ^= state[1] << 5ull;

    // Multiply-with-carry
    state[5] = (state[2] >> 2ull) + (state[3] >> 3ull) + (state[4] >> 2ull);
    state[6] = state[3] + state[3] + state[2] + state[4];
    state[2] = state[3];
    state[3] = state[6];
    state[4] = state[5] >> 62ull;  // Top bit of carry (adjusted for 64-bit)

    return state[0] + state[1] + state[3];
}
/* end of "KISS" rng */


/* FAST KISS in another implementation (Hundt) */

//////////////////////////////////////////////////////////////////////////////
// fast keep it simple stupid generator
//////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Thomas Mueller hash for initialization of rngs
// http://stackoverflow.com/questions/664014/
//        what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
//////////////////////////////////////////////////////////////////////////////
randstate_t _hash(randstate_t x) {
  x = ((x >> 16) ^ x) * (randstate_t)0x45d9f3b;
  x = ((x >> 16) ^ x) * (randstate_t)0x45d9f3b;
  x = ((x >> 16) ^ x);
  return x;
}


// SECTION: random number transforms ==========================================



// generate a random number from normal law
double _randnorm(randstate_t* state)
{
  static double v1, v2, s; /* removing static breaks comparison with McStas <= 2.5 */
  static int phase = 0;
  double X, u1, u2;

  if(phase == 0)
  {
    do
    {
      u1 = _rand01(state);
      u2 = _rand01(state);
      v1 = 2*u1 - 1;
      v2 = 2*u2 - 1;
      s = v1*v1 + v2*v2;
    } while(s >= 1 || s == 0);

    X = v1*sqrt(-2*log(s)/s);
  }
  else
  {
    X = v2*sqrt(-2*log(s)/s);
  }

  phase = 1 - phase;
  return X;
}
// another one
double _randnorm2(randstate_t* state) {
  double x, y, r;
  do {
      x = 2.0 * _rand01(state) - 1.0;
      y = 2.0 * _rand01(state) - 1.0;
      r = x*x + y*y;
  } while (r == 0.0 || r >= 1.0);
  return x * sqrt((-2.0 * log(r)) / r);
}

// Generate a random number from -1 to 1 with triangle distribution
double _randtriangle(randstate_t* state) {
	double randnum = _rand01(state);
	if (randnum>0.5) return(1-sqrt(2*(randnum-0.5)));
	else return(sqrt(2*randnum)-1);
}
double _rand01(randstate_t* state) {
	double randnum;
	randnum = (double) _random();
  // TODO: can we mult instead of div?
	randnum /= (double) MC_RAND_MAX + 1;
	return randnum;
}
double _rand01_opague(void* opague_state) {
	randstate_t* state = (randstate_t*)opague_state;
	// Following lines exactly like in _rand01 just above (repeated to
	// avoid another layer of indirection):
	double randnum;
	randnum = (double) _random();
	// TODO: can we mult instead of div?
	randnum /= (double) MC_RAND_MAX + 1;
	return randnum;
}
// Return a random number between 1 and -1
double _randpm1(randstate_t* state) {
	double randnum;
	randnum = (double) _random();
	randnum /= ((double) MC_RAND_MAX + 1) / 2;
	randnum -= 1;
	return randnum;
}
// Return a random number between 0 and max.
double _rand0max(double max, randstate_t* state) {
	double randnum;
	randnum = (double) _random();
	randnum /= ((double) MC_RAND_MAX + 1) / max;
	return randnum;
}
// Return a random number between min and max.
double _randminmax(double min, double max, randstate_t* state) {
	return _rand0max(max - min, state) + max;
}


/* SECTION: main and signal handlers ======================================== */

/*******************************************************************************
* mchelp: displays instrument executable help with possible options
*******************************************************************************/
static void
mchelp(char *pgmname)
{
  int i;

  fprintf(stderr, "%s (%s) instrument simulation, generated with " MCCODE_STRING " (" MCCODE_DATE ")\n", instrument_name, instrument_source);
  fprintf(stderr, "Usage: %s [options] [parm=value ...]\n", pgmname);
  fprintf(stderr,
"Options are:\n"
"  -s SEED   --seed=SEED      Set random seed (must be != 0)\n"
"  -n COUNT  --ncount=COUNT   Set number of particles to simulate.\n"
"  -d DIR    --dir=DIR        Put all data files in directory DIR.\n"
"  -a        --append         Append data files to those in directory DIR.\n"	  
"  -t        --trace          Enable trace of " MCCODE_PARTICLE "s through instrument.\n"
"                             (Use -t=2 or --trace=2 for modernised mcdisplay rendering)\n"
"  -g        --gravitation    Enable gravitation for all trajectories.\n"
"  --no-output-files          Do not write any data files.\n"
"  -h        --help           Show this help message.\n"
"  -i        --info           Detailed instrument information.\n"
"  --list-parameters          Print the instrument parameters to standard out\n"
"  -y        --yes            Assume default values for all parameters with a default\n"
"  --meta-list                Print names of components which defined metadata\n"
"  --meta-defined COMP[:NAME] Print component defined metadata names, or (0,1) if NAME provided\n"
"  --meta-type COMP:NAME      Print metadata format type specified in definition\n"
"  --meta-data COMP:NAME      Print the metadata text\n"
"  --source                   Show the instrument code which was compiled.\n"
#ifdef OPENACC
"\n"
"  --vecsize                  OpenACC vector-size (default: 128)\n"
"  --numgangs                 Number of OpenACC gangs (default: 7813)\n"
"  --gpu_innerloop            Maximum rays to process pr. OpenACC \n"
"                             kernel run (default: 2147483647)\n"
"\n"
#endif
"\n"
"  --bufsiz                   Monitor_nD list/buffer-size (default: 1000000)\n"
"  --format=FORMAT            Output data files using FORMAT="
   FLAVOR_UPPER
#ifdef USE_NEXUS
   " NEXUS\n"
"  --IDF                      Embed an xml-formatted IDF instrument definition\n"
"                             in the NeXus file (if existent in .)\n\n"
#else
"\n\n"
#endif
);
#ifdef USE_MPI
  fprintf(stderr,
  "This instrument has been compiled with MPI support.\n  Use 'mpirun %s [options] [parm=value ...]'.\n", pgmname);
#endif
#ifdef OPENACC
  fprintf(stderr,
  "This instrument has been compiled with NVIDIA GPU support through OpenACC.\n  Running on systems without such devices will lead to segfaults.\nFurter, fprintf, sprintf and printf have been removed from any component TRACE.\n");
#endif

  if(numipar > 0)
  {
    fprintf(stderr, "Instrument parameters are:\n");
    for(i = 0; i < numipar; i++)
      if (mcinputtable[i].val && strlen(mcinputtable[i].val))
        fprintf(stderr, "  %-16s(%s) [default='%s']\n", mcinputtable[i].name,
        (*mcinputtypes[mcinputtable[i].type].parminfo)(mcinputtable[i].name),
        mcinputtable[i].val);
      else
        fprintf(stderr, "  %-16s(%s)\n", mcinputtable[i].name,
        (*mcinputtypes[mcinputtable[i].type].parminfo)(mcinputtable[i].name));
  }

#ifndef NOSIGNALS
  fprintf(stderr, "Known signals are: "
#ifdef SIGUSR1
  "USR1 (status) "
#endif
#ifdef SIGUSR2
  "USR2 (save) "
#endif
#ifdef SIGBREAK
  "BREAK (save) "
#endif
#ifdef SIGTERM
  "TERM (save and exit)"
#endif
  "\n");
#endif /* !NOSIGNALS */
} /* mchelp */


/* mcshowhelp: show help and exit with 0 */
static void
mcshowhelp(char *pgmname)
{
  mchelp(pgmname);
  exit(0);
}

/* mcusage: display usage when error in input arguments and exit with 1 */
static void
mcusage(char *pgmname)
{
  fprintf(stderr, "Error: incorrect command line arguments\n");
  mchelp(pgmname);
  exit(1);
}

/* mcenabletrace: enable trace/mcdisplay or error if requires recompile */
static void
mcenabletrace(int mode)
{
 if(traceenabled) {
  mcdotrace = mode;
  #pragma acc update device ( mcdotrace )
 } else {
   if (mode>0) {
     fprintf(stderr,
	     "Error: trace not enabled (mcenabletrace)\n"
	     "Please re-run the " MCCODE_NAME " compiler "
	     "with the --trace option, or rerun the\n"
	     "C compiler with the MC_TRACE_ENABLED macro defined.\n");
     exit(1);
   }
 }
}

/*******************************************************************************
* mcreadparams: request parameters from the prompt (or use default)
*******************************************************************************/
void
mcreadparams(void)
{
  int i,j,status;
  static char buf[CHAR_BUF_LENGTH];
  char *p;
  int len;

  MPI_MASTER(printf("Instrument parameters for %s (%s)\n",
                    instrument_name, instrument_source));

  for(i = 0; mcinputtable[i].name != 0; i++)
  {
    do
    {
      MPI_MASTER(
                 if (mcinputtable[i].val && strlen(mcinputtable[i].val))
                   printf("Set value of instrument parameter %s (%s) [default='%s']:\n",
                          mcinputtable[i].name,
                          (*mcinputtypes[mcinputtable[i].type].parminfo)
                          (mcinputtable[i].name), mcinputtable[i].val);
                 else
                   printf("Set value of instrument parameter %s (%s):\n",
                          mcinputtable[i].name,
                          (*mcinputtypes[mcinputtable[i].type].parminfo)
                          (mcinputtable[i].name));
                 fflush(stdout);
                 );
#ifdef USE_MPI
      if(mpi_node_rank == mpi_node_root)
        {
          p = fgets(buf, CHAR_BUF_LENGTH, stdin);
          if(p == NULL)
            {
              fprintf(stderr, "Error: empty input for paramater %s (mcreadparams)\n", mcinputtable[i].name);
              exit(1);
            }
        }
      else
        p = buf;
      MPI_Bcast(buf, CHAR_BUF_LENGTH, MPI_CHAR, mpi_node_root, MPI_COMM_WORLD);
#else /* !USE_MPI */
      p = fgets(buf, CHAR_BUF_LENGTH, stdin);
      if(p == NULL)
        {
          fprintf(stderr, "Error: empty input for paramater %s (mcreadparams)\n", mcinputtable[i].name);
          exit(1);
        }
#endif /* USE_MPI */
      len = strlen(buf);
      if (!len || (len == 1 && (buf[0] == '\n' || buf[0] == '\r')))
      {
        if (mcinputtable[i].val && strlen(mcinputtable[i].val)) {
          strncpy(buf, mcinputtable[i].val, CHAR_BUF_LENGTH);  /* use default value */
          len = strlen(buf);
        }
      }
      for(j = 0; j < 2; j++)
      {
        if(len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == '\r'))
        {
          len--;
          buf[len] = '\0';
        }
      }

      status = (*mcinputtypes[mcinputtable[i].type].getparm)
                   (buf, mcinputtable[i].par);
      if(!status)
      {
        (*mcinputtypes[mcinputtable[i].type].error)(mcinputtable[i].name, buf);
        if (!mcinputtable[i].val || strlen(mcinputtable[i].val)) {
          fprintf(stderr, "       Change %s default value in instrument definition.\n", mcinputtable[i].name);
          exit(1);
        }
      }
    } while(!status);
  }
} /* mcreadparams */

/*******************************************************************************
* mcparseoptions: parse command line arguments (options, parameters)
*******************************************************************************/
void
mcparseoptions(int argc, char *argv[])
{
  int i, j;
  char *p;
  int paramset = 0, *paramsetarray;
  char *usedir=NULL;

  /* Add one to numipar to avoid allocating zero size memory block. */
  paramsetarray = (int*)malloc((numipar + 1)*sizeof(*paramsetarray));
  if(paramsetarray == NULL)
  {
    fprintf(stderr, "Error: insufficient memory (mcparseoptions)\n");
    exit(1);
  }
  for(j = 0; j < numipar; j++)
    {
      paramsetarray[j] = 0;
      if (mcinputtable[j].val != NULL && strlen(mcinputtable[j].val))
      {
        int  status;
        char buf[CHAR_BUF_LENGTH];
        strncpy(buf, mcinputtable[j].val, CHAR_BUF_LENGTH);
        status = (*mcinputtypes[mcinputtable[j].type].getparm)
                   (buf, mcinputtable[j].par);
        if(!status) fprintf(stderr, "Invalid '%s' default value %s in instrument definition (mcparseoptions)\n", mcinputtable[j].name, buf);
        else paramsetarray[j] = 1;
      } else {
        (*mcinputtypes[mcinputtable[j].type].getparm)
          (NULL, mcinputtable[j].par);
        paramsetarray[j] = 0;
      }
    }
  for(i = 1; i < argc; i++)
  {
    if(!strcmp("-s", argv[i]) && (i + 1) < argc)
      mcsetseed(argv[++i]);
    else if(!strncmp("-s", argv[i], 2))
      mcsetseed(&argv[i][2]);
    else if(!strcmp("--seed", argv[i]) && (i + 1) < argc)
      mcsetseed(argv[++i]);
    else if(!strncmp("--seed=", argv[i], 7))
      mcsetseed(&argv[i][7]);
    else if(!strcmp("-n", argv[i]) && (i + 1) < argc)
      mcsetn_arg(argv[++i]);
    else if(!strncmp("-n", argv[i], 2))
      mcsetn_arg(&argv[i][2]);
    else if(!strcmp("--ncount", argv[i]) && (i + 1) < argc)
      mcsetn_arg(argv[++i]);
    else if(!strncmp("--ncount=", argv[i], 9))
      mcsetn_arg(&argv[i][9]);
    else if(!strcmp("-d", argv[i]) && (i + 1) < argc)
      usedir=argv[++i];  /* will create directory after parsing all arguments (end of this function) */
    else if(!strncmp("-d", argv[i], 2))
      usedir=&argv[i][2];
    else if(!strcmp("--dir", argv[i]) && (i + 1) < argc)
      usedir=argv[++i];
    else if(!strncmp("-a", argv[i], 2))
      mcappend = 1;
    else if(!strcmp("--append", argv[i]))
      mcappend = 1;
    else if(!strncmp("--dir=", argv[i], 6))
      usedir=&argv[i][6];
    else if(!strcmp("-h", argv[i]))
      mcshowhelp(argv[0]);
    else if(!strcmp("--help", argv[i]) || !strcmp("--version", argv[i]))
      mcshowhelp(argv[0]);
    else if(!strcmp("-i", argv[i])) {
      mcformat=FLAVOR_UPPER;
      mcinfo();
    }
    else if(!strcmp("--info", argv[i]))
      mcinfo();
    else if (!strcmp("--list-parameters", argv[i]))
      mcparameterinfo();
    else if (!strcmp("--meta-list", argv[i]) && ((i+1) >= argc || argv[i+1][0] == '-')){
      //printf("Components with metadata defined:\n");
      exit(metadata_table_print_all_components(num_metadata, metadata_table) == 0);
    }
    else if (!strcmp("--meta-defined", argv[i]) && (i+1) < argc){
      exit(metadata_table_print_component_keys(num_metadata, metadata_table, argv[i+1]) == 0);
    }
    else if (!strcmp("--meta-type", argv[i]) && (i+1) < argc){
      char * literal_type = metadata_table_type(num_metadata, metadata_table, argv[i+1]);
      if (literal_type == NULL) exit(1);
      printf("%s\n", literal_type);
      exit(0);
    }
    else if (!strcmp("--meta-data", argv[i]) && (i+1) < argc){
      char * literal = metadata_table_literal(num_metadata, metadata_table, argv[i+1]);
      if (literal == NULL) exit(1);
      printf("%s\n", literal);
      exit(0);
    }
    else if(!strncmp("--trace=", argv[i], 8)) {
      mcenabletrace(atoi(&argv[i][8]));
    } else if(!strncmp("-t=", argv[i], 3) || !strcmp("--verbose", argv[i])) {
      mcenabletrace(atoi(&argv[i][3]));
    } else if(!strcmp("-t", argv[i]))
      mcenabletrace(1);
    else if(!strcmp("--trace", argv[i]) || !strcmp("--verbose", argv[i]))
      mcenabletrace(1);
    else if(!strcmp("--gravitation", argv[i]))
      mcgravitation = 1;
    else if(!strcmp("-g", argv[i]))
      mcgravitation = 1;
    else if(!strcmp("--yes", argv[i]))
      mcusedefaults = 1;
    else if(!strcmp("-y", argv[i]))
      mcusedefaults = 1;
    else if(!strncmp("--format=", argv[i], 9)) {
      mcformat=&argv[i][9];
    }
    else if(!strcmp("--format", argv[i]) && (i + 1) < argc) {
      mcformat=argv[++i];
    }
#ifdef USE_NEXUS
    else if(!strcmp("--IDF", argv[i])) {
      mcnexus_embed_idf = 1;
    }
#endif
    else if(!strncmp("--vecsize=", argv[i], 10)) {
      vecsize=atoi(&argv[i][10]);
    }    
    else if(!strcmp("--vecsize", argv[i]) && (i + 1) < argc) {
      vecsize=atoi(argv[++i]);
    }
    else if(!strncmp("--bufsiz=", argv[i], 9)) {
      MONND_BUFSIZ=atoi(&argv[i][9]);
    }
    else if(!strcmp("--bufsiz", argv[i]) && (i + 1) < argc) {
      MONND_BUFSIZ=atoi(argv[++i]);
    }
    else if(!strncmp("--numgangs=", argv[i], 11)) {
      numgangs=atoi(&argv[i][11]);
    }
    else if(!strcmp("--numgangs", argv[i]) && (i + 1) < argc) {
      numgangs=atoi(argv[++i]);
    }
    else if(!strncmp("--gpu_innerloop=", argv[i], 16)) {
      gpu_innerloop=(long)strtod(&argv[i][16], NULL);
    }
    else if(!strcmp("--gpu_innerloop", argv[i]) && (i + 1) < argc) {
      gpu_innerloop=(long)strtod(argv[++i], NULL);
    }

    else if(!strcmp("--no-output-files", argv[i]))
      mcdisable_output_files = 1;
    else if(!strcmp("--source", argv[i])) {
      printf("/* Source code %s from %s: */\n"
        "/******************************************************************************/\n"
        "%s\n"
        "/******************************************************************************/\n"
        "/* End of source code %s from %s */\n",
        instrument_name, instrument_source, instrument_code,
        instrument_name, instrument_source);
      exit(1);
    }
    else if(argv[i][0] != '-' && (p = strchr(argv[i], '=')) != NULL)
    {
      *p++ = '\0';

      for(j = 0; j < numipar; j++)
        if(!strcmp(mcinputtable[j].name, argv[i]))
        {
          int status;
          status = (*mcinputtypes[mcinputtable[j].type].getparm)(p,
                        mcinputtable[j].par);
          if(!status || !strlen(p))
          {
            (*mcinputtypes[mcinputtable[j].type].error)
              (mcinputtable[j].name, p);
            exit(1);
          }
          paramsetarray[j] = 1;
          paramset = 1;
          break;
        }
      if(j == numipar)
      {                                /* Unrecognized parameter name */
        fprintf(stderr, "Error: unrecognized parameter %s (mcparseoptions)\n", argv[i]);
        exit(1);
      }
    }
    else if(argv[i][0] == '-') {
      fprintf(stderr, "Error: unrecognized option argument %s (mcparseoptions). Ignored.\n", argv[i++]);
    }
    else {
      fprintf(stderr, "Error: unrecognized argument %s (mcparseoptions). Aborting.\n", argv[i]);
      mcusage(argv[0]);
    }
  }
  if (mcusedefaults) {
    MPI_MASTER(
     printf("Using all default parameter values\n");
    );
    for(j = 0; j < numipar; j++) {
      int status;
      if(mcinputtable[j].val && strlen(mcinputtable[j].val)){
	status = (*mcinputtypes[mcinputtable[j].type].getparm)(mcinputtable[j].val,
                        mcinputtable[j].par);
	paramsetarray[j] = 1;
	paramset = 1;
      }
    }
  }
  if(!paramset)
    mcreadparams();                /* Prompt for parameters if not specified. */
  else
  {
    for(j = 0; j < numipar; j++)
      if(!paramsetarray[j])
      {
        fprintf(stderr, "Error: Instrument parameter %s left unset (mcparseoptions)\n",
                mcinputtable[j].name);
        exit(1);
      }
  }
  free(paramsetarray);
#ifdef USE_MPI
  if (mcdotrace) mpi_node_count=1; /* disable threading when in trace mode */
#endif
  if (usedir && strlen(usedir) && !mcdisable_output_files) mcuse_dir(usedir);
} /* mcparseoptions */

#ifndef NOSIGNALS
/*******************************************************************************
* sighandler: signal handler that makes simulation stop, and save results
*******************************************************************************/
void sighandler(int sig)
{
  /* MOD: E. Farhi, Sep 20th 2001: give more info */
  time_t t1, t0;
#define SIG_SAVE 0
#define SIG_TERM 1
#define SIG_STAT 2
#define SIG_ABRT 3

  printf("\n# " MCCODE_STRING ": [pid %i] Signal %i detected", getpid(), sig);
#ifdef USE_MPI
  printf(" [proc %i]", mpi_node_rank);
#endif
#if defined(SIGUSR1) && defined(SIGUSR2) && defined(SIGKILL)
  if (!strcmp(mcsig_message, "sighandler") && (sig != SIGUSR1) && (sig != SIGUSR2))
  {
    printf("\n# Fatal : unrecoverable loop ! Suicide (naughty boy).\n");
    kill(0, SIGKILL); /* kill myself if error occurs within sighandler: loops */
  }
#endif
  switch (sig) {
#ifdef SIGINT
    case SIGINT : printf(" SIGINT (interrupt from terminal, Ctrl-C)"); sig = SIG_TERM; break;
#endif
#ifdef SIGILL
    case SIGILL  : printf(" SIGILL (Illegal instruction)"); sig = SIG_ABRT; break;
#endif
#ifdef SIGFPE
    case SIGFPE  : printf(" SIGFPE (Math Error)"); sig = SIG_ABRT; break;
#endif
#ifdef SIGSEGV
    case SIGSEGV : printf(" SIGSEGV (Mem Error)"); sig = SIG_ABRT; break;
#endif
#ifdef SIGTERM
    case SIGTERM : printf(" SIGTERM (Termination)"); sig = SIG_TERM; break;
#endif
#ifdef SIGABRT
    case SIGABRT : printf(" SIGABRT (Abort)"); sig = SIG_ABRT; break;
#endif
#ifdef SIGQUIT
    case SIGQUIT : printf(" SIGQUIT (Quit from terminal)"); sig = SIG_TERM; break;
#endif
#ifdef SIGTRAP
    case SIGTRAP : printf(" SIGTRAP (Trace trap)"); sig = SIG_ABRT; break;
#endif
#ifdef SIGPIPE
    case SIGPIPE : printf(" SIGPIPE (Broken pipe)"); sig = SIG_ABRT; break;
#endif
#ifdef SIGUSR1
    case SIGUSR1 : printf(" SIGUSR1 (Display info)"); sig = SIG_STAT; break;
#endif
#ifdef SIGUSR2
    case SIGUSR2 : printf(" SIGUSR2 (Save simulation)"); sig = SIG_SAVE; break;
#endif
#ifdef SIGHUP
    case SIGHUP  : printf(" SIGHUP (Hangup/update)"); sig = SIG_SAVE; break;
#endif
#ifdef SIGBUS
    case SIGBUS  : printf(" SIGBUS (Bus error)"); sig = SIG_ABRT; break;
#endif
#ifdef SIGURG
    case SIGURG  : printf(" SIGURG (Urgent socket condition)"); sig = SIG_ABRT; break;
#endif
#ifdef SIGBREAK
    case SIGBREAK: printf(" SIGBREAK (Break signal, Ctrl-Break)"); sig = SIG_SAVE; break;
#endif
    default : printf(" (look at signal list for signification)"); sig = SIG_ABRT; break;
  }
  printf("\n");
  printf("# Simulation: %s (%s) \n", instrument_name, instrument_source);
  printf("# Breakpoint: %s ", mcsig_message);
  if (strstr(mcsig_message, "Save") && (sig == SIG_SAVE))
    sig = SIG_STAT;
  SIG_MESSAGE("sighandler");
  if (mcget_ncount() == 0)
    printf("(0 %%)\n" );
  else
  {
    printf("%.2f %% (%10.1f/%10.1f)\n", 100.0*mcget_run_num()/mcget_ncount(), 1.0*mcget_run_num(), 1.0*mcget_ncount());
  }
  t0 = (time_t)mcstartdate;
  t1 = time(NULL);
  printf("# Date:      %s", ctime(&t1));
  printf("# Started:   %s", ctime(&t0));

  if (sig == SIG_STAT)
  {
    printf("# " MCCODE_STRING ": Resuming simulation (continue)\n");
    fflush(stdout);
    return;
  }
  else
  if (sig == SIG_SAVE)
  {
    printf("# " MCCODE_STRING ": Saving data and resume simulation (continue)\n");
    save(NULL);
    fflush(stdout);
    return;
  }
  else
  if (sig == SIG_TERM)
  {
    printf("# " MCCODE_STRING ": Finishing simulation (save results and exit)\n");
    finally();
    exit(0);
  }
  else
  {
    fflush(stdout);
    perror("# Last I/O Error");
    printf("# " MCCODE_STRING ": Simulation stop (abort).\n");
// This portion of the signal handling only works on UNIX
#if defined(__unix__) || defined(__APPLE__)
    signal(sig, SIG_DFL); /* force to use default sighandler now */
    kill(getpid(), sig);  /* and trigger it with the current signal */
#endif
    exit(-1);
  }
#undef SIG_SAVE
#undef SIG_TERM
#undef SIG_STAT
#undef SIG_ABRT

} /* sighandler */
#endif /* !NOSIGNALS */

#ifdef NEUTRONICS
/*Main neutronics function steers the McStas calls, initializes parameters etc */
/* Only called in case NEUTRONICS = TRUE */
void neutronics_main_(float *inx, float *iny, float *inz, float *invx, float *invy, float *invz, float *intime, float *insx, float *insy, float *insz, float *inw, float *outx, float *outy, float *outz, float *outvx, float *outvy, float *outvz, float *outtime, float *outsx, float *outsy, float *outsz, float *outwgt)
{

  extern double mcnx, mcny, mcnz, mcnvx, mcnvy, mcnvz;
  extern double mcnt, mcnsx, mcnsy, mcnsz, mcnp;

  /* External code governs iteration - McStas is iterated once per call to neutronics_main. I.e. below counter must be initiancated for each call to neutronics_main*/
  mcrun_num=0;

  time_t t;
  t = (time_t)mcstartdate;
  mcstartdate = t;  /* set start date before parsing options and creating sim file */
  init();

  /* *** parse options *** */
  SIG_MESSAGE("[" __FILE__ "] main START");
  mcformat=getenv(FLAVOR_UPPER "_FORMAT") ?
           getenv(FLAVOR_UPPER "_FORMAT") : FLAVOR_UPPER;

  /* Set neutron state based on input from neutronics code */
  mcsetstate(*inx,*iny,*inz,*invx,*invy,*invz,*intime,*insx,*insy,*insz,*inw);

  /* main neutron event loop - runs only one iteration */

  //mcstas_raytrace(&mcncount); /* prior to McStas 1.12 */

  mcallowbackprop = 1; //avoid absorbtion from negative dt
  int argc=1;
  char *argv[0];
  int dummy = mccode_main(argc, argv);

  *outx =  mcnx;
  *outy =  mcny;
  *outz =  mcnz;
  *outvx =  mcnvx;
  *outvy =  mcnvy;
  *outvz =  mcnvz;
  *outtime =  mcnt;
  *outsx =  mcnsx;
  *outsy =  mcnsy;
  *outsz =  mcnsz;
  *outwgt =  mcnp;

  return;
} /* neutronics_main */

#endif /*NEUTRONICS*/

#endif /* !MCCODE_H */
/* End of file "mccode-r.c". */
/* End of file "mccode-r.c". */

/* embedding file "mcstas-r.c" */

/*******************************************************************************
*
* McStas, neutron ray-tracing package
*         Copyright (C) 1997-2009, All rights reserved
*         Risoe National Laboratory, Roskilde, Denmark
*         Institut Laue Langevin, Grenoble, France
*
* Runtime: share/mcstas-r.c
*
* %Identification
* Written by: KN
* Date:    Aug 29, 1997
* Release: McStas X.Y
* Version: $Revision$
*
* Runtime system for McStas.
* Embedded within instrument in runtime mode.
*
* Usage: Automatically embbeded in the c code whenever required.
*
* $Id$
*
*******************************************************************************/

#ifndef MCSTAS_R_H
#include "mcstas-r.h"
#endif
#ifdef DANSE
#include "mcstas-globals.h"
#endif

/*******************************************************************************
* The I/O format definitions and functions
*******************************************************************************/

/*the magnet stack*/
#ifdef MC_POL_COMPAT
void (*mcMagnetPrecession) (double, double, double, double, double, double,
    double, double*, double*, double*, double, Coords, Rotation)=NULL;
Coords   mcMagnetPos;
Rotation mcMagnetRot;
double*  mcMagnetData                = NULL;
/* mcMagneticField(x, y, z, t, Bx, By, Bz) */
int (*mcMagneticField) (double, double, double, double,
    double*, double*, double*, void *) = NULL;
#endif

#ifndef MCSTAS_H

/*******************************************************************************
* mcsetstate: transfer parameters into global McStas variables
*******************************************************************************/
_class_particle mcsetstate(double x, double y, double z, double vx, double vy, double vz,
			   double t, double sx, double sy, double sz, double p, int mcgravitation, void *mcMagnet, int mcallowbackprop)
{
  _class_particle mcneutron;

  mcneutron.x  = x;
  mcneutron.y  = y;
  mcneutron.z  = z;
  mcneutron.vx = vx;
  mcneutron.vy = vy;
  mcneutron.vz = vz;
  mcneutron.t  = t;
  mcneutron.sx = sx;
  mcneutron.sy = sy;
  mcneutron.sz = sz;
  mcneutron.p  = p;
  mcneutron.mcgravitation = mcgravitation;
  mcneutron.mcMagnet = mcMagnet;
  mcneutron.allow_backprop = mcallowbackprop;
  mcneutron._uid       = 0;
  mcneutron._index     = 1;
  mcneutron._absorbed  = 0;
  mcneutron._restore   = 0;
  mcneutron._scattered = 0;
  mcneutron.flag_nocoordschange = 0;
  
  /* init tmp-vars - FIXME are they used? */
  mcneutron._mctmp_a = mcneutron._mctmp_b =  mcneutron._mctmp_c = 0;
  // what about mcneutron._logic ?
  mcneutron._logic.dummy=1;
  // init uservars via cogen'd-function
  particle_uservar_init(&mcneutron);

  return(mcneutron);
} /* mcsetstate */

/*******************************************************************************
* mcgetstate: get neutron parameters from particle structure
*******************************************************************************/
_class_particle mcgetstate(_class_particle mcneutron, double *x, double *y, double *z,
               double *vx, double *vy, double *vz, double *t,
               double *sx, double *sy, double *sz, double *p)
{
  *x  =  mcneutron.x;
  *y  =  mcneutron.y;
  *z  =  mcneutron.z;
  *vx =  mcneutron.vx;
  *vy =  mcneutron.vy;
  *vz =  mcneutron.vz;
  *t  =  mcneutron.t;
  *sx =  mcneutron.sx;
  *sy =  mcneutron.sy;
  *sz =  mcneutron.sz;
  *p  =  mcneutron.p;

  return(mcneutron);
} /* mcgetstate */

/*******************************************************************************
* SCATTER_func: provides function to SCATTER from within libaries
*******************************************************************************/
void SCATTER_func(_class_particle *_particle) {
  if(mcdotrace) {
    printf("SCATTER: %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g\n", 
           _particle->x,_particle->y,_particle->z,
           _particle->vx,_particle->vy,_particle->vz,
		   _particle->t,
		   _particle->sx,_particle->sy,_particle->sz,
		   _particle->p);
  }
  if (!_particle->_absorbed) _particle->_scattered++;
}  /* SCATTER_func */

/*******************************************************************************
* mcgenstate: set default neutron parameters
*******************************************************************************/
// Moved to generated code
/* #pragma acc routine seq */
/* _class_particle mcgenstate(void) */
/* { */
/*   return(mcsetstate(0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, mcgravitation, mcMagnet, mcallowbackprop)); */
/* } */

/*******************************************************************************
* mccoordschanges: old style rotation routine rot -> (x y z) ,(vx vy vz),(sx,sy,sz)
*******************************************************************************/
void
mccoordschanges(Coords a, Rotation t, double *x, double *y, double *z,
               double *vx, double *vy, double *vz, double *sx, double *sy, double *sz)
{
  Coords b, c;

  b.x = *x;
  b.y = *y;
  b.z = *z;
  c = rot_apply(t, b);
  b = coords_add(c, a);
  *x = b.x;
  *y = b.y;
  *z = b.z;

  if ( (vz && vy  && vx) && (*vz != 0.0 || *vx != 0.0 || *vy != 0.0) )
    mccoordschange_polarisation(t, vx, vy, vz);

  if ( (sz && sy  && sx) && (*sz != 0.0 || *sx != 0.0 || *sy != 0.0) )
    mccoordschange_polarisation(t, sx, sy, sz);

}

/* intersection routines ==================================================== */

/*******************************************************************************
* inside_rectangle: Check if (x,y) is inside rectangle (xwidth, yheight)
* return 0 if outside and 1 if inside
*******************************************************************************/
int inside_rectangle(double x, double y, double xwidth, double yheight)
{
  if (x>-xwidth/2 && x<xwidth/2 && y>-yheight/2 && y<yheight/2)
    return 1;
  else
    return 0;
}

/*******************************************************************************
 * box_intersect: compute time intersection with a box
 * returns 0 when no intersection is found
 *      or 1 in case of intersection with resulting times dt_in and dt_out
 * This function written by Stine Nyborg, 1999.
 *******************************************************************************/
int box_intersect(double *dt_in, double *dt_out,
                  double x, double y, double z,
                  double vx, double vy, double vz,
                  double dx, double dy, double dz)
{
  double x_in, y_in, z_in, tt, t[6], a, b;
  int i, count, s;

      /* Calculate intersection time for each of the six box surface planes
       *  If the box surface plane is not hit, the result is zero.*/

  if(vx != 0)
   {
    tt = -(dx/2 + x)/vx;
    y_in = y + tt*vy;
    z_in = z + tt*vz;
    if( y_in > -dy/2 && y_in < dy/2 && z_in > -dz/2 && z_in < dz/2)
      t[0] = tt;
    else
      t[0] = 0;

    tt = (dx/2 - x)/vx;
    y_in = y + tt*vy;
    z_in = z + tt*vz;
    if( y_in > -dy/2 && y_in < dy/2 && z_in > -dz/2 && z_in < dz/2)
      t[1] = tt;
    else
      t[1] = 0;
   }
  else
    t[0] = t[1] = 0;

  if(vy != 0)
   {
    tt = -(dy/2 + y)/vy;
    x_in = x + tt*vx;
    z_in = z + tt*vz;
    if( x_in > -dx/2 && x_in < dx/2 && z_in > -dz/2 && z_in < dz/2)
      t[2] = tt;
    else
      t[2] = 0;

    tt = (dy/2 - y)/vy;
    x_in = x + tt*vx;
    z_in = z + tt*vz;
    if( x_in > -dx/2 && x_in < dx/2 && z_in > -dz/2 && z_in < dz/2)
      t[3] = tt;
    else
      t[3] = 0;
   }
  else
    t[2] = t[3] = 0;

  if(vz != 0)
   {
    tt = -(dz/2 + z)/vz;
    x_in = x + tt*vx;
    y_in = y + tt*vy;
    if( x_in > -dx/2 && x_in < dx/2 && y_in > -dy/2 && y_in < dy/2)
      t[4] = tt;
    else
      t[4] = 0;

    tt = (dz/2 - z)/vz;
    x_in = x + tt*vx;
    y_in = y + tt*vy;
    if( x_in > -dx/2 && x_in < dx/2 && y_in > -dy/2 && y_in < dy/2)
      t[5] = tt;
    else
      t[5] = 0;
   }
  else
    t[4] = t[5] = 0;

  /* The intersection is evaluated and *dt_in and *dt_out are assigned */

  a = b = s = 0;
  count = 0;

  for( i = 0; i < 6; i = i + 1 )
    if( t[i] == 0 )
      s = s+1;
    else if( count == 0 )
    {
      a = t[i];
      count = 1;
    }
    else
    {
      b = t[i];
      count = 2;
    }

  if ( a == 0 && b == 0 )
    return 0;
  else if( a < b )
  {
    *dt_in = a;
    *dt_out = b;
    return 1;
  }
  else
  {
    *dt_in = b;
    *dt_out = a;
    return 1;
  }

} /* box_intersect */

/*******************************************************************************
 * cylinder_intersect: compute intersection with a cylinder
 * returns 0 when no intersection is found
 *      or 2/4/8/16 bits depending on intersection,
 *     and resulting times t0 and t1
 * Written by: EM,NB,ABA 4.2.98
  *******************************************************************************/
int cylinder_intersect(double *t0, double *t1, double x, double y, double z,
                   double vx, double vy, double vz, double r, double h)
{
  double D, t_in, t_out, y_in, y_out;
  int ret=1;

  D = (2*vx*x + 2*vz*z)*(2*vx*x + 2*vz*z)
    - 4*(vx*vx + vz*vz)*(x*x + z*z - r*r);

  if (D>=0)
  {
    if (vz*vz + vx*vx) {
      t_in  = (-(2*vz*z + 2*vx*x) - sqrt(D))/(2*(vz*vz + vx*vx));
      t_out = (-(2*vz*z + 2*vx*x) + sqrt(D))/(2*(vz*vz + vx*vx));
    } else if (vy) { /* trajectory parallel to cylinder axis */
      t_in = (-h/2-y)/vy;
      t_out = (h/2-y)/vy;
      if (t_in>t_out){
        double tmp=t_in;
        t_in=t_out;t_out=tmp;
      }
    } else return 0;
    y_in = vy*t_in + y;
    y_out =vy*t_out + y;

    if ( (y_in > h/2 && y_out > h/2) || (y_in < -h/2 && y_out < -h/2) )
      return 0;
    else
    {
      if (y_in > h/2)
        { t_in = ((h/2)-y)/vy; ret += 2; }
      else if (y_in < -h/2)
        { t_in = ((-h/2)-y)/vy; ret += 4; }
      if (y_out > h/2)
        { t_out = ((h/2)-y)/vy; ret += 8; }
      else if (y_out < -h/2)
        { t_out = ((-h/2)-y)/vy; ret += 16; }
    }
    *t0 = t_in;
    *t1 = t_out;
    return ret;
  }
  else
  {
    *t0 = *t1 = 0;
    return 0;
  }
} /* cylinder_intersect */


/*******************************************************************************
 * sphere_intersect: Calculate intersection between a line and a sphere.
 * returns 0 when no intersection is found
 *      or 1 in case of intersection with resulting times t0 and t1
 *******************************************************************************/
int sphere_intersect(double *t0, double *t1, double x, double y, double z,
                 double vx, double vy, double vz, double r)
{
  double A, B, C, D, v;

  v = sqrt(vx*vx + vy*vy + vz*vz);
  A = v*v;
  B = 2*(x*vx + y*vy + z*vz);
  C = x*x + y*y + z*z - r*r;
  D = B*B - 4*A*C;
  if(D < 0)
    return 0;
  D = sqrt(D);
  *t0 = (-B - D) / (2*A);
  *t1 = (-B + D) / (2*A);
  return 1;
} /* sphere_intersect */

/*******************************************************************************
 * plane_intersect: Calculate intersection between a plane and a line.
 * returns 0 when no intersection is found (i.e. line is parallel to the plane)
 * returns 1 or -1 when intersection time is positive and negative respectively
 *******************************************************************************/
int plane_intersect(double *t, double x, double y, double z,
                 double vx, double vy, double vz, double nx, double ny, double nz, double wx, double wy, double wz)
{
  double s;
  if (fabs(s=scalar_prod(nx,ny,nz,vx,vy,vz))<FLT_EPSILON) return 0;
  *t = - scalar_prod(nx,ny,nz,x-wx,y-wy,z-wz)/s;
  if (*t<0) return -1;
  else return 1;
} /* plane_intersect */

#endif /* !MCSTAS_H */
/* End of file "mcstas-r.c". */


/* *****************************************************************************
* Start of instrument 'ESS_IN5_reprate' generated code
***************************************************************************** */

#ifdef MC_TRACE_ENABLED
int traceenabled = 1;
#else
int traceenabled = 0;
#endif
#define MCSTAS "/opt/homebrew/Caskroom/miniconda/base/envs/mcstas/share/mcstas/resources/"
int   defaultmain         = 1;
char  instrument_name[]   = "ESS_IN5_reprate";
char  instrument_source[] = "/opt/homebrew/Caskroom/miniconda/base/envs/mcstas/share/mcstas/resources/examples/ESS/ESS_IN5_reprate/ESS_IN5_reprate.instr";
char *instrument_exe      = NULL; /* will be set to argv[0] in main */
char  instrument_code[]   = "Instrument ESS_IN5_reprate source code /opt/homebrew/Caskroom/miniconda/base/envs/mcstas/share/mcstas/resources/examples/ESS/ESS_IN5_reprate/ESS_IN5_reprate.instr is not embedded in this executable.\n  Use --source option when running mcstas.\n";

int main(int argc, char *argv[]){return mccode_main(argc, argv);}

/* *****************************************************************************
* instrument 'ESS_IN5_reprate' and components DECLARE
***************************************************************************** */

/* Instrument parameters: structure and a table for the initialisation
   (Used in e.g. inputparse and I/O function (e.g. detector_out) */

struct _struct_instrument_parameters {
  MCNUM Lmin;
  MCNUM Lmax;
  MCNUM lambda0;
  MCNUM Pulse_width;
  MCNUM Num_pulses;
  MCNUM GUI_start;
  MCNUM FO1_DIST;
  MCNUM L_ballistic_begin;
  MCNUM L_ballistic_end;
  MCNUM Length;
  MCNUM SAMPLE_DIST;
  MCNUM DETECTOR_DIST;
  MCNUM GUI_h;
  MCNUM GUI_w;
  MCNUM GUI_GAP;
  MCNUM H1;
  MCNUM W1;
  MCNUM H2;
  MCNUM W2;
  MCNUM H3;
  MCNUM W3;
  MCNUM H4;
  MCNUM W4;
  MCNUM H_chop;
  MCNUM W_chop;
  MCNUM H_end;
  MCNUM W_end;
  MCNUM ALPHA;
  MCNUM M;
  MCNUM F_slow1;
  MCNUM F_slow2;
  MCNUM F_fast1;
  MCNUM F_fast2;
  MCNUM N_fast;
  MCNUM SLOW1_THETA;
  MCNUM FO3;
  MCNUM THETA_fast1;
  MCNUM FAST_THETA;
  MCNUM Gamma;
  MCNUM Etun;
  MCNUM V_HOLE;
  MCNUM FRAC_QUASIEL;
  MCNUM FRAC_TUNNEL;
  MCNUM TT;
  MCNUM RES_DE;
  MCNUM cold;
};
typedef struct _struct_instrument_parameters _class_instrument_parameters;

struct _instrument_struct {
  char   _name[256]; /* the name of this instrument e.g. 'ESS_IN5_reprate' */
/* Counters per component instance */
  double counter_AbsorbProp[52]; /* absorbed events in PROP routines */
  double counter_N[52], counter_P[52], counter_P2[52]; /* event counters after each component instance */
  _class_particle _trajectory[52]; /* current trajectory for STORE/RESTORE */
/* Components position table (absolute and relative coords) */
  Coords _position_relative[52]; /* positions of all components */
  Coords _position_absolute[52];
  _class_instrument_parameters _parameters; /* instrument parameters */
} _instrument_var;
struct _instrument_struct *instrument = & _instrument_var;
#pragma acc declare create ( _instrument_var )
#pragma acc declare create ( instrument )

int numipar = 46;
struct mcinputtable_struct mcinputtable[] = {
  "Lmin", &(_instrument_var._parameters.Lmin), instr_type_double, "4.9", "",
  "Lmax", &(_instrument_var._parameters.Lmax), instr_type_double, "5.1", "",
  "lambda0", &(_instrument_var._parameters.lambda0), instr_type_double, "5", "",
  "Pulse_width", &(_instrument_var._parameters.Pulse_width), instr_type_double, "2.857e-3", "",
  "Num_pulses", &(_instrument_var._parameters.Num_pulses), instr_type_double, "1", "",
  "GUI_start", &(_instrument_var._parameters.GUI_start), instr_type_double, "2.0", "",
  "FO1_DIST", &(_instrument_var._parameters.FO1_DIST), instr_type_double, "6", "",
  "L_ballistic_begin", &(_instrument_var._parameters.L_ballistic_begin), instr_type_double, "19.5", "",
  "L_ballistic_end", &(_instrument_var._parameters.L_ballistic_end), instr_type_double, "17", "",
  "Length", &(_instrument_var._parameters.Length), instr_type_double, "100", "",
  "SAMPLE_DIST", &(_instrument_var._parameters.SAMPLE_DIST), instr_type_double, "1.2", "",
  "DETECTOR_DIST", &(_instrument_var._parameters.DETECTOR_DIST), instr_type_double, "4", "",
  "GUI_h", &(_instrument_var._parameters.GUI_h), instr_type_double, "0.105", "",
  "GUI_w", &(_instrument_var._parameters.GUI_w), instr_type_double, "0.1", "",
  "GUI_GAP", &(_instrument_var._parameters.GUI_GAP), instr_type_double, "0.05", "",
  "H1", &(_instrument_var._parameters.H1), instr_type_double, "0.167", "",
  "W1", &(_instrument_var._parameters.W1), instr_type_double, "0.116", "",
  "H2", &(_instrument_var._parameters.H2), instr_type_double, "0.185", "",
  "W2", &(_instrument_var._parameters.W2), instr_type_double, "0.15", "",
  "H3", &(_instrument_var._parameters.H3), instr_type_double, "0.19", "",
  "W3", &(_instrument_var._parameters.W3), instr_type_double, "0.15", "",
  "H4", &(_instrument_var._parameters.H4), instr_type_double, "0.213", "",
  "W4", &(_instrument_var._parameters.W4), instr_type_double, "0.14", "",
  "H_chop", &(_instrument_var._parameters.H_chop), instr_type_double, "0.075", "",
  "W_chop", &(_instrument_var._parameters.W_chop), instr_type_double, "0.03", "",
  "H_end", &(_instrument_var._parameters.H_end), instr_type_double, "0.042", "",
  "W_end", &(_instrument_var._parameters.W_end), instr_type_double, "0.0215", "",
  "ALPHA", &(_instrument_var._parameters.ALPHA), instr_type_double, "3.4", "",
  "M", &(_instrument_var._parameters.M), instr_type_double, "3.5", "",
  "F_slow1", &(_instrument_var._parameters.F_slow1), instr_type_double, "16.6667", "",
  "F_slow2", &(_instrument_var._parameters.F_slow2), instr_type_double, "0", "",
  "F_fast1", &(_instrument_var._parameters.F_fast1), instr_type_double, "0", "",
  "F_fast2", &(_instrument_var._parameters.F_fast2), instr_type_double, "200", "",
  "N_fast", &(_instrument_var._parameters.N_fast), instr_type_double, "1", "",
  "SLOW1_THETA", &(_instrument_var._parameters.SLOW1_THETA), instr_type_double, "120", "",
  "FO3", &(_instrument_var._parameters.FO3), instr_type_double, "1", "",
  "THETA_fast1", &(_instrument_var._parameters.THETA_fast1), instr_type_double, "180", "",
  "FAST_THETA", &(_instrument_var._parameters.FAST_THETA), instr_type_double, "5", "",
  "Gamma", &(_instrument_var._parameters.Gamma), instr_type_double, "0", "",
  "Etun", &(_instrument_var._parameters.Etun), instr_type_double, "1", "",
  "V_HOLE", &(_instrument_var._parameters.V_HOLE), instr_type_double, "0", "",
  "FRAC_QUASIEL", &(_instrument_var._parameters.FRAC_QUASIEL), instr_type_double, "0", "",
  "FRAC_TUNNEL", &(_instrument_var._parameters.FRAC_TUNNEL), instr_type_double, "0", "",
  "TT", &(_instrument_var._parameters.TT), instr_type_double, "50", "",
  "RES_DE", &(_instrument_var._parameters.RES_DE), instr_type_double, "0.5", "",
  "cold", &(_instrument_var._parameters.cold), instr_type_double, "0.95", "",
  NULL, NULL, instr_type_double, ""
};

struct metadata_table_struct metadata_table[] = {
  "", "", "", ""
};
int num_metadata = 0;

/* ************************************************************************** */
/*             SHARE user declarations for all components                     */
/* ************************************************************************** */

/* Shared user declarations for all components types 'ESS_butterfly'. */
  #ifdef ADJUSTABLE_ESS_PULSE
  #ifndef ESS_SOURCE_DURATION
  #define ESS_SOURCE_DURATION _instrument_var._parameters.pulse_duration
  #endif
  #endif
/*******************************************************************************
*
* McStas, neutron ray-tracing package
*         Copyright 1997-2013, All rights reserved
*         DTU Physics, Lyngby, Denmark
*         Institut Laue Langevin, Grenoble, France
*
* Library: share/ESS_butterfly-lib.h
*
* %Identification
* Written by: PW
* Date: Nov 7, 2013
* Origin: DTU Physics
* Release: McStas 2.1
* Version: 0.1
*
* This file is to be imported by the ESS_moderator_long component
* It defines a set of brilliance definitions (used via function pointer) for
* easier use of the component.
*
* Usage: within SHARE
* %include "ESS_butterfly-lib"
*
*******************************************************************************/

#ifndef ESS_BUTTERFLY_LIB_H
#define ESS_BUTTERFLY_LIB_H 0.1

#ifdef ADJUSTABLE_ESS_PULSE
#ifndef ESS_SOURCE_DURATION
#define ESS_SOURCE_DURATION _instrument_var._parameters.pulse_duration
#endif
#endif

#ifndef ESS_SOURCE_DURATION
#define ESS_SOURCE_DURATION 2.857e-3
#endif

#ifndef ESS_SOURCE_FREQUENCY
#define ESS_SOURCE_FREQUENCY 14
#endif

#ifndef ESS_SOURCE_POWER
#define ESS_SOURCE_POWER 5
#endif

/* Struct for extra source parameters - for future geometrical adjustments */
struct ess_struct {
  double X;
  double Y;
  double Z;
  double height_t;
  double height_c;
  double Width_c;
  double Width_t;
  double Mwidth_c;
  double Mwidth_t;
  double tmultiplier;
  double Radius_c;
  double beamportangle;
  int Uniform;
  double extractionangle;
  int Wasleft;
};
typedef struct ess_struct ess_moderator_struct;

typedef void (*functype)(double* t , double* p, double lambda,  double tfocus_w, double tfocus_t, double tfocus_dt, ess_moderator_struct extras);

double ESS_2015_Schoenfeldt_cold_spectrum(double lambda,double theta);
double ESS_2015_Schoenfeldt_thermal_spectrum(double lambda, double theta);

/* List of brilliance definitions */
void ESS_2015_Schoenfeldt_cold(double *t, double *p, double lambda, double tfocus_w, double tfocus_t, double tfocus_dt, double height_t, double Mwidth_t, double height_c, double Mwidth_c, double tmultiplier, double beamportangle, double X, double Y);
void ESS_2015_Schoenfeldt_thermal(double *t, double *p, double lambda, double tfocus_w, double tfocus_t, double tfocus_dt, double height_t, double Mwidth_t, double height_c, double Mwidth_c, double tmultiplier, double beamportangle, double X, double Y);
/* List of pulse-shape definitions */
double ESS_2015_Schoenfeldt_cold_timedist(double t, double lambda, double height, double pulselength);
double ESS_2015_Schoenfeldt_thermal_timedist(double t, double lambda, double height, double pulselength);

/* List of moderator-geometry-weighting definitions */
double ESS_2014_Schoenfeldt_cold_y0(double y0,double height);
double ESS_2014_Schoenfeldt_cold_x0(double x0,double height, double width);
double ESS_2014_Schoenfeldt_thermal_y0(double y0,double height);
double ESS_2014_Schoenfeldt_thermal_x0(double x0,double height, double width);

double ESS_2015_Schoenfeldt_cold_y0(double y0);
double ESS_2015_Schoenfeldt_cold_x0(double x0, double theta, double width);
double ESS_2015_Schoenfeldt_thermal_y0(double y0);
double ESS_2015_Schoenfeldt_thermal_x0(double x0,double theta, double width);
double ESS_2015_Schoenfeldt_cold_Y(double x0,double height);
double ESS_2015_Schoenfeldt_thermal_Y(double y0,double height);
double ESS_2015_Schoenfeldt_cold_Theta120(double x0,double height);
double ESS_2015_Schoenfeldt_thermal_Theta120(double beamportangle,int isleft);

/* end of ESS_butterfly-lib.h */
#endif

/*******************************************************************************
*
* McStas, neutron ray-tracing package
*         Copyright 1997-2013, All rights reserved
*         DTU Physics, Lyngby, Denmark
*         Institut Laue Langevin, Grenoble, France
*
* Library: share/ESS_butterfly-lib.c
*
* %Identification
* Written by: PW
* Date: Nov 7, 2013
* Origin: DTU Physics
* Release: McStas 2.1
* Version: 0.1
*
* This file is to be imported by the ESS_moderator_long component
* It defines a set of brilliance definitions (used via function pointer) for
* easier use of the component.
*
* Usage: within SHARE
* %include "ESS_butterfly-lib"
*
*******************************************************************************/

#ifndef ESS_BUTTERFLY_LIB_H
#error McStas : please import this library with %include "ESS_butterfly-lib"
#endif

#ifdef ADJUSTABLE_ESS_PULSE
#ifndef ESS_SOURCE_DURATION
#define ESS_SOURCE_DURATION _instrument_var._parameters.pulse_duration
#endif
#endif

#ifdef OPENACC
#define exit(...) noprintf()
#endif

#pragma acc routine seq
double ESS_2015_Schoenfeldt_cold_spectrum(double lambda,double theta){
  if(lambda<=0)return 0;
  double par0=8.44e13/25.;
  double par1=2.5;
  double par2=2.2;
  
  double par3=-13.-.5*(theta-5);
  double par4=2.53;
  double par5=-0.0478073-0.160*exp(-0.45186*(theta-5.)/10.);
  
  double par6;
  if(theta==5)par6=5.73745e+015/25.;
  else if(theta==15)par6=5.88284e+015/25.;
  else if(theta==25)par6=6.09573e+015/25.;
  else if(theta==35)par6=6.29116e+015/25.;
  else if(theta==45)par6=6.03436e+015/25.;
  else if(theta==55)par6=6.02045e+015/25.;
  double par7=0.788956+0.00854184*(theta-5.)/10.;
  double par8=0.0461868-0.0016464*(theta-5.)/10.;
  double par9=0.325;
  
  double SD_part=par0/((1+exp(par1*(lambda-par2)))*lambda);
  double para_part=pow((1+exp(par3*(lambda-par4))),par5)*(par6*(exp(-par7*(lambda))+par8*exp(-par9*(lambda))));
  return para_part+SD_part;
  
}
#pragma acc routine seq
double ESS_2015_Schoenfeldt_thermal_spectrum(double lambda, double theta){
    if(lambda<=0)return 0;
    double i=(theta-5.)/10.;
    double par0=4.2906e+013-9.2758e+011*i+8.02603e+011*i*i-1.29523e+011*i*i*i;
    double par2=6.24806e+012-8.84602e+010*i;
    double par3=-0.31107+0.0221138*i;
    double aOlsqr=949./(325*lambda*lambda);
    return par0*2.*aOlsqr*aOlsqr/lambda*pow(lambda,-par3)*exp(-aOlsqr)+par2/((1+exp(2.5*(lambda-0.88)))*lambda);
	  
}


/* This is ESS_2014_Schoenfeldt_cold_y0 - vertical intensity distribution for the 2014 Schoenfeldt cold moderator */
#pragma acc routine seq
double ESS_2014_Schoenfeldt_cold_y0(double y0,double height){
  
  double one_over_integral_y0_of_height= height/((0.36434*height*height+2.53796*height-0.107774));
  if(y0 < -height/2. || y0 > height/2. )return 0;
  double cosh_ish=(exp(-7e-1/sqrt(height)*(y0-height/2.))+exp(-7e-1/20.*height+7e-1/sqrt(height)*(y0+height/2.)));
  double sinh_ish=(exp(50/sqrt(height)*(y0-height/2.))-1)*(exp(-50/sqrt(height)*(y0+height/2.))-1);
  double tmp=one_over_integral_y0_of_height*cosh_ish*sinh_ish;
  return tmp;
} /* end of ESS_2014_Schoenfeldt_cold_y0 */

/* This is ESS_2014_Schoenfeldt_thermal_y0 - vertical intensity distribution for the 2014 Schoenfeldt cold moderator */
#pragma acc routine seq
double ESS_2014_Schoenfeldt_thermal_y0(double y0,double height){
  /* Placeholder - we assume that this distribution is flat for now */
  return 1;
} /* end of ESS_2014_Schoenfeldt_thermal_y0 */

/* This is ESS_2014_Schoenfeldt_cold_x0 - horizontal intensity distribution for the 2014 Schoenfeldt cold moderator */
#pragma acc routine seq
double ESS_2014_Schoenfeldt_cold_x0(double x0,double height, double width){
  double normalization=1;
  if(x0<-width||x0>width)return 0;
  return normalization*(0.008*x0+1)*(exp(height/2.*(x0-width/2))-1)*(exp(-height/2.*(x0+width/2))-1);
} /* end of ESS_2014_Schoenfeldt_cold_x0 */

/* This is ESS_2014_Schoenfeldt_thermal_x0 - horizontal intensity distribution for the 2014 Schoenfeldt cold moderator */

double ESS_2014_Schoenfeldt_thermal_x0(double x0,double height, double width){
  // Kept for reference only...
  /* if(x0>-width&&x0<width)return 0; */
  /* if(x0<0)return fmax(0,2.5*(0.0524986*fabs(x0)-1.84817-0.0189762*height+(-1.49712e+002*exp(-4.06814e-001*height))*exp(-4.48657e-001*fabs(x0)))*(exp(7*(x0+width))-1)); */
  /* return fmax(0,2.5*(0.84199+0.00307022*height)*(0.0524986*fabs(x0)-1.84817-0.0189762*height+(-1.49712e+002*exp(-4.06814e-001*height))*exp(-4.48657e-001*fabs(x0)))*(exp(-7*(x0-width))-1)); */  
  if(x0>-23./2.&&x0<23./2.)return 0;
  long double cosh_ish=fmin(0.0524986*fabs(x0)-1.84817-0.0189762*height+(-1.49712e+002*exp(-4.06814e-001*height))*exp(-4.48657e-001*fabs(x0)),0);
  if(x0<0)return (-1.73518e-003*height*height+2.10277e-002*height+7.65692e-001) // intensity
	    *cosh_ish*(exp(7.*(x0+23./2.))-1); // slope 
  return (-1.73518e-003*height*height+2.10277e-002*height+7.65692e-001) // intensity
    *(0.84199+0.00307022*height) // asumetry
    *cosh_ish*(exp(-7.*(x0-23./2.))-1); // slope
} /* end of ESS_2014_Schoenfeldt_thermal_x0 */

/* This is the thermal moderator with 2015 updates, fits from Troels Schoenfeldt */
#pragma acc routine seq
void ESS_2015_Schoenfeldt_thermal(double *t, double *p, double lambda, double tfocus_w, double tfocus_t, double tfocus_dt, double height_t, double Mwidth_t, double height_c, double Mwidth_c, double tmultiplier, double beamportangle, double X, double Y)
{
  if ((height_t == 0.03) || (height_t == 0.06)) {
    *p = ESS_2015_Schoenfeldt_thermal_spectrum(lambda, beamportangle);
  } else {
    printf("Sorry! Moderator height must be either %g or %g m\n",0.03,0.06);
    exit(-1);
  }

  /* Troels Schoenfeldt function for timestructure */
  *p *= tmultiplier*ESS_2015_Schoenfeldt_thermal_timedist(*t, lambda, 3 /* cm height */, ESS_SOURCE_DURATION);  
  if (height_c == 0.03) {
    // 3cm case
    *p *= ESS_2015_Schoenfeldt_thermal_y0(100*Y) * ESS_2015_Schoenfeldt_thermal_x0(100*X, beamportangle, Mwidth_t);
  } else {
    // 6cm case
    // Downscale brightness by factor from 
    // "New ESS Moderator Baseline", Ken Andersen, 9/4/2015
    *p *= (6.2e14/9.0e14);
    *p *= ESS_2014_Schoenfeldt_thermal_y0(100*Y, 100*height_c) * ESS_2015_Schoenfeldt_thermal_x0(100*X, beamportangle, Mwidth_t);
  }
} /* end of ESS_2015_Schoenfeldt_thermal */


/* This is the cold moderator with 2015 updates, fits from Troels Schoenfeldt */
/* Parametrization including moderator height for the "pancake" moderator */
#pragma acc routine seq
void ESS_2015_Schoenfeldt_cold(double *t, double *p, double lambda, double tfocus_w, double tfocus_t, double tfocus_dt, double height_t, double Mwidth_t, double height_c, double Mwidth_c, double tmultiplier, double beamportangle, double X, double Y)
{
   if ((height_c == 0.03) || (height_c == 0.06)) {
    *p = ESS_2015_Schoenfeldt_cold_spectrum(lambda,beamportangle);
  } else {
    printf("Sorry! Moderator height must be either %g or %g m\n",0.03,0.06);
    exit(-1);
  }

  /* Troels Schoenfeldt function for timestructure */
  *p *= tmultiplier*ESS_2015_Schoenfeldt_cold_timedist(*t, lambda, 3 /* cm height */, ESS_SOURCE_DURATION);
  
  if (height_c == 0.03) {
    // 3cm case
    *p *= ESS_2015_Schoenfeldt_cold_y0(100*Y) * ESS_2015_Schoenfeldt_cold_x0(100*X, beamportangle, Mwidth_c);
  } else {
    // 6cm case
    // Downscale brightness by factor from 
    // "New ESS Moderator Baseline", Ken Andersen, 9/4/2015
    *p *= (10.1e14/16.0e14);
    *p *= ESS_2014_Schoenfeldt_cold_y0(100*Y, 100*height_c) * ESS_2015_Schoenfeldt_cold_x0(100*X, beamportangle, Mwidth_c);
  }
} /* end of ESS_2015_Schoenfeldt_cold */

/* This is ESS_2015_Schoenfeldt_cold_y0 - vertical intensity distribution for the 2015 Schoenfeldt cold moderator */
#pragma acc routine seq
double ESS_2015_Schoenfeldt_cold_y0(double y0){
    double par3=30;
    double par4=.35;
    double cosh_ish=exp(-par4*y0)+exp(par4*y0);
    double sinh_ish=pow(1+exp(par3*(y0-3./2.)),-1)*pow(1+exp(-par3*(y0+3./2.)),-1);
    return 1./2.*(double)((double)cosh_ish*(double)sinh_ish);

} /* end of ESS_2015_Schoenfeldt_cold_y0 */

/* This is ESS_2015_Schoenfeldt_thermal_y0 - vertical intensity distribution for the 2015 Schoenfeldt cold moderator */
#pragma acc routine seq
double ESS_2015_Schoenfeldt_thermal_y0(double y0){
    if(y0<-3./2.+0.105){
        return 1.005*exp(-pow((y0+3./2.-0.105)/0.372,2));
    } else if(y0>3./2.-0.105){
        return 1.005*exp(-pow((y0-3./2.+0.105)/0.372,2));
    }
    return 1.005;
} /* end of ESS_2015_Schoenfeldt_thermal_y0 */

/* This is ESS_2015_Schoenfeldt_cold_x0 - horizontal intensity distribution for the 2015 Schoenfeldt cold moderator */
#pragma acc routine seq
double ESS_2015_Schoenfeldt_cold_x0(double x0,double theta, double width){
  // GEOMETRY / SAMPLING SPACE
    double i=(theta-5.)/10.;
    double par0=0.0146115+0.00797729*i-0.00279541*i*i;
    double par1=0.980886;
    if(i==1)par1=0.974217;
    if(i==2)par1=0.981462;
    if(i==3)par1=1.01466;
    if(i==4)par1=1.11707;
    if(i==5)par1=1.16057;
        
    double par2=-4-.75*i;
    if(i==0)par2=-20;
    double par3=-14.9402-0.178369*i+0.0367007*i*i;
    if(i==0)par3*=0.95;
    double par4=-15;
    if(i==3)par4=-3.5;
    if(i==5)par4=-1.9;
    double par5=-7.07979+0.0835695*i-0.0546662*i*i;
    if(i==5)par5*=0.85;
    
    //printf("Angle %g, width is %g\n",theta,width,cos(theta*DEG2RAD)*width);
    //if(i==4) width=width+0.3;
    //if(i==5) width=width-0.7;

    /* Rescaling to achieve a BF1 model */
    double tmp=(par5-par3)/width;
    //printf("Cold x0 in BF1 units: %g,",x0);
    x0=x0*tmp-7.16;
    //printf("x0 in BF2 units: %g, moderator width is %g from %g\n",x0,width,par5-par3);

    /* if (x0<=par5 && x0>=par3) */
    /*   return 1; */
    /* else */
    /*   return 0; */
    

    double line=par0*(x0+12)+par1;
    double CutLeftCutRight=1./((1+exp(par2*(x0-par3)))*(1+exp(-par4*(x0-par5))));

    return line*CutLeftCutRight;
} /* end of ESS_2015_Schoenfeldt_cold_x0 */

/* This is ESS_2015_Schoenfeldt_thermal_x0 - horizontal intensity distribution for the 2015 Schoenfeldt cold moderator */
#pragma acc routine seq
double ESS_2015_Schoenfeldt_thermal_x0(double x0,double theta, double width){
    double i=(theta-5.)/10.;
    double par0=-5.54775+0.492804*i;
    double par1=-0.265929-0.711477*i;
    if(theta==55)par1=-2.55;

    double par2=0.821885+0.00914832*i;
    double par3=1.31108-0.00698647*i;
    if(theta==55)par3=1.23;
    double par4=-.035;
    double par5=-0.0817358+0.00807125*i;
        
    double par6=-8;
    double par7=-7.15;
    if(theta==45)par7=-8.2;
    if(theta==55)par7=-7.7;

    double par8=-8;
    double par9=7.15;
    if(theta==45)par9=7.5;
    if(theta==55)par9=8.2;

    /* Rescaling to achieve a BF1 model */
    double tmp=(par9-par7)/width;
    //printf("Thermal x0 in BF1 units: %g,",x0);
    x0=x0*tmp-7.16;
    //printf(" x0 in BF2 units: %g, moderator width is %g from %g\n",x0,width,par9-par7);
    
    /* if (x0<=par9 && x0>=par7) */
    /*   return 1; */
    /* else */
    /*   return 0; */
    
    double soften1=1./(1+exp(8.*(x0-par0)));
    double soften2=1./(1+exp(8.*(x0-par1)));
    double CutLeftCutRight=1./((1+exp(par6*(x0-par7)))*(1+exp(-par8*(x0-par9))));
    double line1=par4*(x0-par0)+par2;
    double line2=(par2-par3)/(par0-par1)*(x0-par0)+par2;
    double line3=par5*(x0-par1)+par3;
    double add45degbumb=1.2*exp(-(x0+7.55)*(x0+7.55)/.35/.35);


    return CutLeftCutRight*(
        (line1)*soften1
        +line2*soften2*(1-soften1)
        +line3*(1-soften2)
        );
} /* end of ESS_2015_Schoenfeldt_thermal_x0 */

/* This is ESS_2015_Schoenfeldt_cold_Y - vertical intensity distribution for the 2015 Schoenfeldt cold moderator */
#pragma acc routine seq
double ESS_2015_Schoenfeldt_cold_Y(double Y,double height){
  /* Placeholder - we assume that this distribution is flat for now */
  return 1;
} /* end of ESS_2015_Schoenfeldt_cold_Y */

/* This is ESS_2015_Schoenfeldt_thermal_Y - vertical intensity distribution for the 2015 Schoenfeldt cold moderator */
#pragma acc routine seq
double ESS_2015_Schoenfeldt_thermal_Y(double Y,double height){
  /* Placeholder - we assume that this distribution is flat for now */
  return 1;
} /* end of ESS_2015_Schoenfeldt_thermal_Y */

/* This is ESS_2015_Schoenfeldt_cold_Theta120 - vertical intensity distribution for the 2015 Schoenfeldt cold moderator */
#pragma acc routine seq
double ESS_2015_Schoenfeldt_cold_Theta120(double Theta120,double height){
  /* Placeholder - we assume that this distribution is flat for now */
  return 1;
} /* end of ESS_2015_Schoenfeldt_cold_Theta120 */

/* This is ESS_2015_Schoenfeldt_thermal_Theta120 - vertical intensity distribution for the 2015 Schoenfeldt cold moderator */
#pragma acc routine seq
double ESS_2015_Schoenfeldt_thermal_Theta120(double beamportangle,int isleft){
  if(!isleft)return cos((beamportangle-30)*DEG2RAD)/cos(30*DEG2RAD);
  return cos((90-beamportangle)*DEG2RAD)/cos(30*DEG2RAD);
/* Placeholder - we assume that this distribution is flat for now */
  return 1;
} /* end of ESS_2015_Schoenfeldt_thermal_Theta120 */


/* This is ESS_2015_Schoenfeldt_cold_timedist time-distribution of the 2014 Schoenfeldt cold moderator */ 
#pragma acc routine seq
double ESS_2015_Schoenfeldt_cold_timedist(double time,double lambda,double height, double pulselength){
        if(time<0)return 0;
        double tau=3.00094e-004*(4.15681e-003*lambda*lambda+2.96212e-001*exp(-1.78408e-001*height)+7.77496e-001)*exp(-6.63537e+001*pow(fmax(1e-13,lambda+.9),-8.64455e+000));
        if(time<pulselength)return ((1-exp(-time/tau)));
        return ((1-exp(-pulselength/tau))*exp(-(time-pulselength)/tau));

} /* end of ESS_2015_Schoenfeldt_cold_timedist */

/* This is ESS_2015_Schoenfeldt_thermal_timedist time-distribution of the 2015 Schoenfeldt cold moderator */    
#pragma acc routine seq
double ESS_2015_Schoenfeldt_thermal_timedist(double time,double lambda,double height, double pulselength){
        if(time<0)return 0;
        double tau=3.00000e-004*(1.23048e-002*lambda*lambda+1.75628e-001*exp(-1.82452e-001*height)+9.27770e-001)*exp(-3.91090e+001*pow(fmax(1e-13,lambda+9.87990e-001),-7.65675e+000));
        if(time<pulselength)return ((1-exp(-time/tau)));
        return ((1-exp(-pulselength/tau))*exp(-(time-pulselength)/tau));
} /* end of ESS_2015_Schoenfeldt_thermal_timedist */

/* end of ESS_butterfly-lib.c */
#ifdef OPENACC
#undef exit
#endif

/* MCDISPLAY-section for the ESS butterfly moderator */

/* NOTA BENE:
   From McStas 3, this file should be included in %SHARE due to requirement  from clang / macOS, that dislikes function definitons in DISPLAY.
   (clang is more c99-strict than e.g. gcc)

   For this reason, the geometry-calls e.g. line() have to be explicitly the mcdis_line() variant */


/* Define the positioning of the buttefly sketch */

/* Point sets for the butterfly */
double butterfly_z[] = {-1.9922764e-01, -1.8484553e-01, -2.0252845e-01, -2.0795122e-01, -2.1054471e-01, -2.1030894e-01, -2.0889431e-01, -2.0535772e-01, -2.0134959e-01, -1.9639837e-01, -1.9026829e-01, -1.8390244e-01, -1.7565041e-01, -1.7093496e-01, -1.4617886e-01, -1.2873171e-01, -9.2658537e-02, -4.0552845e-02, -1.7682927e-02, -9.1951221e-03, -2.3577231e-03, 5.1869889e-03, 1.1788619e-02, 1.7918699e-02, 2.3105689e-02, 2.4991869e-02, 2.4756099e-02, 2.2162599e-02, 1.8154469e-02, 1.2731709e-02, 5.6585389e-03, -2.3577214e-04, 1.4146339e-02, -2.9707317e-02, 1.4146339e-02, -4.7154514e-04, 1.5325199e-02, 2.1691059e-02, 2.4991869e-02, 2.4991869e-02, 2.2634149e-02, 1.8154469e-02, 1.1552849e-02, 3.3008089e-03, -6.1300811e-03, -9.1951221e-03, -6.0593496e-02, -9.2658537e-02, -1.7541463e-01, -1.8508130e-01, -1.9309756e-01, -1.9899187e-01, -2.0182114e-01, -2.0582927e-01, -2.0913008e-01, -2.1078049e-01, -2.1007317e-01, -2.0630081e-01, -2.0229268e-01, -1.9828455e-01, -1.8484553e-01, -1.9922764e-01, -1.5584553e-01, -1.9922764e-01};
#pragma acc declare create(butterfly_z)

double butterfly_x[] = {1.4279319e-02, 3.1692034e-10, -1.7654432e-02, -2.5962400e-02, -3.4789615e-02, -4.3876452e-02, -5.1665172e-02, -5.8934642e-02, -6.4646372e-02, -6.9059982e-02, -7.2694722e-02, -7.5031332e-02, -7.6589082e-02, -7.6589082e-02, -7.6848702e-02, -7.6589082e-02, -7.6589082e-02, -7.6589082e-02, -7.6589082e-02, -7.6848702e-02, -7.5290962e-02, -7.2694722e-02, -6.8281112e-02, -6.1790512e-02, -5.3482542e-02, -4.4136082e-02, -3.3491495e-02, -2.5443152e-02, -1.9212176e-02, -1.2981200e-02, -6.2309757e-03, -2.5962368e-04, 1.4538943e-02, 5.8415398e-02, 1.0229185e-01, 1.1709042e-01, 1.3266786e-01, 1.4149508e-01, 1.5213966e-01, 1.6200537e-01, 1.7135184e-01, 1.7888093e-01, 1.8589078e-01, 1.9082364e-01, 1.9290063e-01, 1.9341988e-01, 1.9341988e-01, 1.9341988e-01, 1.9341988e-01, 1.9186213e-01, 1.8822740e-01, 1.8459266e-01, 1.8069830e-01, 1.7602507e-01, 1.6849597e-01, 1.5811101e-01, 1.4928380e-01, 1.3993733e-01, 1.3370636e-01, 1.2981200e-01, 1.1709042e-01, 1.0229185e-01, 5.8415398e-02, 1.4279319e-02};
#pragma acc declare create(butterfly_x)

double butterfly_e_z1[]= {-3.0488e-04,  -5.5017e-02, -3.0488e-04};
#pragma acc declare create(butterfly_e_z1)
double butterfly_e_x1[]= {-4.3103e-04,   5.8521e-02,  1.1701e-01};
#pragma acc declare create(butterfly_e_x1)
double butterfly_e_z2[]= {-1.8501e-01, -1.2719e-01, -1.8501e-01};
#pragma acc declare create(butterfly_e_z2)
double butterfly_e_x2[]= {3.3156e-05,  5.8985e-02,  1.1701e-01};
#pragma acc declare create(butterfly_e_x2)

void butterfly_geometry(double Bdelta_y, int Bjmax, double Bcx, double Bcz,
  double Borientation_angle, double *BBeamlines, double Btx, double Bty, double Btz,
  double BrC1_x, double BrC1_z, 
  double BrC2_x, double BrC2_z, 
  double BrC3_x, double BrC3_z,  
  double BrT1_x, double BrT1_z, 
  double BrT2_x, double BrT2_z, 
  double BrT3_x, double BrT3_z,
  double Br11, double Br12, double Br21, double Br22,
  double Bfoc_XW, double Bfoc_YH)
{
  /* Draw the two butterfly shapes at top and bottom level */
  double y0;
  int j;
  double rAx,rAz,rBx,rBz;

  for (y0=-Bdelta_y; y0<2*Bdelta_y; y0+=2*Bdelta_y) {
    for (j=0; j<63; j++) {
      
      rAx = Br11*(butterfly_z[j]-Bcz) + Br12*(butterfly_x[j]-Bcx);
      rAz = Br21*(butterfly_z[j]-Bcz) + Br22*(butterfly_x[j]-Bcx);

      rBx = Br11*(butterfly_z[j+1]-Bcz) + Br12*(butterfly_x[j+1]-Bcx);
      rBz = Br21*(butterfly_z[j+1]-Bcz) + Br22*(butterfly_x[j+1]-Bcx);

      mcdis_line(rAx, y0, rAz, rBx, y0, rBz);

    }
  }

  /* Draw the "border" between the thermal and cold areas */
  for (y0=-Bdelta_y; y0<2*Bdelta_y; y0+=2*Bdelta_y) {
    for (j=0; j<2; j++) {
      
      rAx = Br11*(butterfly_e_z1[j]-Bcz) + Br12*(butterfly_e_x1[j]-Bcx);
      rAz = Br21*(butterfly_e_z1[j]-Bcz) + Br22*(butterfly_e_x1[j]-Bcx);

      rBx = Br11*(butterfly_e_z1[j+1]-Bcz) + Br12*(butterfly_e_x1[j+1]-Bcx);
      rBz = Br21*(butterfly_e_z1[j+1]-Bcz) + Br22*(butterfly_e_x1[j+1]-Bcx);

      mcdis_line(rAx, y0, rAz, rBx, y0, rBz);

      rAx = Br11*(butterfly_e_z2[j]-Bcz) + Br12*(butterfly_e_x2[j]-Bcx);
      rAz = Br21*(butterfly_e_z2[j]-Bcz) + Br22*(butterfly_e_x2[j]-Bcx);

      rBx = Br11*(butterfly_e_z2[j+1]-Bcz) + Br12*(butterfly_e_x2[j+1]-Bcx);
      rBz = Br21*(butterfly_e_z2[j+1]-Bcz) + Br22*(butterfly_e_x2[j+1]-Bcx);

      mcdis_line(rAx, y0, rAz, rBx, y0, rBz);
    }
  }

  /* Indicate the emission planes of cold/thermal moderator */
  for (y0=-Bdelta_y; y0<2*Bdelta_y; y0+=2*Bdelta_y) {
    mcdis_dashed_line(BrC1_x, y0, BrC1_z, BrC2_x, y0, BrC2_z, 11);
    mcdis_dashed_line(BrC1_x, y0, BrC1_z, BrC3_x, y0, BrC3_z, 11);
    mcdis_dashed_line(BrT1_x, y0, BrT1_z, BrT2_x, y0, BrT2_z, 11);
    mcdis_dashed_line(BrT1_x, y0, BrT1_z, BrT3_x, y0, BrT3_z, 11);
  }
  mcdis_dashed_line(BrC1_x, -Bdelta_y, BrC1_z, BrC1_x, Bdelta_y, BrC1_z, 11);
  mcdis_dashed_line(BrC2_x, -Bdelta_y, BrC2_z, BrC2_x, Bdelta_y, BrC2_z, 11);
  mcdis_dashed_line(BrC3_x, -Bdelta_y, BrC3_z, BrC3_x, Bdelta_y, BrC3_z, 11);
  mcdis_dashed_line(BrT1_x, -Bdelta_y, BrT1_z, BrT1_x, Bdelta_y, BrT1_z, 11);
  mcdis_dashed_line(BrT2_x, -Bdelta_y, BrT2_z, BrT2_x, Bdelta_y, BrT2_z, 11);
  mcdis_dashed_line(BrT3_x, -Bdelta_y, BrT3_z, BrT3_x, Bdelta_y, BrT3_z, 11);


  /* Arrow indicating proton beam direction */
  double ax,az,bx,bz,bbx,bbz,cBcx,cBcz;
  az = -0.0925-Bcz;
  ax = 0.0585-Bcx;
  bz = -0.0925-Bcz;
  bx = 0.0585+6-Bcx;
  bbx = 0.0585+0.1-Bcx;
  bbz = -0.0925+0.03-Bcz;
  cBcx = 0.0585+0.1-Bcx;
  cBcz = -0.0925-0.03-Bcz;
  /* rAx,0,rAz is the centre of the moderator */
  rAx = Br11*(az) + Br12*(ax);
  rAz = Br21*(az) + Br22*(ax);
  rBx = Br11*(bz) + Br12*(bx);
  rBz = Br21*(bz) + Br22*(bx);
  /* Main part of the arrow */
  mcdis_line(rAx, 0, rAz, rBx, 0, rBz);
  /* Inclined lines for arrow head */
  rBx = Br11*(bbz) + Br12*(bbx);
  rBz = Br21*(bbz) + Br22*(bbx);
  mcdis_line(rAx, 0, rAz, rBx, 0, rBz);
  rBx = Br11*(cBcz) + Br12*(cBcx);
  rBz = Br21*(cBcz) + Br22*(cBcx);
  mcdis_line(rAx, 0, rAz, rBx, 0, rBz);

  /* 120 degree "end of sector" lines */
  bbz = 2 * cos(DEG2RAD*61);
  bbx = 2 * sin(DEG2RAD*61);
  cBcz = 2 * cos(-DEG2RAD*61);
  cBcx = 2 * sin(-DEG2RAD*61);
  rBx = Br11*(bbz) + Br12*(bbx);
  rBz = Br21*(bbz) + Br22*(bbx);
  mcdis_dashed_line(rAx, 0, rAz, rBx+rAx, 0, rBz+rAz,51);
  rBx = Br11*(cBcz) + Br12*(cBcx);
  rBz = Br21*(cBcz) + Br22*(cBcx);
  mcdis_dashed_line(rAx, 0, rAz, rBx+rAx, 0, rBz+rAz,51);
  bbz = 2 * cos(DEG2RAD*119);
  bbx = 2 * sin(DEG2RAD*119);
  cBcz = 2 * cos(-DEG2RAD*119);
  cBcx = 2 * sin(-DEG2RAD*119);
  rBx = Br11*(bbz) + Br12*(bbx);
  rBz = Br21*(bbz) + Br22*(bbx);
  mcdis_dashed_line(rAx, 0, rAz, rBx+rAx, 0, rBz+rAz,51);
  rBx = Br11*(cBcz) + Br12*(cBcx);
  rBz = Br21*(cBcz) + Br22*(cBcx);
  mcdis_dashed_line(rAx, 0, rAz, rBx+rAx, 0, rBz+rAz,51);
  /* Circles indicating extent of the "empBty" zone where optics is not allowed */
  mcdis_circle("xz", rAx, 0, rAz, 2.0);
  mcdis_circle("xz", rAx, -0.1, rAz, 2.0);
  mcdis_circle("xz", rAx, 0.1, rAz, 2.0);

  /* Circles indicating extent of the target monolith */
  mcdis_circle("xz", rAx, 0, rAz, 5.5);
  mcdis_circle("xz", rAx, -1, rAz, 5.5);
  mcdis_circle("xz", rAx, 1, rAz, 5.5);

  /* Beamport "plug" dimensions */
  double w1=0.206/2.0, w2=0.276/2.0, l1=2.0+rAz, l2=2.0+rAz+1.75, l3=2.0+rAz+3.5;
  mcdis_line(w1, 0, l1, w1, 0, l2);
  mcdis_line(-w1, 0, l1, -w1, 0, l2);
  mcdis_line(w1, 0, l2, w2, 0, l2);
  mcdis_line(-w1, 0, l2, -w2, 0, l2);
  mcdis_line(w2, 0, l2, w2, 0, l3);
  mcdis_line(-w2, 0, l2, -w2, 0, l3);

  /* Draw all the BBeamlines in "this sector" +1 */
  double xx1, yy1, zz1, xx2, yy2, zz2, delta_omega;
  for (j=0; j<Bjmax+1; j++) {
    delta_omega = Borientation_angle - BBeamlines[j];
    Br11 = cos(DEG2RAD*delta_omega);
    Br12 = -sin(DEG2RAD*delta_omega);
    Br21 = sin(DEG2RAD*delta_omega);
    Br22 = cos(DEG2RAD*delta_omega);
    xx1 = Br11*(w1) + Br12*(l1);
    zz1 = Br21*(w1) + Br22*(l1);
    xx2 = Br11*(w1) + Br12*(l2);
    zz2 = Br21*(w1) + Br22*(l2);
    mcdis_dashed_line(xx1, 0, zz1, xx2, 0, zz2, 11);
    xx1 = Br11*(-w1) + Br12*(l1);
    zz1 = Br21*(-w1) + Br22*(l1);
    xx2 = Br11*(-w1) + Br12*(l2);
    zz2 = Br21*(-w1) + Br22*(l2);
    mcdis_dashed_line(xx1, 0, zz1, xx2, 0, zz2, 11);
    xx1 = Br11*(w2) + Br12*(l2);
    zz1 = Br21*(w2) + Br22*(l2);
    xx2 = Br11*(w2) + Br12*(l3);
    zz2 = Br21*(w2) + Br22*(l3);
    mcdis_dashed_line(xx1, 0, zz1, xx2, 0, zz2, 11);
    xx1 = Br11*(-w2) + Br12*(l2);
    zz1 = Br21*(-w2) + Br22*(l2);
    xx2 = Br11*(-w2) + Br12*(l3);
    zz2 = Br21*(-w2) + Br22*(l3);
    mcdis_dashed_line(xx1, 0, zz1, xx2, 0, zz2, 11);
  }

  /* Show instrument axis... */
  mcdis_dashed_line(0,0,0,0,0,2+rAz,21);

  /* Draw up the "focusing rectangle" */ 
  /* Horizontal direction vector @ focusing area */
  vec_prod(xx1,yy1,zz1,Btx,Bty,Btz,0.0,1.0,0.0);
  NORM(xx1,yy1,zz1);
  vec_prod(xx2,yy2,zz2,Btx,Bty,Btz,xx1,yy1,zz1);
  NORM(xx2,yy2,zz2);
  xx1*=Bfoc_XW/2.0; yy1*=Bfoc_XW/2.0; zz1*=Bfoc_XW/2.0;
  xx2*=Bfoc_YH/2.0; yy2*=Bfoc_YH/2.0; zz2*=Bfoc_YH/2.0;
  printf("Normal vectors pointing in directions\n %g %g %g and \n %g %g %g \n",xx1,yy1,zz1,xx2,yy2,zz2);
  mcdis_dashed_line(Btx -xx1 -xx2, Bty -yy1 -yy2, Btz -zz1 -zz2,
	      Btx +xx1 -xx2, Bty +yy1 -yy2, Btz +zz1 -zz2,5);
  mcdis_dashed_line(Btx -xx1 +xx2, Bty -yy1 +yy2, Btz -zz1 +zz2,
	      Btx +xx1 +xx2, Bty +yy1 +yy2, Btz +zz1 +zz2,5);

  mcdis_dashed_line(Btx -xx1 -xx2, Bty -yy1 -yy2, Btz -zz1 -zz2,
	      Btx -xx1 +xx2, Bty -yy1 +yy2, Btz -zz1 +zz2,5);
  mcdis_dashed_line(Btx +xx1 -xx2, Bty +yy1 -yy2, Btz +zz1 -zz2,
	      Btx +xx1 +xx2, Bty +yy1 +yy2, Btz +zz1 +zz2,5);
}


  int nearest_angle(double angle) {
    int AngleList[] = {5, 15, 25, 35, 45, 55};
    double diff = 180;
    int jmin=0;
    int j;
    for (j=0; j<6; j++) {
      if (fabs(AngleList[j]-angle) < diff) {
	diff = fabs(AngleList[j]-angle);
	jmin = j;
      }
    }
    return AngleList[jmin];
  }
  double BeamlinesN[]={ 30.0,  36.0,  42.0,  48.0,  54.0,  60.0,  66.0,  72.0,  78.0,  84.0,  90.0};
  double BeamlinesE[]={-30.0, -36.0, -42.0, -48.0, -54.0, -60.0, -66.0, -72.0, -78.0, -84.0, -90.0};
  double BeamlinesW[]={ 150.0,  144.7,  138.0,  132.7,  126.0,  120.7,  114.0,  108.7,  102.0,  96.7,  90.0,  84.0};
  double BeamlinesS[]={-150.0, -144.7, -138.0, -132.7, -126.0, -120.7, -114.0, -108.7, -102.0, -96.7, -90.0, -84.0};
  double ColdWidthNE[]={7e-2, 7.45e-2, 8.3e-2, 8.6e-2, 8.7e-2, 8.8e-2, 8.8e-2, 8.7e-2, 8.6e-2, 8.3e-2};
  double ThermalWidthNE[]={5.4e-2, 6.2e-2, 7.2e-2, 8.2e-2, 8.5e-2, 9.1e-2, 9.6e-2, 10e-2, 10.3e-2, 10.5e-2};
  double ColdWidthSW[]={7e-2, 7.45e-2, 8.3e-2, 8.6e-2, 8.7e-2, 8.8e-2, 8.8e-2, 8.8e-2, 8.6e-2, 8.4e-2, 6.9e-2};
  double ThermalWidthSW[]={5.4e-2, 6.2e-2, 7.2e-2, 8.2e-2, 8.5e-2, 9.1e-2, 9.6e-2, 9.95e-2, 10.25e-2, 10.45e-2, 10.5e-2};
  double ColdScalarsN[]={9.8788e-01, 1.0009e+00, 9.9335e-01, 9.5997e-01, 9.0717e-01, 9.1646e-01, 9.1028e-01, 9.1773e-01, 9.2537e-01, 9.1727e-01, -1};
  double ColdScalarsE[]={9.9032e-01, 1.0020e+00, 9.9647e-01, 9.6885e-01, 9.0713e-01, 9.1787e-01, 9.1190e-01, 9.2113e-01, 9.2786e-01, 9.2146e-01, -1};
  double ColdScalarsW[]={9.9017e-01, 1.0069e+00, 9.9366e-01, 9.7144e-01, 9.0624e-01, 8.9379e-01, 9.1022e-01, 9.2847e-01, 9.2812e-01, 9.2703e-01, 8.3098e-01};
  double ColdScalarsS[]={8.6550e-01, 1.0071e+00, 9.9401e-01, 9.6243e-01, 9.0398e-01, 8.9299e-01, 9.0830e-01, 9.2450e-01, 9.2270e-01, 9.2373e-01, 8.2508e-01};
  double ThermalScalarsN[]={8.6782e-01, 7.8627e-01, 7.6528e-01, 7.9469e-01, 7.3645e-01, 7.3012e-01, 7.2755e-01, 7.1750e-01, 7.1973e-01, 7.0459e-01, -1};
  double ThermalScalarsE[]={8.6838e-01, 7.8295e-01, 7.6719e-01, 7.9431e-01, 7.3989e-01, 7.3107e-01, 7.2811e-01, 7.2201e-01, 7.2097e-01, 7.0307e-01, -1};
  double ThermalScalarsW[]={8.7232e-01, 8.0007e-01, 7.6853e-01, 8.0251e-01, 7.3728e-01, 7.3761e-01, 7.2808e-01, 7.2151e-01, 7.1797e-01, 6.9857e-01, 6.9610e-01};
  double ThermalScalarsS[]={8.6910e-01, 7.9964e-01, 7.6365e-01, 7.9922e-01, 7.3479e-01, 7.3836e-01, 7.2773e-01, 7.2202e-01, 7.1667e-01, 7.0149e-01, 7.0084e-01};
  double dxCold[]={-0.01, -0.01, -0.002, 0.004,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0};
  double dxThermal[]={0.002, 0.003, 0.002, 0.007, 0.007, 0.007, 0.007, 0.007, 0.007, 0.007, 0.007};

/* Shared user declarations for all components types 'Guide'. */
/*******************************************************************************
*
* McStas, neutron ray-tracing package
*         Copyright 1997-2002, All rights reserved
*         Risoe National Laboratory, Roskilde, Denmark
*         Institut Laue Langevin, Grenoble, France
*
* Library: share/read_table-lib.h
*
* %Identification
* Written by: EF
* Date: Aug 28, 2002
* Origin: ILL
* Release: McStas 1.6
* Version: $Revision$
*
* This file is to be imported by components that may read data from table files
* It handles some shared functions.
*
* This library may be used directly as an external library. It has no dependency
*
* Usage: within SHARE
* %include "read_table-lib"
*
*******************************************************************************/

#ifndef READ_TABLE_LIB_H
#define READ_TABLE_LIB_H "$Revision$"

#define READ_TABLE_STEPTOL  0.04 /* tolerancy for constant step approx */

#ifndef MC_PATHSEP_C
#ifdef WIN32
#define MC_PATHSEP_C '\\'
#define MC_PATHSEP_S "\\"
#else  /* !WIN32 */
#ifdef MAC
#define MC_PATHSEP_C ':'
#define MC_PATHSEP_S ":"
#else  /* !MAC */
#define MC_PATHSEP_C '/'
#define MC_PATHSEP_S "/"
#endif /* !MAC */
#endif /* !WIN32 */
#endif /* !MC_PATHSEP_C */

#ifndef MCSTAS
#ifdef WIN32
#define MCSTAS "C:\\mcstas\\lib"
#else  /* !WIN32 */
#ifdef MAC
#define MCSTAS ":mcstas:lib" /* ToDo: What to put here? */
#else  /* !MAC */
#define MCSTAS "/usr/local/lib/mcstas"
#endif /* !MAC */
#endif /* !WIN32 */
#endif /* !MCSTAS */

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _MSC_EXTENSIONS
#include <strings.h>
#else
#  include <string.h>
#  define strcasecmp _stricmp
#  define strncasecmp _strnicmp
#endif

  typedef struct struct_table
  {
    char    filename[1024];
    long    filesize;
    char   *header;  /* text header, e.g. comments */
    double *data;    /* vector { x[0], y[0], ... x[n-1], y[n-1]... } */
    double  min_x;   /* min value of first column */
    double  max_x;   /* max value of first column */
    double  step_x;  /* minimal step value of first column */
    long    rows;    /* number of rows in matrix block */
    long    columns; /* number of columns in matrix block */

    long    begin;   /* start fseek index of block */
    long    end;     /* stop  fseek index of block */
    long    block_number;  /* block index. 0 is catenation of all */
    long    array_length;  /* number of elements in the t_Table array */
    char    monotonic;     /* true when 1st column/vector data is monotonic */
    char    constantstep;  /* true when 1st column/vector data has constant step */
    char    method[32];    /* interpolation method: nearest, linear */
    char    quiet;   /*output level for messages to the console 0: print all messages, 1:only print some/including errors, 2: never print anything.*/
  } t_Table;

/*maximum number of rows to rebin a table = 1M*/
enum { mcread_table_rebin_maxsize = 1000000 };

typedef struct t_Read_table_file_item {
    int ref_count;
    t_Table *table_ref;
} t_Read_table_file_item;

typedef enum enum_Read_table_file_actions {STORE,FIND,GC}  t_Read_table_file_actions;

/* read_table-lib function prototypes */
/* ========================================================================= */

/* 'public' functions */
long     Table_Read              (t_Table *Table, char *File, long block_number);
long     Table_Read_Offset       (t_Table *Table, char *File, long block_number,
                                  long *offset, long max_lines);
long     Table_Read_Offset_Binary(t_Table *Table, char *File, char *Type,
                                  long *Offset, long Rows, long Columns);
long     Table_Rebin(t_Table *Table); /* rebin table with regular 1st column and interpolate all columns 2:end */
long     Table_Info (t_Table Table);
#pragma acc routine
double   Table_Index(t_Table Table,   long i, long j); /* get indexed value */
#pragma acc routine
double   Table_Value(t_Table Table, double X, long j); /* search X in 1st column and return interpolated value in j-column */
t_Table *Table_Read_Array(char *File, long *blocks);
void     Table_Free_Array(t_Table *Table);
long     Table_Info_Array(t_Table *Table);
int      Table_SetElement(t_Table *Table, long i, long j, double value);
long     Table_Init(t_Table *Table, long rows, long columns); /* create a Table */
#pragma acc routine
double   Table_Value2d(t_Table Table, double X, double Y);    /* same as Table_Index with non-integer indices and 2d interpolation */
MCDETECTOR Table_Write(t_Table Table, char*file, char*xl, char*yl, 
           double x1, double x2, double y1, double y2); /* write Table to disk */
void * Table_File_List_Handler(t_Read_table_file_actions action, void *item, void *item_modifier);
t_Table *Table_File_List_find(char *name, int block, int offset);
int Table_File_List_gc(t_Table *tab);
void *Table_File_List_store(t_Table *tab);

#define Table_ParseHeader(header, ...) \
  Table_ParseHeader_backend(header,__VA_ARGS__,NULL);

char **Table_ParseHeader_backend(char *header, ...);
FILE *Open_File(char *name, const char *Mode, char *path);


/* private functions */
void Table_Free(t_Table *Table);
long Table_Read_Handle(t_Table *Table, FILE *fid, long block_number, long max_lines, char *name);
static void Table_Stat(t_Table *Table);
#pragma acc routine
double Table_Interp1d(double x, double x1, double y1, double x2, double y2);
#pragma acc routine
double Table_Interp1d_nearest(double x, double x1, double y1, double x2, double y2);
#pragma acc routine
double Table_Interp2d(double x, double y, double x1, double y1, double x2, double y2,
double z11, double z12, double z21, double z22);


#endif

/* end of read_table-lib.h */
/*******************************************************************************
*
* McStas, neutron ray-tracing package
*         Copyright (C) 1997-2009, All rights reserved
*         Risoe National Laboratory, Roskilde, Denmark
*         Institut Laue Langevin, Grenoble, France
*
* Library: share/read_table-lib.c
*
* %Identification
* Written by: EF
* Date: Aug 28, 2002
* Origin: ILL
* Release: McStas CVS_090504
* Version: $Revision$
*
* This file is to be imported by components that may read data from table files
* It handles some shared functions. Embedded within instrument in runtime mode.
*
* Usage: within SHARE
* %include "read_table-lib"
*
*******************************************************************************/

#ifndef READ_TABLE_LIB_H
#include "read_table-lib.h"
#endif

#ifndef READ_TABLE_LIB_C
#define READ_TABLE_LIB_C "$Revision$"


/*******************************************************************************
 * void *Table_File_List_Handler(action, item, item_modifier)
 *   ACTION: handle file entries in the read_table-lib file list. If a file is read - it is supposed to be
 *   stored in a list such that we can avoid reading the same file many times.
 *   input  action: FIND, STORE, GC. check if file exists in the list, store an item in the list, or check if it can be garbage collected.
 *   input item: depends on the action.
 *    FIND)  item is a filename, and item_modifier is the block number
 *    STORE) item is the Table to store - item_modifier is ignored
 *    GC)    item is the Table to check. If it has a ref_count >1 then this is simply decremented.
 *   return  depends on the action
 *    FIND)  return a reference to a table+ref_count item if found - NULL otherwise. I.e. NULL means the file has not been read before and must be read again.
 *    STORE) return NULL always
 *    GC)    return NULL if no garbage collection is needed, return an adress to the t_Table which should be garbage collected. 0x1 is returned if
 *           the item is not found in the list
*******************************************************************************/
void * Table_File_List_Handler(t_Read_table_file_actions action, void *item, void *item_modifier){

    /* logic here is Read_Table should include a call to FIND. If found the return value should just be used as
     * if the table had been read from disk. If not found then read the table and STORE.
     * Table_Free should include a call to GC. If this returns non-NULL then we should proceed with freeing the memory
     * associated with the table item - otherwise only decrement the reference counter since there are more references
     * that may need it.*/

    static t_Read_table_file_item read_table_file_list[1024];  
    static int read_table_file_count=0;

    t_Read_table_file_item *tr;
    switch(action){
        case FIND:
            /*interpret data item as a filename, if it is found return a pointer to the table and increment refcount.
             * if not found return the item itself*/
            tr=read_table_file_list;
            while ( tr->table_ref!=NULL ){
                int i=*((int*) item_modifier);
                int j=*( ((int*) item_modifier)+1);
                if ( !strcmp(tr->table_ref->filename,(char *) item) &&
                        tr->table_ref->block_number==i && tr->table_ref->begin==j ){
                    tr->ref_count++;
                    return (void *) tr;
                }
                tr++;
            }
            return NULL;
        case STORE:
            /*find an available slot and store references to table there*/
            tr=&(read_table_file_list[read_table_file_count++]);
            tr->table_ref = ((t_Table *) item);
            tr->ref_count++;
            return NULL;
        case GC:
            /* Should this item be garbage collected (freed) - if so scratch the entry and return the address of the item - 
             * else decrement ref_count and return NULL.
             * A non-NULL return expects the item to actually be freed afterwards.*/
            tr=read_table_file_list;
            while ( tr->table_ref!=NULL ){
                if ( tr->table_ref->data ==((t_Table *)item)->data && 
                        tr->table_ref->block_number == ((t_Table *)item)->block_number){
                    /*matching item found*/
                    if (tr->ref_count>1){
                        /*the item is found and no garbage collection needed*/
                        tr->ref_count--;
                        return NULL;
                    }else{
                        /* The item is found and the reference counter is 1.
                         * This means we should garbage collect. Move remaining list items up one slot,
                         * and return the table for garbage collection by caller*/
                        while (tr->table_ref!=NULL){
                            *tr=*(tr+1);
                            tr++;
                        }
                        read_table_file_count--;
                        return (t_Table *) item;
                    }
                }
                tr++;
            }
            /* item not found, and so should be garbage collected. This could be the case if freeing a
             * Table that has been constructed from code - not read from file. Return 0x1 to flag it for
             * collection.*/
            return (void *) 0x1 ;
    }
    /* If we arrive here, nothing worked, return NULL */
    return NULL;
}

/* Access functions to the handler*/

/********************************************
 * t_Table *Table_File_List_find(char *name, int block, int offset)
 * input name: filename to search for in the file list
 * input block: data block in the file as each file may contain more than 1 data block.
 * return a ref. to a table if it is found (you may use this pointer and skip reading the file), NULL otherwise (i.e. go ahead and read the file)
*********************************************/
t_Table *Table_File_List_find(char *name, int block, int offset){
    int vars[2]={block,offset};
    t_Read_table_file_item *item = Table_File_List_Handler(FIND,name, vars);
    if (item == NULL){
        return NULL;
    }else{
        return item->table_ref;
    }
}
/********************************************
 * int Table_File_List_gc(t_Table *tab)
 * input tab: the table to check for references.
 * return 0: no garbage collection needed
 *        1: Table's data and header (at least) should be freed.
*********************************************/
int Table_File_List_gc(t_Table *tab){
    void *rval=Table_File_List_Handler(GC,tab,0);
    if (rval==NULL) return 0;
    else return 1;
}


/*****************************************************************************
 * void *Table_File_List_store(t_Table *tab)
 * input tab: pointer to table to store.
 * return None. 
*******************************************************************************/
void *Table_File_List_store(t_Table *tab){
    return Table_File_List_Handler(STORE,tab,0);
}


/*******************************************************************************
* FILE *Open_File(char *name, char *Mode, char *path)
*   ACTION: search for a file and open it. Optionally return the opened path.
*   input   name:  file name from which table should be extracted
*           mode: "r", "w", "a" or any valid fopen mode
*           path:  NULL or a pointer to at least 1024 allocated chars
*   return  initialized file handle or NULL in case of error
*******************************************************************************/

  FILE *Open_File(char *File, const char *Mode, char *Path)
  {
    char path[1024];
    FILE *hfile = NULL;
    
    if (!File || File[0]=='\0')                     return(NULL);
    if (!strcmp(File,"NULL") || !strcmp(File,"0"))  return(NULL);
    
    /* search in current or full path */
    strncpy(path, File, 1024);
    hfile = fopen(path, Mode);
    if(!hfile)
    {
      char dir[1024];

      if (!hfile && instrument_source[0] != '\0' && strlen(instrument_source)) /* search in instrument source location */
      {
        char *path_pos   = NULL;
        /* extract path: searches for last file separator */
        path_pos    = strrchr(instrument_source, MC_PATHSEP_C);  /* last PATHSEP */
        if (path_pos) {
          long path_length = path_pos +1 - instrument_source;  /* from start to path+sep */
          if (path_length) {
            strncpy(dir, instrument_source, path_length);
            dir[path_length] = '\0';
            snprintf(path, 1024, "%s%c%s", dir, MC_PATHSEP_C, File);
            hfile = fopen(path, Mode);
          }
        }
      }
      if (!hfile && instrument_exe[0] != '\0' && strlen(instrument_exe)) /* search in PWD instrument executable location */
      {
        char *path_pos   = NULL;
        /* extract path: searches for last file separator */
        path_pos    = strrchr(instrument_exe, MC_PATHSEP_C);  /* last PATHSEP */
        if (path_pos) {
          long path_length = path_pos +1 - instrument_exe;  /* from start to path+sep */
          if (path_length) {
            strncpy(dir, instrument_exe, path_length);
            dir[path_length] = '\0';
            snprintf(path, 1024, "%s%c%s", dir, MC_PATHSEP_C, File);
            hfile = fopen(path, Mode);
          }
        }
      }
      if (!hfile) /* search in HOME or . */
      {
        strcpy(dir, getenv("HOME") ? getenv("HOME") : ".");
        snprintf(path, 1024, "%s%c%s", dir, MC_PATHSEP_C, File);
        hfile = fopen(path, Mode);
      }
      if (!hfile) /* search in MCSTAS/data */
      {
        strcpy(dir, getenv(FLAVOR_UPPER) ? getenv(FLAVOR_UPPER) : MCSTAS);
        snprintf(path, 1024, "%s%c%s%c%s", dir, MC_PATHSEP_C, "data", MC_PATHSEP_C, File);
        hfile = fopen(path, Mode);
      }
      if (!hfile) /* search in MVCSTAS/contrib */
      {
        strcpy(dir, getenv(FLAVOR_UPPER) ? getenv(FLAVOR_UPPER) : MCSTAS);
        snprintf(path, 1024, "%s%c%s%c%s", dir, MC_PATHSEP_C, "contrib", MC_PATHSEP_C, File);
        hfile = fopen(path, Mode);
      }
      if(!hfile)
      {
        // fprintf(stderr, "Warning: Could not open input file '%s' (Open_File)\n", File);
        return (NULL);
      }
    }
    if (Path) strncpy(Path, path, 1024);
    return(hfile);
  } /* end Open_File */

/*******************************************************************************
* long Read_Table(t_Table *Table, char *name, int block_number)
*   ACTION: read a single Table from a text file
*   input   Table: pointer to a t_Table structure
*           name:  file name from which table should be extracted
*           block_number: if the file does contain more than one
*                 data block, then indicates which one to get (from index 1)
*                 a 0 value means append/catenate all
*   return  initialized single Table t_Table structure containing data, header, ...
*           number of read elements (-1: error, 0:header only)
* The routine stores any line starting with '#', '%' and ';' into the header
* File is opened, read and closed
* Other lines are interpreted as numerical data, and stored.
* Data block should be a rectangular matrix or vector.
* Data block may be rebinned with Table_Rebin (also sort in ascending order)
*******************************************************************************/
  long Table_Read(t_Table *Table, char *File, long block_number)
  { /* reads all or a single data block from 'file' and returns a Table structure  */
    return(Table_Read_Offset(Table, File, block_number, NULL, 0));
  } /* end Table_Read */

/*******************************************************************************
* long Table_Read_Offset(t_Table *Table, char *name, int block_number, long *offset
*                        long max_rows)
*   ACTION: read a single Table from a text file, starting at offset
*     Same as Table_Read(..) except:
*   input   offset:    pointer to an offset (*offset should be 0 at start)
*           max_rows: max number of data rows to read from file (0 means all)
*   return  initialized single Table t_Table structure containing data, header, ...
*           number of read elements (-1: error, 0:header only)
*           updated *offset position (where end of reading occured)
*******************************************************************************/
  long Table_Read_Offset(t_Table *Table, char *File,
                         long block_number, long *offset,
                         long max_rows)
  { /* reads all/a data block in 'file' and returns a Table structure  */
    FILE *hfile;
    long  nelements=0;
    long  begin=0;
    long  filesize=0;
    char  name[1024];
    char  path[1024];
    struct stat stfile;

    /*Need to be able to store the pointer*/
    if (!Table) return(-1);

    /*TK: Valgrind flags it as usage of uninitialised variable: */
    Table->quiet = 0;

    //if (offset && *offset) snprintf(name, 1024, "%s@%li", File, *offset);
    //else                   
    strncpy(name, File, 1024);
    if(offset && *offset){
        begin=*offset;
    }
    /* Check if the table has already been read from file.
     * If so just reuse the table, if not (this is flagged by returning NULL
     * set up a new table and read the data into it */
    t_Table *tab_p= Table_File_List_find(name,block_number,begin);
    if ( tab_p!=NULL ){
        /*table was found in the Table_File_List*/
        *Table=*tab_p;
        MPI_MASTER(
            if(Table->quiet<1)
              printf("Reusing input file '%s' (Table_Read_Offset)\n", name);
            );
        return Table->rows*Table->columns;
    }

    /* open the file */
    hfile = Open_File(File, "r", path);
    if (!hfile) return(-1);
    else {
      MPI_MASTER(
          if(Table->quiet<1)
            printf("Opening input file '%s' (Table_Read_Offset)\n", path);
          );
    }
    
    /* read file state */
    stat(path,&stfile); filesize = stfile.st_size;
    if (offset && *offset) fseek(hfile, *offset, SEEK_SET);
    begin     = ftell(hfile);
    
    Table_Init(Table, 0, 0);

    /* read file content and set the Table */
    nelements = Table_Read_Handle(Table, hfile, block_number, max_rows, name);
    Table->begin = begin;
    Table->end   = ftell(hfile);
    Table->filesize = (filesize>0 ? filesize : 0);
    Table_Stat(Table);
    
    Table_File_List_store(Table);

    if (offset) *offset=Table->end;
    fclose(hfile);
    return(nelements);

  } /* end Table_Read_Offset */

/*******************************************************************************
* long Table_Read_Offset_Binary(t_Table *Table, char *File, char *type,
*                               long *offset, long rows, long columns)
*   ACTION: read a single Table from a binary file, starting at offset
*     Same as Table_Read_Offset(..) except that it handles binary files.
*   input   type: may be "float"/NULL or "double"
*           offset: pointer to an offset (*offset should be 0 at start)
*           rows   : number of rows (0 means read all)
*           columns: number of columns
*   return  initialized single Table t_Table structure containing data, header, ...
*           number of read elements (-1: error, 0:header only)
*           updated *offset position (where end of reading occured)
*******************************************************************************/
  long Table_Read_Offset_Binary(t_Table *Table, char *File, char *type,
                                long *offset, long rows, long columns)
  { /* reads all/a data block in binary 'file' and returns a Table structure  */
    long    nelements, sizeofelement;
    long    filesize;
    FILE   *hfile;
    char    path[1024];
    struct stat stfile;
    double *data    = NULL;
    double *datatmp = NULL;
    long    i;
    long    begin;

    if (!Table) return(-1);

    Table_Init(Table, 0, 0);
    
    /* open the file */
    hfile = Open_File(File, "r", path);
    if (!hfile) return(-1);
    else {
      MPI_MASTER(
          if(Table->quiet<1)
            printf("Opening input file '%s' (Table_Read, Binary)\n", path);
      );
    }
    
    /* read file state */
    stat(File,&stfile);
    filesize = stfile.st_size;
    Table->filesize=filesize;
    
    /* read file content */
    if (type && !strcmp(type,"double")) sizeofelement = sizeof(double);
    else  sizeofelement = sizeof(float);
    if (offset && *offset) fseek(hfile, *offset, SEEK_SET);
    begin     = ftell(hfile);
    if (rows && filesize > sizeofelement*columns*rows)
      nelements = columns*rows;
    else nelements = (long)(filesize/sizeofelement);
    if (!nelements || filesize <= *offset) return(0);
    data    = (double*)malloc(nelements*sizeofelement);
    if (!data) {
      if(!(Table->quiet>1))
        fprintf(stderr,"Error: allocating %ld elements for %s file '%s'. Too big (Table_Read_Offset_Binary).\n", nelements, type, File);
      exit(-1);
    }
    nelements = fread(data, sizeofelement, nelements, hfile);

    if (!data || !nelements)
    {
      if(!(Table->quiet>1))
        fprintf(stderr,"Error: reading %ld elements from %s file '%s' (Table_Read_Offset_Binary)\n", nelements, type, File);
      exit(-1);
    }
    Table->begin   = begin;
    Table->end     = ftell(hfile);
    if (offset) *offset=Table->end;
    fclose(hfile);

    datatmp = (double*)realloc(data, (double)nelements*sizeofelement);
    if (!datatmp) {
      free(data);
      fprintf(stderr,"Error: reallocating %ld elements for %s file '%s'. Too big (Table_Read_Offset_Binary).\n", nelements, type, File);
      exit(-1);
    } else {
      data = datatmp;
    }
    /* copy file data into Table */
    if (type && !strcmp(type,"double")) Table->data = data;
    else {
      float  *s;
      double *dataf;
      s     = (float*)data;
      dataf = (double*)malloc(sizeof(double)*nelements);
      if (!dataf) {
	fprintf(stderr, "Could not allocate data block of size %ld\n", nelements);
	exit(-1);
      }
      for (i=0; i<nelements; i++)
        dataf[i]=s[i];
      free(data);
      Table->data = dataf;
    }
    strncpy(Table->filename, File, 1024);
    Table->rows    = nelements/columns;
    Table->columns = columns;
    Table->array_length = 1;
    Table->block_number = 1;

    Table_Stat(Table);

    return(nelements);
  } /* end Table_Read_Offset_Binary */

/*******************************************************************************
* long Table_Read_Handle(t_Table *Table, FILE *fid, int block_number, long max_rows, char *name)
*   ACTION: read a single Table from a text file handle (private)
*   input   Table:pointer to a t_Table structure
*           fid:  pointer to FILE handle
*           block_number: if the file does contain more than one
*                 data block, then indicates which one to get (from index 1)
*                 a 0 value means append/catenate all
*           max_rows: if non 0, only reads that number of lines
*   return  initialized single Table t_Table structure containing data, header, ...
*           modified Table t_Table structure containing data, header, ...
*           number of read elements (-1: error, 0:header only)
* The routine stores any line starting with '#', '%' and ';' into the header
* Other lines are interpreted as numerical data, and stored.
* Data block should be a rectangular matrix or vector.
* Data block may be rebined with Table_Rebin (also sort in ascending order)
*******************************************************************************/
  long Table_Read_Handle(t_Table *Table, FILE *hfile,
                         long block_number, long max_rows, char *name)
  { /* reads all/a data block from 'file' handle and returns a Table structure  */
    double *Data              = NULL;
    double *Datatmp           = NULL;
    char *Header              = NULL;
    char *Headertmp           = NULL;
    long  malloc_size         = CHAR_BUF_LENGTH;
    long  malloc_size_h       = 4096;
    long  Rows = 0,   Columns = 0;
    long  count_in_array      = 0;
    long  count_in_header     = 0;
    long  count_invalid       = 0;
    long  block_Current_index = 0;
    char  flag_End_row_loop   = 0;

    if (!Table) return(-1);
    Table_Init(Table, 0, 0);
    if (name && name[0]!='\0') strncpy(Table->filename, name, 1024);

    if(!hfile) {
       fprintf(stderr, "Error: File handle is NULL (Table_Read_Handle).\n");
       return (-1);
    }
    Header = (char*)  calloc(malloc_size_h, sizeof(char));
    Data   = (double*)calloc(malloc_size,   sizeof(double));
    if ((Header == NULL) || (Data == NULL)) {
       fprintf(stderr, "Error: Could not allocate Table and Header (Table_Read_Handle).\n");
       return (-1);
    }

    int flag_In_array = 0;
    do { /* while (!flag_End_row_loop) */
      char  *line=malloc(1024*CHAR_BUF_LENGTH*sizeof(char));
      long  back_pos=0;   /* ftell start of line */

      if (!line) {
	fprintf(stderr,"Could not allocate line buffer\n");
	exit(-1);
      }
      back_pos = ftell(hfile);
      if (fgets(line, 1024*CHAR_BUF_LENGTH, hfile) != NULL) { /* analyse line */
        /* first skip blank and tabulation characters */
        int i = strspn(line, " \t");

        /* handle comments: stored in header */
        if (NULL != strchr("#%;/", line[i]))
        { /* line is a comment */
          count_in_header += strlen(line);
          if (count_in_header >= malloc_size_h) {
            /* if succeed and in array : add (and realloc if necessary) */
            malloc_size_h = count_in_header+4096;
            char *Headertmp = (char*)realloc(Header, malloc_size_h*sizeof(char));
	    if(!Headertmp) {
	      free(Header);
	             fprintf(stderr, "Error: Could not reallocate Header (Table_Read_Handle).\n");
		     free(Header);
		     return (-1);
	    } else {
	      Header = Headertmp;
	    }
          }
          strncat(Header, line, 4096);
          flag_In_array=0;
          /* exit line and file if passed desired block */
          if (block_number > 0 && block_number == block_Current_index) {
            flag_End_row_loop = 1;
          }

          /* Continue with next line */
          continue;
        }
        if (strstr(line, "***"))
        {
          count_invalid++;
          /* Continue with next line */
          continue;
        }

        /* get the number of columns splitting line with strtok */
        char  *lexeme;
        char  flag_End_Line = 0;
        long  block_Num_Columns = 0;
        const char seps[] = " ,;\t\n\r";

        lexeme = strtok(line, seps);
        while (!flag_End_Line) {
          if ((lexeme != NULL) && (lexeme[0] != '\0')) {
            /* reading line: the token is not empty */
            double X;
            int    count=1;
            /* test if we have 'NaN','Inf' */
            if (!strncasecmp(lexeme,"NaN",3))
              X = 0;
            else if (!strncasecmp(lexeme,"Inf",3) || !strncasecmp(lexeme,"+Inf",4))
              X = FLT_MAX;
            else if (!strncasecmp(lexeme,"-Inf",4))
              X = -FLT_MAX;
            else
              count = sscanf(lexeme,"%lg",&X);
            if (count == 1) {
              /* reading line: the token is a number in the line */
              if (!flag_In_array) {
                /* reading num: not already in a block: starts a new data block */
                block_Current_index++;
                flag_In_array    = 1;
                block_Num_Columns= 0;
                if (block_number > 0) {
                  /* initialise a new data block */
                  Rows = 0;
                  count_in_array = 0;
                } /* else append */
              }
              /* reading num: all blocks or selected block */
              if (flag_In_array && (block_number == 0 ||
                  block_number == block_Current_index)) {
                /* starting block: already the desired number of rows ? */
                if (block_Num_Columns == 0 &&
                    max_rows > 0 && Rows >= max_rows) {
                  flag_End_Line      = 1;
                  flag_End_row_loop  = 1;
                  flag_In_array      = 0;
                  /* reposition to begining of line (ignore line) */
                  fseek(hfile, back_pos, SEEK_SET);
                } else { /* store into data array */
                  if (count_in_array >= malloc_size) {
                    /* realloc data buffer if necessary */
                    malloc_size = count_in_array*1.5;
                    Datatmp = (double*) realloc(Data, malloc_size*sizeof(double));
                    if (Datatmp == NULL) {
                      fprintf(stderr, "Error: Can not re-allocate memory %zi (Table_Read_Handle).\n",
                              malloc_size*sizeof(double));
		      free(Data);
                      return (-1);
                    } else {
                      Data=Datatmp;
                    }
                  }
                  if (0 == block_Num_Columns) Rows++;
                  Data[count_in_array] = X;
                  count_in_array++;
                  block_Num_Columns++;
                }
              } /* reading num: end if flag_In_array */
            } /* end reading num: end if sscanf lexeme -> numerical */
            else {
              /* reading line: the token is not numerical in that line. end block */
              if (block_Current_index == block_number) {
                flag_End_Line = 1;
                flag_End_row_loop = 1;
              } else {
                flag_In_array = 0;
                flag_End_Line = 1;
              }
            }
          }
          else {
            /* no more tokens in line */
            flag_End_Line = 1;
            if (block_Num_Columns > 0) Columns = block_Num_Columns;
          }

          // parse next token
          lexeme = strtok(NULL, seps);

        } /* while (!flag_End_Line) */
      } /* end: if fgets */
      else flag_End_row_loop = 1; /* else fgets : end of file */
      free(line);
    } while (!flag_End_row_loop); /* end while flag_End_row_loop */

    Table->block_number = block_number;
    Table->array_length = 1;

    // shrink header to actual size (plus terminating 0-byte)
    if (count_in_header) {
      Headertmp = (char*)realloc(Header, count_in_header*sizeof(char) + 1);
      if(!Headertmp) {
	fprintf(stderr, "Error: Could not shrink Header (Table_Read_Handle).\n");
	free(Header);
	return (-1);
      } else {
        Header = Headertmp;
      }
    }
    Table->header = Header;

    if (count_in_array*Rows*Columns == 0)
    {
      Table->rows         = 0;
      Table->columns      = 0;
      free(Data);
      return (0);
    }
    if (Rows * Columns != count_in_array)
    {
      fprintf(stderr, "Warning: Read_Table :%s %s Data has %li values that should be %li x %li\n",
        (Table->filename[0] != '\0' ? Table->filename : ""),
        (!block_number ? " catenated" : ""),
        count_in_array, Rows, Columns);
      Columns = count_in_array; Rows = 1;
    }
    if (count_invalid)
    {
      fprintf(stderr,"Warning: Read_Table :%s %s Data has %li invalid lines (*****). Ignored.\n",
      (Table->filename[0] != '\0' ? Table->filename : ""),
        (!block_number ? " catenated" : ""),
        count_invalid);
    }
    Datatmp     = (double*)realloc(Data, count_in_array*sizeof(double));
    if(!Datatmp) {
      fprintf(stderr, "Error: Could reallocate Data block to %li doubles (Table_Read_Handle).\n", count_in_array);
      free(Data);
      return (-1);
    } else {
      Data = Datatmp;
    }
    Table->data         = Data;
    Table->rows         = Rows;
    Table->columns      = Columns;

    return (count_in_array);

  } /* end Table_Read_Handle */

/*******************************************************************************
* long Table_Rebin(t_Table *Table)
*   ACTION: rebin a single Table, sorting 1st column in ascending order
*   input   Table: single table containing data.
*                  The data block is reallocated in this process
*   return  updated Table with increasing, evenly spaced first column (index 0)
*           number of data elements (-1: error, 0:empty data)
*******************************************************************************/
  long Table_Rebin(t_Table *Table)
  {
    double new_step=0;
    long   i;
    /* performs linear interpolation on X axis (0-th column) */

    if (!Table) return(-1);
    if (!Table->data 
    || Table->rows*Table->columns == 0 || !Table->step_x)
      return(0);
    Table_Stat(Table); /* recompute statitstics and minimal step */
    new_step = Table->step_x; /* minimal step in 1st column */

    if (!(Table->constantstep)) /* not already evenly spaced */
    {
      long Length_Table;
      double *New_Table;

      Length_Table = ceil(fabs(Table->max_x - Table->min_x)/new_step)+1;
      /*return early if the rebinned table will become too large*/
      if (Length_Table > mcread_table_rebin_maxsize){
        fprintf(stderr,"WARNING: (Table_Rebin): Rebinning table from %s would exceed 1M rows. Skipping.\n", Table->filename); 
        return(Table->rows*Table->columns);
      }
      New_Table    = (double*)malloc(Length_Table*Table->columns*sizeof(double));
      if (!New_Table) {
	fprintf(stderr,"Could not allocate New_Table of size %ld x %ld\n", Length_Table, Table->columns);
	exit(-1);
      }
      for (i=0; i < Length_Table; i++)
      {
        long   j;
        double X;
        X = Table->min_x + i*new_step;
        New_Table[i*Table->columns] = X;
        for (j=1; j < Table->columns; j++)
          New_Table[i*Table->columns+j]
                = Table_Value(*Table, X, j);
      } /* end for i */

      Table->rows = Length_Table;
      Table->step_x = new_step;
      Table->max_x = Table->min_x + (Length_Table-1)*new_step; 
      /*max might not be the same anymore
       * Use Length_Table -1 since the first and laset rows are the limits of the defined interval.*/
      free(Table->data);
      Table->data = New_Table;
      Table->constantstep=1;
    } /* end else (!constantstep) */
    return (Table->rows*Table->columns);
  } /* end Table_Rebin */

/*******************************************************************************
* double Table_Index(t_Table Table, long i, long j)
*   ACTION: read an element [i,j] of a single Table
*   input   Table: table containing data
*           i : index of row      (0:Rows-1)
*           j : index of column   (0:Columns-1)
*   return  Value = data[i][j]
* Returns Value from the i-th row, j-th column of Table
* Tests are performed on indexes i,j to avoid errors
*******************************************************************************/

#ifndef MIN
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#endif

double Table_Index(t_Table Table, long i, long j)
{
  long AbsIndex;

  if (Table.rows == 1 || Table.columns == 1) {
    /* vector */
    j = MIN(MAX(0, i+j), Table.columns*Table.rows - 1);
    i = 0;
  } else {
    /* matrix */
    i = MIN(MAX(0, i), Table.rows - 1);
    j = MIN(MAX(0, j), Table.columns - 1);
  }

  /* handle vectors specifically */
  AbsIndex = i*(Table.columns)+j;

  if (Table.data != NULL)
    return (Table.data[AbsIndex]);
  else
    return 0;
} /* end Table_Index */

/*******************************************************************************
* void Table_SetElement(t_Table *Table, long i, long j, double value)
*   ACTION: set an element [i,j] of a single Table
*   input   Table: table containing data
*           i : index of row      (0:Rows-1)
*           j : index of column   (0:Columns-1)
*           value = data[i][j]
* Returns 0 in case of error
* Tests are performed on indexes i,j to avoid errors
*******************************************************************************/
int Table_SetElement(t_Table *Table, long i, long j,
                     double value)
{
  long AbsIndex;

  if (Table->rows == 1 || Table->columns == 1) {
    /* vector */
    j = MIN(MAX(0, i+j), Table->columns*Table->rows - 1); i=0;
  } else {
    /* matrix */
    i = MIN(MAX(0, i), Table->rows - 1);
    j = MIN(MAX(0, j), Table->columns - 1);
  }

  AbsIndex = i*(Table->columns)+j;
  if (Table->data != NULL) {
    Table->data[AbsIndex] = value;
    return 1;
  }

  return 0;
} /* end Table_SetElement */

/*******************************************************************************
* double Table_Value(t_Table Table, double X, long j)
*   ACTION: read column [j] of a single Table at row which 1st column is X
*   input   Table: table containing data.
*           X : data value in the first column (index 0)
*           j : index of column from which is extracted the Value (0:Columns-1)
*   return  Value = data[index for X][j] with linear interpolation
* Returns Value from the j-th column of Table corresponding to the
* X value for the 1st column (index 0)
* Tests are performed (within Table_Index) on indexes i,j to avoid errors
* NOTE: data should rather be monotonic, and evenly sampled.
*******************************************************************************/
double Table_Value(t_Table Table, double X, long j)
{
  long   Index = -1;
  double X1=0, Y1=0, X2=0, Y2=0;
  double ret=0;

  if (X > Table.max_x) return Table_Index(Table,Table.rows-1  ,j);
  if (X < Table.min_x) return Table_Index(Table,0  ,j);

  // Use constant-time lookup when possible
  if(Table.constantstep) {
    Index = (long)floor(
              (X - Table.min_x) / (Table.max_x - Table.min_x) * (Table.rows-1));
    X1 = Table_Index(Table,Index-1,0);
    X2 = Table_Index(Table,Index  ,0);
  }
  // Use binary search on large, monotonic tables
  else if(Table.monotonic && Table.rows > 100) {
    long left = Table.min_x;
    long right = Table.max_x;

    while (!((X1 <= X) && (X < X2)) && (right - left > 1)) {
      Index = (left + right) / 2;

      X1 = Table_Index(Table, Index-1, 0);
      X2 = Table_Index(Table, Index,   0);

      if (X < X1) {
        right = Index;
      } else {
        left  = Index;
      }
    }
  }

  // Fall back to linear search, if no-one else has set X1, X2 correctly
  if (!((X1 <= X) && (X < X2))) {
    /* look for index surrounding X in the table -> Index */
    for (Index=1; Index <= Table.rows-1; Index++) {
        X1 = Table_Index(Table, Index-1,0);
        X2 = Table_Index(Table, Index  ,0);
        if ((X1 <= X) && (X < X2)) break;
      } /* end for Index */
  }

  Y1 = Table_Index(Table,Index-1, j);
  Y2 = Table_Index(Table,Index  , j);

#ifdef OPENACC
#define strcmp(a,b) str_comp(a,b)
#endif

  if (!strcmp(Table.method,"linear")) {
    ret = Table_Interp1d(X, X1,Y1, X2,Y2);
  }
  else if (!strcmp(Table.method,"nearest")) {
    ret = Table_Interp1d_nearest(X, X1,Y1, X2,Y2);
  }

#ifdef OPENACC
#ifdef strcmp
#undef strcmp
#endif
#endif

  return ret;
} /* end Table_Value */

/*******************************************************************************
* double Table_Value2d(t_Table Table, double X, double Y)
*   ACTION: read element [X,Y] of a matrix Table
*   input   Table: table containing data.
*           X : row index, may be non integer
*           Y : column index, may be non integer
*   return  Value = data[index X][index Y] with bi-linear interpolation
* Returns Value for the indices [X,Y]
* Tests are performed (within Table_Index) on indexes i,j to avoid errors
* NOTE: data should rather be monotonic, and evenly sampled.
*******************************************************************************/
double Table_Value2d(t_Table Table, double X, double Y)
  {
    long   x1,x2,y1,y2;
    double z11,z12,z21,z22;
    double ret=0;

    x1 = (long)floor(X);
    y1 = (long)floor(Y);

    if (x1 > Table.rows-1 || x1 < 0) {
      x2 = x1;
    } else {
      x2 = x1 + 1;
    }

    if (y1 > Table.columns-1 || y1 < 0) {
      y2 = y1;
    } else {
      y2 = y1 + 1;
    }

    z11 = Table_Index(Table, x1, y1);

    if (y2 != y1) z12=Table_Index(Table, x1, y2); else z12 = z11;
    if (x2 != x1) z21=Table_Index(Table, x2, y1); else z21 = z11;
    if (y2 != y1) z22=Table_Index(Table, x2, y2); else z22 = z21;

#ifdef OPENACC
#define strcmp(a,b) str_comp(a,b)
#endif

    if (!strcmp(Table.method,"linear"))
      ret = Table_Interp2d(X,Y, x1,y1,x2,y2, z11,z12,z21,z22);
#ifdef OPENACC
#ifdef strcmp
#undef strcmp
#endif
#endif
    else {
      if (fabs(X-x1) < fabs(X-x2)) {
        if (fabs(Y-y1) < fabs(Y-y2)) ret = z11; else ret = z12;
      } else {
        if (fabs(Y-y1) < fabs(Y-y2)) ret = z21; else ret = z22;
      }
    }
    return ret;
  } /* end Table_Value2d */


/*******************************************************************************
* void Table_Free(t_Table *Table)
*   ACTION: free a single Table. First Call Table_File_list_gc. If this returns
*   non-zero it means there are more refernces to the table, and so the table
*   should not bee freed.
*   return: empty Table
*******************************************************************************/
  void Table_Free(t_Table *Table)
  {
    if( !Table_File_List_gc(Table) ){
       return;
    } 
    if (!Table) return;
    if (Table->data   != NULL) free(Table->data);
    if (Table->header != NULL) free(Table->header);
    Table->data   = NULL;
    Table->header = NULL;
  } /* end Table_Free */

/******************************************************************************
* void Table_Info(t_Table Table)
*    ACTION: print informations about a single Table
*******************************************************************************/
  long Table_Info(t_Table Table)
  {
    char buffer[256];
    long ret=0;

    if (!Table.block_number) strcpy(buffer, "catenated");
    else sprintf(buffer, "block %li", Table.block_number);
    printf("Table from file '%s' (%s)",
        Table.filename[0] != '\0' ? Table.filename : "", buffer);
    if ((Table.data != NULL) && (Table.rows*Table.columns))
    {
      printf(" is %li x %li ", Table.rows, Table.columns);
      if (Table.rows*Table.columns > 1)
        printf("(x=%g:%g)", Table.min_x, Table.max_x);
      else printf("(x=%g) ", Table.min_x);
      ret = Table.rows*Table.columns;
      if (Table.monotonic)    printf(", monotonic");
      if (Table.constantstep) printf(", constant step");
      printf(". interpolation: %s\n", Table.method);
    }
    else printf(" is empty.\n");

    if (Table.header && strlen(Table.header)) {
      char *header;
      int  i;
      header = malloc(80);
      if (!header) return(ret);
      for (i=0; i<80; header[i++]=0);
      strncpy(header, Table.header, 75);
      if (strlen(Table.header) > 75) {
        strcat( header, " ...");
      }
      for (i=0; i<strlen(header); i++)
        if (header[i] == '\n' || header[i] == '\r') header[i] = ';';
      printf("  '%s'\n", header);
      free(header);
    }

    return(ret);
  } /* end Table_Info */

/******************************************************************************
* long Table_Init(t_Table *Table, m, n)
*   ACTION: initialise a Table to empty m by n table
*   return: empty Table
******************************************************************************/
long Table_Init(t_Table *Table, long rows, long columns)
{
  double *data=NULL;
  long   i;

  if (!Table) return(0);

  Table->header  = NULL;
  Table->filename[0]= '\0';
  Table->filesize= 0;
  Table->min_x   = 0;
  Table->max_x   = 0;
  Table->step_x  = 0;
  Table->block_number = 0;
  Table->array_length = 0;
  Table->monotonic    = 0;
  Table->constantstep = 0;
  Table->begin   = 0;
  Table->end     = 0;
  strcpy(Table->method,"linear");

  if (rows*columns >= 1) {
    data    = (double*)malloc(rows*columns*sizeof(double));
    if (data) for (i=0; i < rows*columns; data[i++]=0);
    else {
      if(Table->quiet<2)
        fprintf(stderr,"Error: allocating %ld double elements."
            "Too big (Table_Init).\n", rows*columns);
      rows = columns = 0;
    }
  }
  Table->rows    = (rows >= 1 ? rows : 0);
  Table->columns = (columns >= 1 ? columns : 0);
  Table->data    = data;
  return(Table->rows*Table->columns);
} /* end Table_Init */

/******************************************************************************
* long Table_Write(t_Table Table, char *file, x1,x2, y1,y2)
*   ACTION: write a Table to disk (ascii).
*     when x1=x2=0 or y1=y2=0, the table default limits are used.
*   return: 0=all is fine, non-0: error
*******************************************************************************/
MCDETECTOR Table_Write(t_Table Table, char *file, char *xl, char *yl, 
  double x1, double x2, double y1, double y2)
{
  MCDETECTOR detector;

  if ((Table.data == NULL) && (Table.rows*Table.columns)) {
    detector.m = 0;
    detector.xmin = 0;
    detector.xmax = 0;
    detector.ymin = 0;
    detector.ymax = 0;
    detector.zmin = 0;
    detector.zmax = 0; 
    detector.intensity = 0;
    detector.error = 0;
    detector.events = 0;
    detector.min = 0;
    detector.max = 0;
    detector.mean = 0;
    detector.centerX = 0;
    detector.halfwidthX = 0;
    detector.centerY = 0;
    detector.halfwidthY = 0;
    detector.rank = 0;
    detector.istransposed = 0;
    detector.n = 0;
    detector.p = 0;
    detector.date_l = 0;
    detector.p0 = NULL;
    detector.p1 = NULL;
    detector.p2 = NULL;
    return(detector); /* Table is empty - nothing to do */
  }
  if (!x1 && !x2) {
    x1 = Table.min_x;
    x2 = Table.max_x;
  }
  if (!y1 && !y2) {
    y1 = 1;
    y2 = Table.columns;
  }

  /* transfer content of the Table into a 2D detector */
  Coords coords = { 0, 0, 0};
  Rotation rot;
  rot_set_rotation(rot, 0, 0, 0);
  
  if (Table.rows == 1 || Table.columns == 1) {
    detector = mcdetector_out_1D(Table.filename,
                      xl ? xl : "", yl ? yl : "",
                      "x", x1, x2,
                      Table.rows * Table.columns,
                      NULL, Table.data, NULL,
		      file, file, coords, rot,9999);
  } else {
    detector = mcdetector_out_2D(Table.filename,
                      xl ? xl : "", yl ? yl : "",
                      x1, x2, y1, y2,
                      Table.rows, Table.columns,
                      NULL, Table.data, NULL,
		      file, file, coords, rot,9999);
  }
  return(detector);
}

/******************************************************************************
* void Table_Stat(t_Table *Table)
*   ACTION: computes min/max/mean step of 1st column for a single table (private)
*   return: updated Table
*******************************************************************************/
  static void Table_Stat(t_Table *Table)
  {
    long   i;
    double max_x, min_x;
    double row=1;
    char   monotonic=1;
    char   constantstep=1;
    double step=0;
    long n;

    if (!Table) return;
    if (!Table->rows || !Table->columns) return;
    if (Table->rows == 1) row=0; // single row
    max_x = -FLT_MAX;
    min_x =  FLT_MAX;
    n     = (row ? Table->rows : Table->columns);
    /* get min and max of first column/vector */
    for (i=0; i < n; i++)
    {
      double X;
      X = (row ? Table_Index(*Table,i  ,0)
                               : Table_Index(*Table,0, i));
      if (X < min_x) min_x = X;
      if (X > max_x) max_x = X;
    } /* for */
    
    /* test for monotonicity and constant step if the table is an XY or single vector */
    if (n > 1) {
      /* mean step */
      step = (max_x - min_x)/(n-1);
      /* now test if table is monotonic on first column, and get minimal step size */
      for (i=0; i < n-1; i++) {
        double X, diff;;
        X    = (row ? Table_Index(*Table,i  ,0)
                    : Table_Index(*Table,0,  i));
        diff = (row ? Table_Index(*Table,i+1,0)
                    : Table_Index(*Table,0,  i+1)) - X;
        if (diff && fabs(diff) < fabs(step)) step = diff;
        /* change sign ? */
        if ((max_x - min_x)*diff < 0 && monotonic)
          monotonic = 0;
      } /* end for */
      
      /* now test if steps are constant within READ_TABLE_STEPTOL */
      if(!step){
        /*means there's a disconitnuity -> not constantstep*/
        constantstep=0;
      }else if (monotonic) {
        for (i=0; i < n-1; i++) {
          double X, diff;
          X    = (row ? Table_Index(*Table,i  ,0)
              : Table_Index(*Table,0,  i));
          diff = (row ? Table_Index(*Table,i+1,0)
              : Table_Index(*Table,0,  i+1)) - X;
          if ( fabs(step)*(1+READ_TABLE_STEPTOL) < fabs(diff) ||
                fabs(diff) < fabs(step)*(1-READ_TABLE_STEPTOL) )
          { constantstep = 0; break; }
        }
      }

    }
    Table->step_x= step;
    Table->max_x = max_x;
    Table->min_x = min_x;
    Table->monotonic = monotonic;
    Table->constantstep = constantstep;
  } /* end Table_Stat */

/******************************************************************************
* t_Table *Table_Read_Array(char *File, long *blocks)
*   ACTION: read as many data blocks as available, iteratively from file
*   return: initialized t_Table array, last element is an empty Table.
*           the number of extracted blocks in non NULL pointer *blocks
*******************************************************************************/
  t_Table *Table_Read_Array(char *File, long *blocks)
  {
    t_Table *Table_Array    = NULL;
    t_Table *Table_Arraytmp = NULL;
    long offset=0;
    long block_number=0;
    long allocated=256;
    long nelements=1;

    /* first allocate an initial empty t_Table array */
    Table_Array = (t_Table *)malloc(allocated*sizeof(t_Table));
    if (!Table_Array) {
      fprintf(stderr, "Error: Can not allocate memory %zi (Table_Read_Array).\n",
         allocated*sizeof(t_Table));
      *blocks = 0;
      return (NULL);
    }

    while (nelements > 0)
    {
      t_Table Table;

      /* if ok, set t_Table block number else exit loop */
      block_number++;
      Table.block_number = block_number;
      
      /* access file at offset and get following block. Block number is from the set offset
       * hence the hardcoded 1 - i.e. the next block counted from offset.*/
      nelements = Table_Read_Offset(&Table, File, 1, &offset,0);
      /*if the block is empty - don't store it*/
      if (nelements>0){
          /* if t_Table array is not long enough, expand and realocate */
          if (block_number >= allocated-1) {
              allocated += 256;
              Table_Arraytmp = (t_Table *)realloc(Table_Array,
                      allocated*sizeof(t_Table));
              if (!Table_Arraytmp) {
                  fprintf(stderr, "Error: Can not re-allocate memory %zi (Table_Read_Array).\n",
                          allocated*sizeof(t_Table));
                  free(Table_Array);
                  *blocks = 0;
                  return (NULL);
              } else {
                Table_Array = Table_Arraytmp;
              }
          }
          /* store it into t_Table array */
          //snprintf(Table.filename, 1024, "%s#%li", File, block_number-1);
          Table_Array[block_number-1] = Table;
      }
      /* continues until we find an empty block */
    }
    /* send back number of extracted blocks */
    if (blocks) *blocks = block_number-1;

    /* now store total number of elements in Table array */
    for (offset=0; offset < block_number;
      Table_Array[offset++].array_length = block_number-1);

    return(Table_Array);
  } /* end Table_Read_Array */
/*******************************************************************************
* void Table_Free_Array(t_Table *Table)
*   ACTION: free a Table array
*******************************************************************************/
  void Table_Free_Array(t_Table *Table)
  {
    long index;
    if (!Table) return;
    for (index=0;index < Table[0].array_length; index++){
            Table_Free(&Table[index]);
    }
    free(Table);
  } /* end Table_Free_Array */

/******************************************************************************
* long Table_Info_Array(t_Table *Table)
*    ACTION: print informations about a Table array
*    return: number of elements in the Table array
*******************************************************************************/
  long Table_Info_Array(t_Table *Table)
  {
    long index=0;

    if (!Table) return(-1);
    while (index < Table[index].array_length
       && (Table[index].data || Table[index].header)
       && (Table[index].rows*Table[index].columns) ) {
      Table_Info(Table[index]);
      index++;
    }
    printf("This Table array contains %li elements\n", index);
    return(index);
  } /* end Table_Info_Array */

/******************************************************************************
* char **Table_ParseHeader(char *header, symbol1, symbol2, ..., NULL)
*    ACTION: search for char* symbols in header and return their value or NULL
*            the search is not case sensitive.
*            Last argument MUST be NULL
*    return: array of char* with line following each symbol, or NULL if not found
*******************************************************************************/
#ifndef MyNL_ARGMAX
#define MyNL_ARGMAX 50
#endif

char **Table_ParseHeader_backend(char *header, ...){
  va_list ap;
  char exit_flag=0;
  int counter   =0;
  char **ret    =NULL;
  if (!header || header[0]=='\0') return(NULL);

  ret = (char**)calloc(MyNL_ARGMAX, sizeof(char*));
  if (!ret) {
    printf("Table_ParseHeader: Cannot allocate %i values array for Parser (Table_ParseHeader).\n",
      MyNL_ARGMAX);
    return(NULL);
  }
  for (counter=0; counter < MyNL_ARGMAX; ret[counter++] = NULL);
  counter=0;

  va_start(ap, header);
  while(!exit_flag && counter < MyNL_ARGMAX-1)
  {
    char *arg_char=NULL;
    char *pos     =NULL;
    /* get variable argument value as a char */
    arg_char = va_arg(ap, char *);
    if (!arg_char || arg_char[0]=='\0'){
      exit_flag = 1; break;
    }
    /* search for the symbol in the header */
    pos = (char*)strcasestr(header, arg_char);
    if (pos) {
      char *eol_pos;
      eol_pos = strchr(pos+strlen(arg_char), '\n');
      if (!eol_pos)
        eol_pos = strchr(pos+strlen(arg_char), '\r');
      if (!eol_pos)
        eol_pos = pos+strlen(pos)-1;
      ret[counter] = (char*)malloc(eol_pos - pos);
      if (!ret[counter]) {
        printf("Table_ParseHeader: Cannot allocate value[%i] array for Parser searching for %s (Table_ParseHeader).\n",
          counter, arg_char);
        exit_flag = 1; break;
      }
      strncpy(ret[counter], pos+strlen(arg_char), eol_pos - pos - strlen(arg_char));
      ret[counter][eol_pos - pos - strlen(arg_char)]='\0';
    }
    counter++;
  }
  va_end(ap);
  return(ret);
} /* Table_ParseHeader */

/******************************************************************************
* double Table_Interp1d(x, x1, y1, x2, y2)
*    ACTION: interpolates linearly at x between y1=f(x1) and y2=f(x2)
*    return: y=f(x) value
*******************************************************************************/
double Table_Interp1d(double x,
  double x1, double y1,
  double x2, double y2)
{
  double slope;
  if (x2 == x1) return (y1+y2)/2;
  if (y1 == y2) return  y1;
  slope = (y2 - y1)/(x2 - x1);
  return y1+slope*(x - x1);
} /* Table_Interp1d */

/******************************************************************************
* double Table_Interp1d_nearest(x, x1, y1, x2, y2)
*    ACTION: table lookup with nearest method at x between y1=f(x1) and y2=f(x2)
*    return: y=f(x) value
*******************************************************************************/
double Table_Interp1d_nearest(double x,
  double x1, double y1,
  double x2, double y2)
{
  if (fabs(x-x1) < fabs(x-x2)) return (y1);
  else return(y2);
} /* Table_Interp1d_nearest */

/******************************************************************************
* double Table_Interp2d(x,y, x1,y1, x2,y2, z11,z12,z21,z22)
*    ACTION: interpolates bi-linearly at (x,y) between z1=f(x1,y1) and z2=f(x2,y2)
*    return: z=f(x,y) value
*    x,y |   x1   x2
*    ----------------
*     y1 |   z11  z21
*     y2 |   z12  z22
*******************************************************************************/
double Table_Interp2d(double x, double y,
  double x1, double y1,
  double x2, double y2,
  double z11, double z12, double z21, double z22)
{
  double ratio_x, ratio_y;
  if (x2 == x1) return Table_Interp1d(y, y1,z11, y2,z12);
  if (y1 == y2) return Table_Interp1d(x, x1,z11, x2,z21);

  ratio_y = (y - y1)/(y2 - y1);
  ratio_x = (x - x1)/(x2 - x1);
  return (1-ratio_x)*(1-ratio_y)*z11 + ratio_x*(1-ratio_y)*z21
    + ratio_x*ratio_y*z22         + (1-ratio_x)*ratio_y*z12;
} /* Table_Interp2d */

/* end of read_table-lib.c */
#endif // READ_TABLE_LIB_C

/*****************************************************************************
*
* McStas, neutron ray-tracing package
*         Copyright 1997-2006, All rights reserved
*         Risoe National Laboratory, Roskilde, Denmark
*         Institut Laue Langevin, Grenoble, France
*
* Library: share/ref-lib.h
*
* %Identification
* Written by: Peter Christiansen
* Date: August, 2006
* Origin: RISOE
* Release: McStas 1.10
* Version: $Revision$
*
* Add StdDoubleReflecFunc, ExtendedReflecFunc
* Date: October, 2022
* Locale: ESS
* Release: McStas 2.7x, 3.1x
*
* Commonly used reflection functions are declared in this file which
* are used by some guide and mirror components.
*
* Depends on read_table-lib
*
* Usage: within SHARE
* %include "ref-lib"
*
****************************************************************************/


#ifndef REF_LIB_H
#define REF_LIB_H "$Revision$"

void StdReflecFunc(double, double*, double*);
void TableReflecFunc(double, t_Table*, double*);
void StdDoubleReflecFunc(double, double*, double*);
void ExtendedReflecFunc(double, double*, double*);

#endif

/* end of ref-lib.h */
/****************************************************************************
*
* McStas, neutron ray-tracing package
*         Copyright 1997-2006, All rights reserved
*         Risoe National Laboratory, Roskilde, Denmark
*         Institut Laue Langevin, Grenoble, France
*
* Library: share/ref-lib.c
*
* %Identification
* Written by: Peter Christiansen
* Date: August, 2006
* Origin: RISOE
* Release: McStas 1.10
* Version: $Revision$
*
* Add StdDoubleReflecFunc, ExtendedReflecFunc
* Date: October, 2022
* Locale: ESS
* Release: McStas 2.7.x, 3.x
*
* Commonly used reflection functions are declared in this file which
* are used by some guide and mirror components.
*
* Variable names have prefix 'mc_ref_' for 'McStas Reflection'
* to avoid conflicts
*
* Usage: within SHARE
* %include "ref-lib"
*
****************************************************************************/

#ifndef REF_LIB_H
#include "ref-lib.h"
#endif

#ifndef READ_TABLE_LIB_H
#include "read_table-lib.h"
#include "read_table-lib.c"
#endif

/****************************************************************************
* void StdReflecFunc(double q, double *par, double *r)
*
* The McStas standard analytic parametrization of the reflectivity.
* The parameters are:
* R0:      [1]    Low-angle reflectivity
* Qc:      [AA-1] Critical scattering vector
* alpha:   [AA]   Slope of reflectivity
* m:       [1]    m-value of material. Zero means completely absorbing.
* W:       [AA-1] Width of supermirror cut-off
*****************************************************************************/
#pragma acc routine seq
void StdReflecFunc(double mc_pol_q, double *mc_pol_par, double *mc_pol_r) {
    double R0    = mc_pol_par[0];
    double Qc    = mc_pol_par[1];
    double alpha = mc_pol_par[2];
    double m     = mc_pol_par[3];
    double W     = mc_pol_par[4];
    double beta  = 0;
    mc_pol_q     = fabs(mc_pol_q);
    double arg;
    double m_corr;

    /* Simpler parametrization from Henrik Jacobsen uses these values that depend on m only.
       double m_value=m*0.9853+0.1978;
       double W=-0.0002*m_value+0.0022;
       double alpha=0.2304*m_value+5.0944;
       double beta=-7.6251*m_value+68.1137;
       If W and alpha are set to 0, use Henrik's approach for estimating these parameters
       and apply the formulation:
       arg = R0*0.5*(1-tanh(arg))*(1-alpha*(q-Qc)+beta*(q-Qc)*(q-Qc));
    */
    if (W==0 && alpha==0) {
      m = m * 0.9853 + 0.1978;
      m_corr = m * 0.9853 - 0.7875;
      W = -0.0002 * m_corr + 0.0022;
      alpha = 0.2304 * m_corr + 5.0944;
      beta = -7.6251 * m_corr + 68.1137;
      if (m==3) {
	alpha = m_corr;
	beta = 0;
      }
      arg = (mc_pol_q - m*Qc)/W; // <--- here m, not m_corr!!
    }

    arg = W > 0 ? (mc_pol_q - m*Qc)/W : 11;

    if (arg > 10 || m <= 0 || Qc <=0 || R0 <= 0) {
      *mc_pol_r = 0;
      return;
    }

    if (m < 1) { Qc *= m; m=1; }

    if(mc_pol_q <= Qc) {
      *mc_pol_r = R0;
      return;
    }


    *mc_pol_r = R0*0.5*(1 - tanh(arg))*(1 - alpha*(mc_pol_q - Qc) + beta*(mc_pol_q - Qc)*(mc_pol_q - Qc));

    return;
  }

/****************************************************************************
* void TableReflecFunc(double q, t_Table *par, double *r) {
*
* Looks up the reflectivity in a table using the routines in read_table-lib.
*****************************************************************************/
#pragma acc routine seq
void TableReflecFunc(double mc_pol_q, t_Table *mc_pol_par, double *mc_pol_r) {

  *mc_pol_r = Table_Value(*mc_pol_par, mc_pol_q, 1);
  if(*mc_pol_r>1)
    *mc_pol_r = 1;
  return;
}


/****************************************************************************
* void StdDoubleReflecFunc(double q, double *par, double *r)
* 
* The McStas standard analytic parametrization of the reflectivity for 
* double-side coated supermirror.
* The parameters are:
* R0:      [1]    Low-angle reflectivity
* Qc:      [AA-1] Critical scattering vector
* alpha:   [AA]   Slope of reflectivity
* m:       [1]    m-value of material. Zero means completely absorbing.
* W:       [AA-1] Width of supermirror cut-off
*****************************************************************************/
void StdDoubleReflecFunc(double mc_pol_q, double *mc_pol_par, double *mc_pol_r) {
    double R0    = mc_pol_par[0];
    double Qc    = mc_pol_par[1];
    double alpha = mc_pol_par[2];
    double m     = mc_pol_par[3];
    double W     = mc_pol_par[4];
    double beta  = 0;
    mc_pol_q     = fabs(mc_pol_q);
    double arg;
        
    /* Simpler parametrization from Henrik Jacobsen uses these values that depend on m only.
       double m_value=m*0.9853+0.1978;
       double W=-0.0002*m_value+0.0022;
       double alpha=0.2304*m_value+5.0944;
       double beta=-7.6251*m_value+68.1137; 
       If W and alpha are set to 0, use Henrik's approach for estimating these parameters
       and apply the formulation:
       arg = R0*0.5*(1-tanh(arg))*(1-alpha*(q-Qc)+beta*(q-Qc)*(q-Qc));
    */  
    if (W==0 && alpha==0) {
      m=m*0.9853+0.1978;
      W=-0.0002*m+0.0022;
      alpha=0.2304*m+5.0944;
      beta=-7.6251*m+68.1137;
      if (m<=3) {
	alpha=m;
	beta=0;
      }
    }
    
    arg = W > 0 ? (mc_pol_q - m*Qc)/W : 11;

    if (arg > 10 || m <= 0 || Qc <=0 || R0 <= 0) {
      *mc_pol_r = 0;
      return;
    }
    
    if (m < 1) { Qc *= m; m=1; }
    
	/*
		Reflectivity R0 = single-side coated supermirror reflectivity
		double-side coated supermirror reflectivity = 1- (1-R0)^2
	*/
	
    if(mc_pol_q <= Qc) {      
      *mc_pol_r = 1- (1-R0)*(1-R0);
      return;
    }
    
    
    R0 = R0*0.5*(1 - tanh(arg))*(1 - alpha*(mc_pol_q - Qc) + beta*(mc_pol_q - Qc)*(mc_pol_q - Qc));
    *mc_pol_r = 1- (1-R0)*(1-R0);
   
    return;
  }

void ExtendedReflecFunc(double mc_pol_q, double *mc_pol_par, double *mc_pol_r) {
    double R0    = mc_pol_par[0];
    double Qc    = mc_pol_par[1];
    double alpha = mc_pol_par[2];
    double m     = mc_pol_par[3];
    double W     = mc_pol_par[4];
    double beta  = mc_pol_par[5];
    mc_pol_q     = fabs(mc_pol_q);
    double arg;

    /* Simpler parametrization from Henrik Jacobsen uses these values that depend on m only.
       double m_value=m*0.9853+0.1978;
       double W=-0.0002*m_value+0.0022;
       double alpha=0.2304*m_value+5.0944;
       double beta=-7.6251*m_value+68.1137;
       If W and alpha are set to 0, use Henrik's approach for estimating these parameters
       and apply the formulation:
       arg = R0*0.5*(1-tanh(arg))*(1-alpha*(q-Qc)+beta*(q-Qc)*(q-Qc));
    */
    if (W==0 && alpha==0) {
      m=m*0.9853+0.1978;
      W=-0.0002*m+0.0022;
      alpha=0.2304*m+5.0944;
      beta=-7.6251*m+68.1137;
      if (m<=3) {
	alpha=m;
	beta=0;
      }
    }

    arg = W > 0 ? (mc_pol_q - m*Qc)/W : 11;

    if (arg > 10 || m <= 0 || Qc <=0 || R0 <= 0) {
      *mc_pol_r = 0;
      return;
    }

    if (m < 1) { Qc *= m; m=1; }

    if(mc_pol_q <= Qc) {
      *mc_pol_r = R0;
      return;
    }


    *mc_pol_r = R0*0.5*(1 - tanh(arg))*(1 - alpha*(mc_pol_q - Qc) + beta*(mc_pol_q - Qc)*(mc_pol_q - Qc));

    return;
  }

/* end of ref-lib.c */


/* Shared user declarations for all components types 'Tunneling_sample'. */
  struct StructVarsV {
    double sigma_a; /* Absorption cross section per atom (barns) */
    double sigma_i; /* Incoherent scattering cross section per atom (barns) */
    double rho;     /* Density of atoms (AA-3) */
    double my_s;
    double my_a_v;
    char isrect;       /* true when sample is a box */
    double distance;   /* when non zero, gives rect target distance */
    double aw, ah;     /* rectangular angular dimensions */
    double xw, yh;     /* rectangular metrical dimensions */
    double tx, ty, tz; /* target coords */
  };



/* ************************************************************************** */
/*             End of SHARE user declarations for all components              */
/* ************************************************************************** */


/* ********************** component definition declarations. **************** */

/* component source=ESS_butterfly() [1] DECLARE */
/* Parameter definition for component type 'ESS_butterfly' */
struct _struct_ESS_butterfly_parameters {
  /* Component type 'ESS_butterfly' setting parameters */
  char sector[16384];
  int beamline;
  MCNUM yheight;
  MCNUM cold_frac;
  int target_index;
  MCNUM dist;
  MCNUM focus_xw;
  MCNUM focus_yh;
  MCNUM c_performance;
  MCNUM t_performance;
  MCNUM Lmin;
  MCNUM Lmax;
  MCNUM tmax_multiplier;
  int n_pulses;
  MCNUM acc_power;
  MCNUM tfocus_dist;
  MCNUM tfocus_time;
  MCNUM tfocus_width;
  /* Component type 'ESS_butterfly' private parameters */
  double*  ColdWidths;
  double*  ThermalWidths;
  double  ColdScalars[11];
  double  ThermalScalars[11];
  double*  Beamlines;
  double  wfrac_cold;
  double  wfrac_thermal;
  double  C1_x;
  double  C1_z;
  double  C2_x;
  double  C2_z;
  double  C3_x;
  double  C3_z;
  double  T1_x;
  double  T1_z;
  double  T2_x;
  double  T2_z;
  double  T3_x;
  double  T3_z;
  double  rC1_x;
  double  rC1_z;
  double  rC2_x;
  double  rC2_z;
  double  rC3_x;
  double  rC3_z;
  double  rT1_x;
  double  rT1_z;
  double  rT2_x;
  double  rT2_z;
  double  rT3_x;
  double  rT3_z;
  double  tx;
  double  ty;
  double  tz;
  double  r11;
  double  r12;
  double  r21;
  double  r22;
  double  delta_y;
  double  Mwidth_c;
  double  Mwidth_t;
  double  beamportangle;
  double  w_mult;
  double  w_stat;
  double  w_focus;
  double  w_tfocus;
  double  w_geom_c;
  double  w_geom_t;
  int  isleft;
  double  l_range;
  double  cos_thermal;
  double  cos_cold;
  double  orientation_angle;
  double  cx;
  double  cz;
  int  jmax;
  double  dxC;
  double  dxT;
}; /* _struct_ESS_butterfly_parameters */
typedef struct _struct_ESS_butterfly_parameters _class_ESS_butterfly_parameters;

/* Parameters for component type 'ESS_butterfly' */
struct _struct_ESS_butterfly {
  char     _name[256]; /* e.g. source */
  char     _type[256]; /* ESS_butterfly */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_ESS_butterfly_parameters _parameters;
};
typedef struct _struct_ESS_butterfly _class_ESS_butterfly;
_class_ESS_butterfly _source_var;
#pragma acc declare create ( _source_var )

/* component Origin=Progress_bar() [2] DECLARE */
/* Parameter definition for component type 'Progress_bar' */
struct _struct_Progress_bar_parameters {
  /* Component type 'Progress_bar' setting parameters */
  char profile[16384];
  MCNUM percent;
  MCNUM flag_save;
  MCNUM minutes;
  /* Component type 'Progress_bar' private parameters */
  double  IntermediateCnts;
  time_t  StartTime;
  time_t  EndTime;
  time_t  CurrentTime;
  char  infostring[64];
}; /* _struct_Progress_bar_parameters */
typedef struct _struct_Progress_bar_parameters _class_Progress_bar_parameters;

/* Parameters for component type 'Progress_bar' */
struct _struct_Progress_bar {
  char     _name[256]; /* e.g. Origin */
  char     _type[256]; /* Progress_bar */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_Progress_bar_parameters _parameters;
};
typedef struct _struct_Progress_bar _class_Progress_bar;
_class_Progress_bar _Origin_var;
#pragma acc declare create ( _Origin_var )

/* component TOFmoderator_zoom=TOF_monitor() [3] DECLARE */
/* Parameter definition for component type 'TOF_monitor' */
struct _struct_TOF_monitor_parameters {
  /* Component type 'TOF_monitor' setting parameters */
  int nt;
  char filename[16384];
  MCNUM xmin;
  MCNUM xmax;
  MCNUM ymin;
  MCNUM ymax;
  MCNUM xwidth;
  MCNUM yheight;
  MCNUM tmin;
  MCNUM tmax;
  MCNUM dt;
  int restore_neutron;
  int nowritefile;
  /* Component type 'TOF_monitor' private parameters */
  DArray1d  TOF_N;
  DArray1d  TOF_p;
  DArray1d  TOF_p2;
  double  t_min;
  double  t_max;
  double  delta_t;
}; /* _struct_TOF_monitor_parameters */
typedef struct _struct_TOF_monitor_parameters _class_TOF_monitor_parameters;

/* Parameters for component type 'TOF_monitor' */
struct _struct_TOF_monitor {
  char     _name[256]; /* e.g. TOFmoderator_zoom */
  char     _type[256]; /* TOF_monitor */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_TOF_monitor_parameters _parameters;
};
typedef struct _struct_TOF_monitor _class_TOF_monitor;
_class_TOF_monitor _TOFmoderator_zoom_var;
#pragma acc declare create ( _TOFmoderator_zoom_var )

_class_TOF_monitor _TOFmoderator_var;
#pragma acc declare create ( _TOFmoderator_var )

/* component Lmon_guistart=L_monitor() [5] DECLARE */
/* Parameter definition for component type 'L_monitor' */
struct _struct_L_monitor_parameters {
  /* Component type 'L_monitor' setting parameters */
  int nL;
  char filename[16384];
  int nowritefile;
  MCNUM xmin;
  MCNUM xmax;
  MCNUM ymin;
  MCNUM ymax;
  MCNUM xwidth;
  MCNUM yheight;
  MCNUM Lmin;
  MCNUM Lmax;
  int restore_neutron;
  /* Component type 'L_monitor' private parameters */
  DArray1d  L_N;
  DArray1d  L_p;
  DArray1d  L_p2;
}; /* _struct_L_monitor_parameters */
typedef struct _struct_L_monitor_parameters _class_L_monitor_parameters;

/* Parameters for component type 'L_monitor' */
struct _struct_L_monitor {
  char     _name[256]; /* e.g. Lmon_guistart */
  char     _type[256]; /* L_monitor */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_L_monitor_parameters _parameters;
};
typedef struct _struct_L_monitor _class_L_monitor;
_class_L_monitor _Lmon_guistart_var;
#pragma acc declare create ( _Lmon_guistart_var )

_class_L_monitor _Lmon_normalize_var;
#pragma acc declare create ( _Lmon_normalize_var )

/* component Guide1=Guide() [7] DECLARE */
/* Parameter definition for component type 'Guide' */
struct _struct_Guide_parameters {
  /* Component type 'Guide' setting parameters */
  char reflect[16384];
  MCNUM w1;
  MCNUM h1;
  MCNUM w2;
  MCNUM h2;
  MCNUM l;
  MCNUM R0;
  MCNUM Qc;
  MCNUM alpha;
  MCNUM m;
  MCNUM W;
  /* Component type 'Guide' private parameters */
  t_Table  pTable;
  int  table_present;
}; /* _struct_Guide_parameters */
typedef struct _struct_Guide_parameters _class_Guide_parameters;

/* Parameters for component type 'Guide' */
struct _struct_Guide {
  char     _name[256]; /* e.g. Guide1 */
  char     _type[256]; /* Guide */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_Guide_parameters _parameters;
};
typedef struct _struct_Guide _class_Guide;
_class_Guide _Guide1_var;
#pragma acc declare create ( _Guide1_var )

_class_L_monitor _Lmonslow1_var;
#pragma acc declare create ( _Lmonslow1_var )

/* component PSDslow1=PSD_monitor() [9] DECLARE */
/* Parameter definition for component type 'PSD_monitor' */
struct _struct_PSD_monitor_parameters {
  /* Component type 'PSD_monitor' setting parameters */
  int nx;
  int ny;
  char filename[16384];
  MCNUM xmin;
  MCNUM xmax;
  MCNUM ymin;
  MCNUM ymax;
  MCNUM xwidth;
  MCNUM yheight;
  int restore_neutron;
  int nowritefile;
  /* Component type 'PSD_monitor' private parameters */
  DArray2d  PSD_N;
  DArray2d  PSD_p;
  DArray2d  PSD_p2;
}; /* _struct_PSD_monitor_parameters */
typedef struct _struct_PSD_monitor_parameters _class_PSD_monitor_parameters;

/* Parameters for component type 'PSD_monitor' */
struct _struct_PSD_monitor {
  char     _name[256]; /* e.g. PSDslow1 */
  char     _type[256]; /* PSD_monitor */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_PSD_monitor_parameters _parameters;
};
typedef struct _struct_PSD_monitor _class_PSD_monitor;
_class_PSD_monitor _PSDslow1_var;
#pragma acc declare create ( _PSDslow1_var )

/* component FOchop1=DiskChopper() [10] DECLARE */
/* Parameter definition for component type 'DiskChopper' */
struct _struct_DiskChopper_parameters {
  /* Component type 'DiskChopper' setting parameters */
  MCNUM theta_0;
  MCNUM radius;
  MCNUM yheight;
  MCNUM nu;
  MCNUM nslit;
  MCNUM jitter;
  MCNUM delay;
  MCNUM isfirst;
  MCNUM n_pulse;
  MCNUM abs_out;
  MCNUM phase;
  MCNUM xwidth;
  MCNUM verbose;
  /* Component type 'DiskChopper' private parameters */
  double  Tg;
  double  To;
  double  delta_y;
  double  height;
  double  omega;
}; /* _struct_DiskChopper_parameters */
typedef struct _struct_DiskChopper_parameters _class_DiskChopper_parameters;

/* Parameters for component type 'DiskChopper' */
struct _struct_DiskChopper {
  char     _name[256]; /* e.g. FOchop1 */
  char     _type[256]; /* DiskChopper */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_DiskChopper_parameters _parameters;
};
typedef struct _struct_DiskChopper _class_DiskChopper;
_class_DiskChopper _FOchop1_var;
#pragma acc declare create ( _FOchop1_var )

/* component TOFLmon1=TOFLambda_monitor() [11] DECLARE */
/* Parameter definition for component type 'TOFLambda_monitor' */
struct _struct_TOFLambda_monitor_parameters {
  /* Component type 'TOFLambda_monitor' setting parameters */
  int nowritefile;
  int nL;
  int nt;
  MCNUM tmin;
  MCNUM tmax;
  char filename[16384];
  MCNUM xmin;
  MCNUM xmax;
  MCNUM ymin;
  MCNUM ymax;
  MCNUM xwidth;
  MCNUM yheight;
  MCNUM Lmin;
  MCNUM Lmax;
  int restore_neutron;
  /* Component type 'TOFLambda_monitor' private parameters */
  DArray2d  TOFL_N;
  DArray2d  TOFL_p;
  DArray2d  TOFL_p2;
  double  tt_0;
  double  tt_1;
}; /* _struct_TOFLambda_monitor_parameters */
typedef struct _struct_TOFLambda_monitor_parameters _class_TOFLambda_monitor_parameters;

/* Parameters for component type 'TOFLambda_monitor' */
struct _struct_TOFLambda_monitor {
  char     _name[256]; /* e.g. TOFLmon1 */
  char     _type[256]; /* TOFLambda_monitor */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_TOFLambda_monitor_parameters _parameters;
};
typedef struct _struct_TOFLambda_monitor _class_TOFLambda_monitor;
_class_TOFLambda_monitor _TOFLmon1_var;
#pragma acc declare create ( _TOFLmon1_var )

_class_L_monitor _Lmon_afterslow1_var;
#pragma acc declare create ( _Lmon_afterslow1_var )

_class_PSD_monitor _PSD_afterslow1_var;
#pragma acc declare create ( _PSD_afterslow1_var )

_class_Guide _Guidelong1_var;
#pragma acc declare create ( _Guidelong1_var )

_class_Guide _Guidelong1b_var;
#pragma acc declare create ( _Guidelong1b_var )

_class_L_monitor _Lmon_slow2_var;
#pragma acc declare create ( _Lmon_slow2_var )

_class_DiskChopper _FOchop2_var;
#pragma acc declare create ( _FOchop2_var )

_class_DiskChopper _Fastchop1_var;
#pragma acc declare create ( _Fastchop1_var )

_class_PSD_monitor _PSD_afterslow2_var;
#pragma acc declare create ( _PSD_afterslow2_var )

_class_L_monitor _Lmon_afterslow2_var;
#pragma acc declare create ( _Lmon_afterslow2_var )

_class_TOFLambda_monitor _TOFL_afterslow2_var;
#pragma acc declare create ( _TOFL_afterslow2_var )

_class_Guide _Guidelong2_var;
#pragma acc declare create ( _Guidelong2_var )

_class_L_monitor _Lmon_beforeballistic_var;
#pragma acc declare create ( _Lmon_beforeballistic_var )

_class_PSD_monitor _PSD_beforeballistic_var;
#pragma acc declare create ( _PSD_beforeballistic_var )

_class_Guide _Guidelong2a_var;
#pragma acc declare create ( _Guidelong2a_var )

_class_L_monitor _Lmonfast2_var;
#pragma acc declare create ( _Lmonfast2_var )

_class_L_monitor _Lmonfast2_zoom_var;
#pragma acc declare create ( _Lmonfast2_zoom_var )

_class_TOFLambda_monitor _TOFLfast2_var;
#pragma acc declare create ( _TOFLfast2_var )

_class_TOFLambda_monitor _TOFLfast2zoom_var;
#pragma acc declare create ( _TOFLfast2zoom_var )

_class_PSD_monitor _PSDfast2_var;
#pragma acc declare create ( _PSDfast2_var )

_class_DiskChopper _Fastchop2_var;
#pragma acc declare create ( _Fastchop2_var )

_class_DiskChopper _Fastchop2counter_var;
#pragma acc declare create ( _Fastchop2counter_var )

_class_DiskChopper _FOchop3_var;
#pragma acc declare create ( _FOchop3_var )

_class_TOF_monitor _TOFfast2_zoom_var;
#pragma acc declare create ( _TOFfast2_zoom_var )

_class_L_monitor _Lmon_afterfast2_var;
#pragma acc declare create ( _Lmon_afterfast2_var )

_class_TOFLambda_monitor _TOFL_afterfast2_var;
#pragma acc declare create ( _TOFL_afterfast2_var )

_class_TOFLambda_monitor _TOFL_afterfast2_zoom_var;
#pragma acc declare create ( _TOFL_afterfast2_zoom_var )

_class_PSD_monitor _PSD_afterfast2_var;
#pragma acc declare create ( _PSD_afterfast2_var )

_class_Guide _Guidesample_var;
#pragma acc declare create ( _Guidesample_var )

_class_L_monitor _Lmon_guideend_var;
#pragma acc declare create ( _Lmon_guideend_var )

_class_PSD_monitor _PSDsample_var;
#pragma acc declare create ( _PSDsample_var )

_class_TOF_monitor _TOFsample_zoom_var;
#pragma acc declare create ( _TOFsample_zoom_var )

/* component Esample=E_monitor() [43] DECLARE */
/* Parameter definition for component type 'E_monitor' */
struct _struct_E_monitor_parameters {
  /* Component type 'E_monitor' setting parameters */
  int nE;
  char filename[16384];
  MCNUM xmin;
  MCNUM xmax;
  MCNUM ymin;
  MCNUM ymax;
  int nowritefile;
  MCNUM xwidth;
  MCNUM yheight;
  MCNUM Emin;
  MCNUM Emax;
  int restore_neutron;
  /* Component type 'E_monitor' private parameters */
  DArray1d  E_N;
  DArray1d  E_p;
  DArray1d  E_p2;
  double  S_p;
  double  S_pE;
  double  S_pE2;
}; /* _struct_E_monitor_parameters */
typedef struct _struct_E_monitor_parameters _class_E_monitor_parameters;

/* Parameters for component type 'E_monitor' */
struct _struct_E_monitor {
  char     _name[256]; /* e.g. Esample */
  char     _type[256]; /* E_monitor */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_E_monitor_parameters _parameters;
};
typedef struct _struct_E_monitor _class_E_monitor;
_class_E_monitor _Esample_var;
#pragma acc declare create ( _Esample_var )

_class_L_monitor _Lmon_sample_zoom_var;
#pragma acc declare create ( _Lmon_sample_zoom_var )

/* component sample=Tunneling_sample() [45] DECLARE */
/* Parameter definition for component type 'Tunneling_sample' */
struct _struct_Tunneling_sample_parameters {
  /* Component type 'Tunneling_sample' setting parameters */
  MCNUM thickness;
  MCNUM radius;
  MCNUM focus_r;
  MCNUM p_interact;
  MCNUM f_QE;
  MCNUM f_tun;
  MCNUM gamma;
  MCNUM E_tun;
  MCNUM target_x;
  MCNUM target_y;
  MCNUM target_z;
  MCNUM focus_xw;
  MCNUM focus_yh;
  MCNUM focus_aw;
  MCNUM focus_ah;
  MCNUM xwidth;
  MCNUM yheight;
  MCNUM zdepth;
  MCNUM sigma_abs;
  MCNUM sigma_inc;
  MCNUM Vc;
  int target_index;
  /* Component type 'Tunneling_sample' private parameters */
  struct StructVarsV  VarsV;
  double  ftun;
  double  fQE;
}; /* _struct_Tunneling_sample_parameters */
typedef struct _struct_Tunneling_sample_parameters _class_Tunneling_sample_parameters;

/* Parameters for component type 'Tunneling_sample' */
struct _struct_Tunneling_sample {
  char     _name[256]; /* e.g. sample */
  char     _type[256]; /* Tunneling_sample */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_Tunneling_sample_parameters _parameters;
};
typedef struct _struct_Tunneling_sample _class_Tunneling_sample;
_class_Tunneling_sample _sample_var;
#pragma acc declare create ( _sample_var )

/* component detectorarm=Arm() [46] DECLARE */
/* Parameter definition for component type 'Arm' */
struct _struct_Arm_parameters {
  char Arm_has_no_parameters;
}; /* _struct_Arm_parameters */
typedef struct _struct_Arm_parameters _class_Arm_parameters;

/* Parameters for component type 'Arm' */
struct _struct_Arm {
  char     _name[256]; /* e.g. detectorarm */
  char     _type[256]; /* Arm */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_Arm_parameters _parameters;
};
typedef struct _struct_Arm _class_Arm;
_class_Arm _detectorarm_var;
#pragma acc declare create ( _detectorarm_var )

_class_TOF_monitor _TOFdetector_var;
#pragma acc declare create ( _TOFdetector_var )

_class_TOF_monitor _TOFdetector_zoom_var;
#pragma acc declare create ( _TOFdetector_zoom_var )

_class_E_monitor _Edetector_var;
#pragma acc declare create ( _Edetector_var )

/* component TOF2Edetector=TOF2E_monitor() [50] DECLARE */
/* Parameter definition for component type 'TOF2E_monitor' */
struct _struct_TOF2E_monitor_parameters {
  /* Component type 'TOF2E_monitor' setting parameters */
  int nE;
  char filename[16384];
  int nowritefile;
  MCNUM xmin;
  MCNUM xmax;
  MCNUM ymin;
  MCNUM ymax;
  MCNUM xwidth;
  MCNUM yheight;
  MCNUM Emin;
  MCNUM Emax;
  MCNUM T_zero;
  MCNUM L_flight;
  int restore_neutron;
  /* Component type 'TOF2E_monitor' private parameters */
  DArray1d  E_N;
  DArray1d  E_p;
  DArray1d  E_p2;
  double  S_p;
  double  S_pE;
  double  S_pE2;
}; /* _struct_TOF2E_monitor_parameters */
typedef struct _struct_TOF2E_monitor_parameters _class_TOF2E_monitor_parameters;

/* Parameters for component type 'TOF2E_monitor' */
struct _struct_TOF2E_monitor {
  char     _name[256]; /* e.g. TOF2Edetector */
  char     _type[256]; /* TOF2E_monitor */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_TOF2E_monitor_parameters _parameters;
};
typedef struct _struct_TOF2E_monitor _class_TOF2E_monitor;
_class_TOF2E_monitor _TOF2Edetector_var;
#pragma acc declare create ( _TOF2Edetector_var )

int mcNUMCOMP = 50;

/* User declarations from instrument definition. Can define functions. */
        double FREQ;
        double t_FO1, t_FO2, t_fast1, t_fast2, t_fast2a, t_fast3, t_sample, t_detector;
        double tmin_zoom, tmax_zoom, t_offset;
        double E_target;

#undef compcurname
#undef compcurtype
#undef compcurindex
/* end of instrument 'ESS_IN5_reprate' and components DECLARE */

/* *****************************************************************************
* instrument 'ESS_IN5_reprate' and components INITIALISE
***************************************************************************** */

double index_getdistance(int first_index, int second_index)
/* Calculate the distance two components from their indexes*/
{
  return coords_len(coords_sub(POS_A_COMP_INDEX(first_index), POS_A_COMP_INDEX(second_index)));
}

double getdistance(char* first_component, char* second_component)
/* Calculate the distance between two named components */
{
  int first_index = _getcomp_index(first_component);
  int second_index = _getcomp_index(second_component);
  return index_getdistance(first_index, second_index);
}

double checked_setpos_getdistance(int current_index, char* first_component, char* second_component)
/* Calculate the distance between two named components at *_setpos() time, with component index checking */
{
  int first_index = _getcomp_index(first_component);
  int second_index = _getcomp_index(second_component);
  if (first_index >= current_index || second_index >= current_index) {
    printf("setpos_getdistance can only be used with the names of components before the current one!\n");
    return 0;
  }
  return index_getdistance(first_index, second_index);
}
#define setpos_getdistance(first, second) checked_setpos_getdistance(current_setpos_index, first, second)

/* component source=ESS_butterfly() SETTING, POSITION/ROTATION */
int _source_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_source_setpos] component source=ESS_butterfly() SETTING [ESS_butterfly:0]");
  stracpy(_source_var._name, "source", 16384);
  stracpy(_source_var._type, "ESS_butterfly", 16384);
  _source_var._index=1;
  int current_setpos_index = 1;
  if("W" && strlen("W"))
    stracpy(_source_var._parameters.sector, "W" ? "W" : "", 16384);
  else 
  _source_var._parameters.sector[0]='\0';
  _source_var._parameters.beamline = 5;
  _source_var._parameters.yheight = 0.03;
  _source_var._parameters.cold_frac = _instrument_var._parameters.cold;
  _source_var._parameters.target_index = 6;
  _source_var._parameters.dist = _instrument_var._parameters.GUI_start;
  _source_var._parameters.focus_xw = _instrument_var._parameters.GUI_w;
  _source_var._parameters.focus_yh = _instrument_var._parameters.GUI_h;
  _source_var._parameters.c_performance = 1;
  _source_var._parameters.t_performance = 1;
  _source_var._parameters.Lmin = _instrument_var._parameters.Lmin;
  _source_var._parameters.Lmax = _instrument_var._parameters.Lmax;
  _source_var._parameters.tmax_multiplier = 3;
  _source_var._parameters.n_pulses = _instrument_var._parameters.Num_pulses;
  _source_var._parameters.acc_power = 5;
  _source_var._parameters.tfocus_dist = 0;
  _source_var._parameters.tfocus_time = 0;
  _source_var._parameters.tfocus_width = 0;


  /* component source=ESS_butterfly() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(_source_var._rotation_absolute,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_copy(_source_var._rotation_relative, _source_var._rotation_absolute);
    _source_var._rotation_is_identity =  rot_test_identity(_source_var._rotation_relative);
    _source_var._position_absolute = coords_set(
      0, 0, -0.1);
    tc1 = coords_neg(_source_var._position_absolute);
    _source_var._position_relative = rot_apply(_source_var._rotation_absolute, tc1);
  } /* source=ESS_butterfly() AT ROTATED */
  DEBUG_COMPONENT("source", _source_var._position_absolute, _source_var._rotation_absolute);
  instrument->_position_absolute[1] = _source_var._position_absolute;
  instrument->_position_relative[1] = _source_var._position_relative;
    _source_var._position_relative_is_zero =  coords_test_zero(_source_var._position_relative);
  instrument->counter_N[1]  = instrument->counter_P[1] = instrument->counter_P2[1] = 0;
  instrument->counter_AbsorbProp[1]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0000_source", _source_var._position_absolute, _source_var._rotation_absolute, "ESS_butterfly");
        mccomp_param_nexus(nxhandle,"0000_source", "sector", "N", "W", "char*");
        mccomp_param_nexus(nxhandle,"0000_source", "beamline", "1", "5","int");
        mccomp_param_nexus(nxhandle,"0000_source", "yheight", "0.03", "0.03","MCNUM");
        mccomp_param_nexus(nxhandle,"0000_source", "cold_frac", "0.5", "_instrument_var._parameters.cold","MCNUM");
        mccomp_param_nexus(nxhandle,"0000_source", "target_index", "0", "6","int");
        mccomp_param_nexus(nxhandle,"0000_source", "dist", "0", "_instrument_var._parameters.GUI_start","MCNUM");
        mccomp_param_nexus(nxhandle,"0000_source", "focus_xw", "0", "_instrument_var._parameters.GUI_w","MCNUM");
        mccomp_param_nexus(nxhandle,"0000_source", "focus_yh", "0", "_instrument_var._parameters.GUI_h","MCNUM");
        mccomp_param_nexus(nxhandle,"0000_source", "c_performance", "1", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0000_source", "t_performance", "1", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0000_source", "Lmin", "NONE", "_instrument_var._parameters.Lmin","MCNUM");
        mccomp_param_nexus(nxhandle,"0000_source", "Lmax", "NONE", "_instrument_var._parameters.Lmax","MCNUM");
        mccomp_param_nexus(nxhandle,"0000_source", "tmax_multiplier", "3", "3","MCNUM");
        mccomp_param_nexus(nxhandle,"0000_source", "n_pulses", "1", "_instrument_var._parameters.Num_pulses","int");
        mccomp_param_nexus(nxhandle,"0000_source", "acc_power", "5", "5","MCNUM");
        mccomp_param_nexus(nxhandle,"0000_source", "tfocus_dist", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0000_source", "tfocus_time", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0000_source", "tfocus_width", "0", "0","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _source_setpos */

/* component Origin=Progress_bar() SETTING, POSITION/ROTATION */
int _Origin_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Origin_setpos] component Origin=Progress_bar() SETTING [Progress_bar:0]");
  stracpy(_Origin_var._name, "Origin", 16384);
  stracpy(_Origin_var._type, "Progress_bar", 16384);
  _Origin_var._index=2;
  int current_setpos_index = 2;
  if("NULL" && strlen("NULL"))
    stracpy(_Origin_var._parameters.profile, "NULL" ? "NULL" : "", 16384);
  else 
  _Origin_var._parameters.profile[0]='\0';
  _Origin_var._parameters.percent = 10;
  _Origin_var._parameters.flag_save = 0;
  _Origin_var._parameters.minutes = 0;


  /* component Origin=Progress_bar() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(_Origin_var._rotation_absolute,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_transpose(_source_var._rotation_absolute, tr1);
    rot_mul(_Origin_var._rotation_absolute, tr1, _Origin_var._rotation_relative);
    _Origin_var._rotation_is_identity =  rot_test_identity(_Origin_var._rotation_relative);
    _Origin_var._position_absolute = coords_set(
      0, 0, 0);
    tc1 = coords_sub(_source_var._position_absolute, _Origin_var._position_absolute);
    _Origin_var._position_relative = rot_apply(_Origin_var._rotation_absolute, tc1);
  } /* Origin=Progress_bar() AT ROTATED */
  DEBUG_COMPONENT("Origin", _Origin_var._position_absolute, _Origin_var._rotation_absolute);
  instrument->_position_absolute[2] = _Origin_var._position_absolute;
  instrument->_position_relative[2] = _Origin_var._position_relative;
    _Origin_var._position_relative_is_zero =  coords_test_zero(_Origin_var._position_relative);
  instrument->counter_N[2]  = instrument->counter_P[2] = instrument->counter_P2[2] = 0;
  instrument->counter_AbsorbProp[2]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0001_Origin", _Origin_var._position_absolute, _Origin_var._rotation_absolute, "Progress_bar");
        mccomp_param_nexus(nxhandle,"0001_Origin", "profile", "NULL", "NULL", "char*");
        mccomp_param_nexus(nxhandle,"0001_Origin", "percent", "10", "10","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Origin", "flag_save", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Origin", "minutes", "0", "0","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Origin_setpos */

/* component TOFmoderator_zoom=TOF_monitor() SETTING, POSITION/ROTATION */
int _TOFmoderator_zoom_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_TOFmoderator_zoom_setpos] component TOFmoderator_zoom=TOF_monitor() SETTING [TOF_monitor:0]");
  stracpy(_TOFmoderator_zoom_var._name, "TOFmoderator_zoom", 16384);
  stracpy(_TOFmoderator_zoom_var._type, "TOF_monitor", 16384);
  _TOFmoderator_zoom_var._index=3;
  int current_setpos_index = 3;
  _TOFmoderator_zoom_var._parameters.nt = 1000;
  if("TOFmoderator_zoom.dat" && strlen("TOFmoderator_zoom.dat"))
    stracpy(_TOFmoderator_zoom_var._parameters.filename, "TOFmoderator_zoom.dat" ? "TOFmoderator_zoom.dat" : "", 16384);
  else 
  _TOFmoderator_zoom_var._parameters.filename[0]='\0';
  _TOFmoderator_zoom_var._parameters.xmin = -0.05;
  _TOFmoderator_zoom_var._parameters.xmax = 0.05;
  _TOFmoderator_zoom_var._parameters.ymin = -0.05;
  _TOFmoderator_zoom_var._parameters.ymax = 0.05;
  _TOFmoderator_zoom_var._parameters.xwidth = 0.12;
  _TOFmoderator_zoom_var._parameters.yheight = 0.12;
  _TOFmoderator_zoom_var._parameters.tmin = 0;
  _TOFmoderator_zoom_var._parameters.tmax = 5000;
  _TOFmoderator_zoom_var._parameters.dt = 1.0;
  _TOFmoderator_zoom_var._parameters.restore_neutron = 1;
  _TOFmoderator_zoom_var._parameters.nowritefile = 0;


  /* component TOFmoderator_zoom=TOF_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Origin_var._rotation_absolute, _TOFmoderator_zoom_var._rotation_absolute);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    rot_mul(_TOFmoderator_zoom_var._rotation_absolute, tr1, _TOFmoderator_zoom_var._rotation_relative);
    _TOFmoderator_zoom_var._rotation_is_identity =  rot_test_identity(_TOFmoderator_zoom_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1E-6);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _TOFmoderator_zoom_var._position_absolute = coords_add(_Origin_var._position_absolute, tc2);
    tc1 = coords_sub(_Origin_var._position_absolute, _TOFmoderator_zoom_var._position_absolute);
    _TOFmoderator_zoom_var._position_relative = rot_apply(_TOFmoderator_zoom_var._rotation_absolute, tc1);
  } /* TOFmoderator_zoom=TOF_monitor() AT ROTATED */
  DEBUG_COMPONENT("TOFmoderator_zoom", _TOFmoderator_zoom_var._position_absolute, _TOFmoderator_zoom_var._rotation_absolute);
  instrument->_position_absolute[3] = _TOFmoderator_zoom_var._position_absolute;
  instrument->_position_relative[3] = _TOFmoderator_zoom_var._position_relative;
    _TOFmoderator_zoom_var._position_relative_is_zero =  coords_test_zero(_TOFmoderator_zoom_var._position_relative);
  instrument->counter_N[3]  = instrument->counter_P[3] = instrument->counter_P2[3] = 0;
  instrument->counter_AbsorbProp[3]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0002_TOFmoderator_zoom", _TOFmoderator_zoom_var._position_absolute, _TOFmoderator_zoom_var._rotation_absolute, "TOF_monitor");
        mccomp_param_nexus(nxhandle,"0002_TOFmoderator_zoom", "nt", "20", "1000","int");
        mccomp_param_nexus(nxhandle,"0002_TOFmoderator_zoom", "filename", 0, "TOFmoderator_zoom.dat", "char*");
        mccomp_param_nexus(nxhandle,"0002_TOFmoderator_zoom", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0002_TOFmoderator_zoom", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0002_TOFmoderator_zoom", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0002_TOFmoderator_zoom", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0002_TOFmoderator_zoom", "xwidth", "0", "0.12","MCNUM");
        mccomp_param_nexus(nxhandle,"0002_TOFmoderator_zoom", "yheight", "0", "0.12","MCNUM");
        mccomp_param_nexus(nxhandle,"0002_TOFmoderator_zoom", "tmin", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0002_TOFmoderator_zoom", "tmax", "0", "5000","MCNUM");
        mccomp_param_nexus(nxhandle,"0002_TOFmoderator_zoom", "dt", "1.0", "1.0","MCNUM");
        mccomp_param_nexus(nxhandle,"0002_TOFmoderator_zoom", "restore_neutron", "0", "1","int");
        mccomp_param_nexus(nxhandle,"0002_TOFmoderator_zoom", "nowritefile", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _TOFmoderator_zoom_setpos */

/* component TOFmoderator=TOF_monitor() SETTING, POSITION/ROTATION */
int _TOFmoderator_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_TOFmoderator_setpos] component TOFmoderator=TOF_monitor() SETTING [TOF_monitor:0]");
  stracpy(_TOFmoderator_var._name, "TOFmoderator", 16384);
  stracpy(_TOFmoderator_var._type, "TOF_monitor", 16384);
  _TOFmoderator_var._index=4;
  int current_setpos_index = 4;
  _TOFmoderator_var._parameters.nt = 1000;
  if("TOFmoderator.dat" && strlen("TOFmoderator.dat"))
    stracpy(_TOFmoderator_var._parameters.filename, "TOFmoderator.dat" ? "TOFmoderator.dat" : "", 16384);
  else 
  _TOFmoderator_var._parameters.filename[0]='\0';
  _TOFmoderator_var._parameters.xmin = -0.05;
  _TOFmoderator_var._parameters.xmax = 0.05;
  _TOFmoderator_var._parameters.ymin = -0.05;
  _TOFmoderator_var._parameters.ymax = 0.05;
  _TOFmoderator_var._parameters.xwidth = 0.12;
  _TOFmoderator_var._parameters.yheight = 0.12;
  _TOFmoderator_var._parameters.tmin = 0;
  _TOFmoderator_var._parameters.tmax = 3.0e5;
  _TOFmoderator_var._parameters.dt = 1.0;
  _TOFmoderator_var._parameters.restore_neutron = 1;
  _TOFmoderator_var._parameters.nowritefile = 0;


  /* component TOFmoderator=TOF_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Origin_var._rotation_absolute, _TOFmoderator_var._rotation_absolute);
    rot_transpose(_TOFmoderator_zoom_var._rotation_absolute, tr1);
    rot_mul(_TOFmoderator_var._rotation_absolute, tr1, _TOFmoderator_var._rotation_relative);
    _TOFmoderator_var._rotation_is_identity =  rot_test_identity(_TOFmoderator_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1E-6);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _TOFmoderator_var._position_absolute = coords_add(_Origin_var._position_absolute, tc2);
    tc1 = coords_sub(_TOFmoderator_zoom_var._position_absolute, _TOFmoderator_var._position_absolute);
    _TOFmoderator_var._position_relative = rot_apply(_TOFmoderator_var._rotation_absolute, tc1);
  } /* TOFmoderator=TOF_monitor() AT ROTATED */
  DEBUG_COMPONENT("TOFmoderator", _TOFmoderator_var._position_absolute, _TOFmoderator_var._rotation_absolute);
  instrument->_position_absolute[4] = _TOFmoderator_var._position_absolute;
  instrument->_position_relative[4] = _TOFmoderator_var._position_relative;
    _TOFmoderator_var._position_relative_is_zero =  coords_test_zero(_TOFmoderator_var._position_relative);
  instrument->counter_N[4]  = instrument->counter_P[4] = instrument->counter_P2[4] = 0;
  instrument->counter_AbsorbProp[4]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0003_TOFmoderator", _TOFmoderator_var._position_absolute, _TOFmoderator_var._rotation_absolute, "TOF_monitor");
        mccomp_param_nexus(nxhandle,"0003_TOFmoderator", "nt", "20", "1000","int");
        mccomp_param_nexus(nxhandle,"0003_TOFmoderator", "filename", 0, "TOFmoderator.dat", "char*");
        mccomp_param_nexus(nxhandle,"0003_TOFmoderator", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0003_TOFmoderator", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0003_TOFmoderator", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0003_TOFmoderator", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0003_TOFmoderator", "xwidth", "0", "0.12","MCNUM");
        mccomp_param_nexus(nxhandle,"0003_TOFmoderator", "yheight", "0", "0.12","MCNUM");
        mccomp_param_nexus(nxhandle,"0003_TOFmoderator", "tmin", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0003_TOFmoderator", "tmax", "0", "3.0e5","MCNUM");
        mccomp_param_nexus(nxhandle,"0003_TOFmoderator", "dt", "1.0", "1.0","MCNUM");
        mccomp_param_nexus(nxhandle,"0003_TOFmoderator", "restore_neutron", "0", "1","int");
        mccomp_param_nexus(nxhandle,"0003_TOFmoderator", "nowritefile", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _TOFmoderator_setpos */

/* component Lmon_guistart=L_monitor() SETTING, POSITION/ROTATION */
int _Lmon_guistart_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Lmon_guistart_setpos] component Lmon_guistart=L_monitor() SETTING [L_monitor:0]");
  stracpy(_Lmon_guistart_var._name, "Lmon_guistart", 16384);
  stracpy(_Lmon_guistart_var._type, "L_monitor", 16384);
  _Lmon_guistart_var._index=5;
  int current_setpos_index = 5;
  _Lmon_guistart_var._parameters.nL = 1000;
  if("Lmon_guistart.dat" && strlen("Lmon_guistart.dat"))
    stracpy(_Lmon_guistart_var._parameters.filename, "Lmon_guistart.dat" ? "Lmon_guistart.dat" : "", 16384);
  else 
  _Lmon_guistart_var._parameters.filename[0]='\0';
  _Lmon_guistart_var._parameters.nowritefile = 0;
  _Lmon_guistart_var._parameters.xmin = -0.05;
  _Lmon_guistart_var._parameters.xmax = 0.05;
  _Lmon_guistart_var._parameters.ymin = -0.05;
  _Lmon_guistart_var._parameters.ymax = 0.05;
  _Lmon_guistart_var._parameters.xwidth = _instrument_var._parameters.GUI_w + 0.01;
  _Lmon_guistart_var._parameters.yheight = _instrument_var._parameters.GUI_h + 0.01;
  _Lmon_guistart_var._parameters.Lmin = 0;
  _Lmon_guistart_var._parameters.Lmax = _instrument_var._parameters.Lmax + 1;
  _Lmon_guistart_var._parameters.restore_neutron = 0;


  /* component Lmon_guistart=L_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Origin_var._rotation_absolute, _Lmon_guistart_var._rotation_absolute);
    rot_transpose(_TOFmoderator_var._rotation_absolute, tr1);
    rot_mul(_Lmon_guistart_var._rotation_absolute, tr1, _Lmon_guistart_var._rotation_relative);
    _Lmon_guistart_var._rotation_is_identity =  rot_test_identity(_Lmon_guistart_var._rotation_relative);
    tc1 = coords_set(
      0, 0, _instrument_var._parameters.GUI_start -2e-6);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Lmon_guistart_var._position_absolute = coords_add(_Origin_var._position_absolute, tc2);
    tc1 = coords_sub(_TOFmoderator_var._position_absolute, _Lmon_guistart_var._position_absolute);
    _Lmon_guistart_var._position_relative = rot_apply(_Lmon_guistart_var._rotation_absolute, tc1);
  } /* Lmon_guistart=L_monitor() AT ROTATED */
  DEBUG_COMPONENT("Lmon_guistart", _Lmon_guistart_var._position_absolute, _Lmon_guistart_var._rotation_absolute);
  instrument->_position_absolute[5] = _Lmon_guistart_var._position_absolute;
  instrument->_position_relative[5] = _Lmon_guistart_var._position_relative;
    _Lmon_guistart_var._position_relative_is_zero =  coords_test_zero(_Lmon_guistart_var._position_relative);
  instrument->counter_N[5]  = instrument->counter_P[5] = instrument->counter_P2[5] = 0;
  instrument->counter_AbsorbProp[5]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0004_Lmon_guistart", _Lmon_guistart_var._position_absolute, _Lmon_guistart_var._rotation_absolute, "L_monitor");
        mccomp_param_nexus(nxhandle,"0004_Lmon_guistart", "nL", "20", "1000","int");
        mccomp_param_nexus(nxhandle,"0004_Lmon_guistart", "filename", 0, "Lmon_guistart.dat", "char*");
        mccomp_param_nexus(nxhandle,"0004_Lmon_guistart", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0004_Lmon_guistart", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0004_Lmon_guistart", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0004_Lmon_guistart", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0004_Lmon_guistart", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0004_Lmon_guistart", "xwidth", "0", "_instrument_var._parameters.GUI_w + 0.01","MCNUM");
        mccomp_param_nexus(nxhandle,"0004_Lmon_guistart", "yheight", "0", "_instrument_var._parameters.GUI_h + 0.01","MCNUM");
        mccomp_param_nexus(nxhandle,"0004_Lmon_guistart", "Lmin", "NONE", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0004_Lmon_guistart", "Lmax", "NONE", "_instrument_var._parameters.Lmax + 1","MCNUM");
        mccomp_param_nexus(nxhandle,"0004_Lmon_guistart", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Lmon_guistart_setpos */

/* component Lmon_normalize=L_monitor() SETTING, POSITION/ROTATION */
int _Lmon_normalize_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Lmon_normalize_setpos] component Lmon_normalize=L_monitor() SETTING [L_monitor:0]");
  stracpy(_Lmon_normalize_var._name, "Lmon_normalize", 16384);
  stracpy(_Lmon_normalize_var._type, "L_monitor", 16384);
  _Lmon_normalize_var._index=6;
  int current_setpos_index = 6;
  _Lmon_normalize_var._parameters.nL = 2880;
  if("Lmon_guistart_normalize.dat" && strlen("Lmon_guistart_normalize.dat"))
    stracpy(_Lmon_normalize_var._parameters.filename, "Lmon_guistart_normalize.dat" ? "Lmon_guistart_normalize.dat" : "", 16384);
  else 
  _Lmon_normalize_var._parameters.filename[0]='\0';
  _Lmon_normalize_var._parameters.nowritefile = 0;
  _Lmon_normalize_var._parameters.xmin = -0.05;
  _Lmon_normalize_var._parameters.xmax = 0.05;
  _Lmon_normalize_var._parameters.ymin = -0.05;
  _Lmon_normalize_var._parameters.ymax = 0.05;
  _Lmon_normalize_var._parameters.xwidth = 0.10;
  _Lmon_normalize_var._parameters.yheight = 0.10;
  _Lmon_normalize_var._parameters.Lmin = 0;
  _Lmon_normalize_var._parameters.Lmax = 20;
  _Lmon_normalize_var._parameters.restore_neutron = 0;


  /* component Lmon_normalize=L_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Origin_var._rotation_absolute, _Lmon_normalize_var._rotation_absolute);
    rot_transpose(_Lmon_guistart_var._rotation_absolute, tr1);
    rot_mul(_Lmon_normalize_var._rotation_absolute, tr1, _Lmon_normalize_var._rotation_relative);
    _Lmon_normalize_var._rotation_is_identity =  rot_test_identity(_Lmon_normalize_var._rotation_relative);
    tc1 = coords_set(
      0, 0, _instrument_var._parameters.GUI_start -1e-6);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Lmon_normalize_var._position_absolute = coords_add(_Origin_var._position_absolute, tc2);
    tc1 = coords_sub(_Lmon_guistart_var._position_absolute, _Lmon_normalize_var._position_absolute);
    _Lmon_normalize_var._position_relative = rot_apply(_Lmon_normalize_var._rotation_absolute, tc1);
  } /* Lmon_normalize=L_monitor() AT ROTATED */
  DEBUG_COMPONENT("Lmon_normalize", _Lmon_normalize_var._position_absolute, _Lmon_normalize_var._rotation_absolute);
  instrument->_position_absolute[6] = _Lmon_normalize_var._position_absolute;
  instrument->_position_relative[6] = _Lmon_normalize_var._position_relative;
    _Lmon_normalize_var._position_relative_is_zero =  coords_test_zero(_Lmon_normalize_var._position_relative);
  instrument->counter_N[6]  = instrument->counter_P[6] = instrument->counter_P2[6] = 0;
  instrument->counter_AbsorbProp[6]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0005_Lmon_normalize", _Lmon_normalize_var._position_absolute, _Lmon_normalize_var._rotation_absolute, "L_monitor");
        mccomp_param_nexus(nxhandle,"0005_Lmon_normalize", "nL", "20", "2880","int");
        mccomp_param_nexus(nxhandle,"0005_Lmon_normalize", "filename", 0, "Lmon_guistart_normalize.dat", "char*");
        mccomp_param_nexus(nxhandle,"0005_Lmon_normalize", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0005_Lmon_normalize", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0005_Lmon_normalize", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0005_Lmon_normalize", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0005_Lmon_normalize", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0005_Lmon_normalize", "xwidth", "0", "0.10","MCNUM");
        mccomp_param_nexus(nxhandle,"0005_Lmon_normalize", "yheight", "0", "0.10","MCNUM");
        mccomp_param_nexus(nxhandle,"0005_Lmon_normalize", "Lmin", "NONE", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0005_Lmon_normalize", "Lmax", "NONE", "20","MCNUM");
        mccomp_param_nexus(nxhandle,"0005_Lmon_normalize", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Lmon_normalize_setpos */

/* component Guide1=Guide() SETTING, POSITION/ROTATION */
int _Guide1_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Guide1_setpos] component Guide1=Guide() SETTING [Guide:0]");
  stracpy(_Guide1_var._name, "Guide1", 16384);
  stracpy(_Guide1_var._type, "Guide", 16384);
  _Guide1_var._index=7;
  int current_setpos_index = 7;
  _Guide1_var._parameters.reflect[0]='\0';
  _Guide1_var._parameters.w1 = _instrument_var._parameters.GUI_w;
  _Guide1_var._parameters.h1 = _instrument_var._parameters.GUI_h;
  _Guide1_var._parameters.w2 = _instrument_var._parameters.W1;
  _Guide1_var._parameters.h2 = _instrument_var._parameters.H1;
  _Guide1_var._parameters.l = _instrument_var._parameters.FO1_DIST - _instrument_var._parameters.GUI_start - _instrument_var._parameters.GUI_GAP / 2;
  _Guide1_var._parameters.R0 = 1;
  _Guide1_var._parameters.Qc = 0.0219;
  _Guide1_var._parameters.alpha = _instrument_var._parameters.ALPHA;
  _Guide1_var._parameters.m = _instrument_var._parameters.M;
  _Guide1_var._parameters.W = 0.003;


  /* component Guide1=Guide() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Origin_var._rotation_absolute, _Guide1_var._rotation_absolute);
    rot_transpose(_Lmon_normalize_var._rotation_absolute, tr1);
    rot_mul(_Guide1_var._rotation_absolute, tr1, _Guide1_var._rotation_relative);
    _Guide1_var._rotation_is_identity =  rot_test_identity(_Guide1_var._rotation_relative);
    tc1 = coords_set(
      0, 0, _instrument_var._parameters.GUI_start);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Guide1_var._position_absolute = coords_add(_Origin_var._position_absolute, tc2);
    tc1 = coords_sub(_Lmon_normalize_var._position_absolute, _Guide1_var._position_absolute);
    _Guide1_var._position_relative = rot_apply(_Guide1_var._rotation_absolute, tc1);
  } /* Guide1=Guide() AT ROTATED */
  DEBUG_COMPONENT("Guide1", _Guide1_var._position_absolute, _Guide1_var._rotation_absolute);
  instrument->_position_absolute[7] = _Guide1_var._position_absolute;
  instrument->_position_relative[7] = _Guide1_var._position_relative;
    _Guide1_var._position_relative_is_zero =  coords_test_zero(_Guide1_var._position_relative);
  instrument->counter_N[7]  = instrument->counter_P[7] = instrument->counter_P2[7] = 0;
  instrument->counter_AbsorbProp[7]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0006_Guide1", _Guide1_var._position_absolute, _Guide1_var._rotation_absolute, "Guide");
        mccomp_param_nexus(nxhandle,"0006_Guide1", "reflect", 0, 0, "char*");
        mccomp_param_nexus(nxhandle,"0006_Guide1", "w1", "NONE", "_instrument_var._parameters.GUI_w","MCNUM");
        mccomp_param_nexus(nxhandle,"0006_Guide1", "h1", "NONE", "_instrument_var._parameters.GUI_h","MCNUM");
        mccomp_param_nexus(nxhandle,"0006_Guide1", "w2", "0", "_instrument_var._parameters.W1","MCNUM");
        mccomp_param_nexus(nxhandle,"0006_Guide1", "h2", "0", "_instrument_var._parameters.H1","MCNUM");
        mccomp_param_nexus(nxhandle,"0006_Guide1", "l", "NONE", "_instrument_var._parameters.FO1_DIST - _instrument_var._parameters.GUI_start - _instrument_var._parameters.GUI_GAP / 2","MCNUM");
        mccomp_param_nexus(nxhandle,"0006_Guide1", "R0", "0.99", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0006_Guide1", "Qc", "0.0219", "0.0219","MCNUM");
        mccomp_param_nexus(nxhandle,"0006_Guide1", "alpha", "6.07", "_instrument_var._parameters.ALPHA","MCNUM");
        mccomp_param_nexus(nxhandle,"0006_Guide1", "m", "2", "_instrument_var._parameters.M","MCNUM");
        mccomp_param_nexus(nxhandle,"0006_Guide1", "W", "0.003", "0.003","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Guide1_setpos */

/* component Lmonslow1=L_monitor() SETTING, POSITION/ROTATION */
int _Lmonslow1_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Lmonslow1_setpos] component Lmonslow1=L_monitor() SETTING [L_monitor:0]");
  stracpy(_Lmonslow1_var._name, "Lmonslow1", 16384);
  stracpy(_Lmonslow1_var._type, "L_monitor", 16384);
  _Lmonslow1_var._index=8;
  int current_setpos_index = 8;
  _Lmonslow1_var._parameters.nL = 200;
  if("Lmonslow1.dat" && strlen("Lmonslow1.dat"))
    stracpy(_Lmonslow1_var._parameters.filename, "Lmonslow1.dat" ? "Lmonslow1.dat" : "", 16384);
  else 
  _Lmonslow1_var._parameters.filename[0]='\0';
  _Lmonslow1_var._parameters.nowritefile = 0;
  _Lmonslow1_var._parameters.xmin = -0.05;
  _Lmonslow1_var._parameters.xmax = 0.05;
  _Lmonslow1_var._parameters.ymin = -0.05;
  _Lmonslow1_var._parameters.ymax = 0.05;
  _Lmonslow1_var._parameters.xwidth = 0.06;
  _Lmonslow1_var._parameters.yheight = 0.21;
  _Lmonslow1_var._parameters.Lmin = 0;
  _Lmonslow1_var._parameters.Lmax = _instrument_var._parameters.Lmax + 1;
  _Lmonslow1_var._parameters.restore_neutron = 0;


  /* component Lmonslow1=L_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Origin_var._rotation_absolute, _Lmonslow1_var._rotation_absolute);
    rot_transpose(_Guide1_var._rotation_absolute, tr1);
    rot_mul(_Lmonslow1_var._rotation_absolute, tr1, _Lmonslow1_var._rotation_relative);
    _Lmonslow1_var._rotation_is_identity =  rot_test_identity(_Lmonslow1_var._rotation_relative);
    tc1 = coords_set(
      0, 0, _instrument_var._parameters.FO1_DIST -2e-6);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Lmonslow1_var._position_absolute = coords_add(_Origin_var._position_absolute, tc2);
    tc1 = coords_sub(_Guide1_var._position_absolute, _Lmonslow1_var._position_absolute);
    _Lmonslow1_var._position_relative = rot_apply(_Lmonslow1_var._rotation_absolute, tc1);
  } /* Lmonslow1=L_monitor() AT ROTATED */
  DEBUG_COMPONENT("Lmonslow1", _Lmonslow1_var._position_absolute, _Lmonslow1_var._rotation_absolute);
  instrument->_position_absolute[8] = _Lmonslow1_var._position_absolute;
  instrument->_position_relative[8] = _Lmonslow1_var._position_relative;
    _Lmonslow1_var._position_relative_is_zero =  coords_test_zero(_Lmonslow1_var._position_relative);
  instrument->counter_N[8]  = instrument->counter_P[8] = instrument->counter_P2[8] = 0;
  instrument->counter_AbsorbProp[8]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0007_Lmonslow1", _Lmonslow1_var._position_absolute, _Lmonslow1_var._rotation_absolute, "L_monitor");
        mccomp_param_nexus(nxhandle,"0007_Lmonslow1", "nL", "20", "200","int");
        mccomp_param_nexus(nxhandle,"0007_Lmonslow1", "filename", 0, "Lmonslow1.dat", "char*");
        mccomp_param_nexus(nxhandle,"0007_Lmonslow1", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0007_Lmonslow1", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0007_Lmonslow1", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0007_Lmonslow1", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0007_Lmonslow1", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0007_Lmonslow1", "xwidth", "0", "0.06","MCNUM");
        mccomp_param_nexus(nxhandle,"0007_Lmonslow1", "yheight", "0", "0.21","MCNUM");
        mccomp_param_nexus(nxhandle,"0007_Lmonslow1", "Lmin", "NONE", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0007_Lmonslow1", "Lmax", "NONE", "_instrument_var._parameters.Lmax + 1","MCNUM");
        mccomp_param_nexus(nxhandle,"0007_Lmonslow1", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Lmonslow1_setpos */

/* component PSDslow1=PSD_monitor() SETTING, POSITION/ROTATION */
int _PSDslow1_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_PSDslow1_setpos] component PSDslow1=PSD_monitor() SETTING [PSD_monitor:0]");
  stracpy(_PSDslow1_var._name, "PSDslow1", 16384);
  stracpy(_PSDslow1_var._type, "PSD_monitor", 16384);
  _PSDslow1_var._index=9;
  int current_setpos_index = 9;
  _PSDslow1_var._parameters.nx = 90;
  _PSDslow1_var._parameters.ny = 90;
  if("PSDslow1.dat" && strlen("PSDslow1.dat"))
    stracpy(_PSDslow1_var._parameters.filename, "PSDslow1.dat" ? "PSDslow1.dat" : "", 16384);
  else 
  _PSDslow1_var._parameters.filename[0]='\0';
  _PSDslow1_var._parameters.xmin = -0.05;
  _PSDslow1_var._parameters.xmax = 0.05;
  _PSDslow1_var._parameters.ymin = -0.05;
  _PSDslow1_var._parameters.ymax = 0.05;
  _PSDslow1_var._parameters.xwidth = 0.1;
  _PSDslow1_var._parameters.yheight = 0.25;
  _PSDslow1_var._parameters.restore_neutron = 0;
  _PSDslow1_var._parameters.nowritefile = 0;


  /* component PSDslow1=PSD_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Origin_var._rotation_absolute, _PSDslow1_var._rotation_absolute);
    rot_transpose(_Lmonslow1_var._rotation_absolute, tr1);
    rot_mul(_PSDslow1_var._rotation_absolute, tr1, _PSDslow1_var._rotation_relative);
    _PSDslow1_var._rotation_is_identity =  rot_test_identity(_PSDslow1_var._rotation_relative);
    tc1 = coords_set(
      0, 0, _instrument_var._parameters.FO1_DIST -1e-6);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _PSDslow1_var._position_absolute = coords_add(_Origin_var._position_absolute, tc2);
    tc1 = coords_sub(_Lmonslow1_var._position_absolute, _PSDslow1_var._position_absolute);
    _PSDslow1_var._position_relative = rot_apply(_PSDslow1_var._rotation_absolute, tc1);
  } /* PSDslow1=PSD_monitor() AT ROTATED */
  DEBUG_COMPONENT("PSDslow1", _PSDslow1_var._position_absolute, _PSDslow1_var._rotation_absolute);
  instrument->_position_absolute[9] = _PSDslow1_var._position_absolute;
  instrument->_position_relative[9] = _PSDslow1_var._position_relative;
    _PSDslow1_var._position_relative_is_zero =  coords_test_zero(_PSDslow1_var._position_relative);
  instrument->counter_N[9]  = instrument->counter_P[9] = instrument->counter_P2[9] = 0;
  instrument->counter_AbsorbProp[9]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0008_PSDslow1", _PSDslow1_var._position_absolute, _PSDslow1_var._rotation_absolute, "PSD_monitor");
        mccomp_param_nexus(nxhandle,"0008_PSDslow1", "nx", "90", "90","int");
        mccomp_param_nexus(nxhandle,"0008_PSDslow1", "ny", "90", "90","int");
        mccomp_param_nexus(nxhandle,"0008_PSDslow1", "filename", 0, "PSDslow1.dat", "char*");
        mccomp_param_nexus(nxhandle,"0008_PSDslow1", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0008_PSDslow1", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0008_PSDslow1", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0008_PSDslow1", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0008_PSDslow1", "xwidth", "0", "0.1","MCNUM");
        mccomp_param_nexus(nxhandle,"0008_PSDslow1", "yheight", "0", "0.25","MCNUM");
        mccomp_param_nexus(nxhandle,"0008_PSDslow1", "restore_neutron", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0008_PSDslow1", "nowritefile", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _PSDslow1_setpos */

/* component FOchop1=DiskChopper() SETTING, POSITION/ROTATION */
int _FOchop1_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_FOchop1_setpos] component FOchop1=DiskChopper() SETTING [DiskChopper:0]");
  stracpy(_FOchop1_var._name, "FOchop1", 16384);
  stracpy(_FOchop1_var._type, "DiskChopper", 16384);
  _FOchop1_var._index=10;
  int current_setpos_index = 10;
  _FOchop1_var._parameters.theta_0 = _instrument_var._parameters.SLOW1_THETA;
  _FOchop1_var._parameters.radius = 0.6;
  _FOchop1_var._parameters.yheight = 0.20;
  _FOchop1_var._parameters.nu = _instrument_var._parameters.F_slow1;
  _FOchop1_var._parameters.nslit = 1;
  _FOchop1_var._parameters.jitter = 0;
  _FOchop1_var._parameters.delay = t_FO1;
  _FOchop1_var._parameters.isfirst = 0;
  _FOchop1_var._parameters.n_pulse = 1;
  _FOchop1_var._parameters.abs_out = 1;
  _FOchop1_var._parameters.phase = 0;
  _FOchop1_var._parameters.xwidth = 0;
  _FOchop1_var._parameters.verbose = 0;


  /* component FOchop1=DiskChopper() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Origin_var._rotation_absolute, _FOchop1_var._rotation_absolute);
    rot_transpose(_PSDslow1_var._rotation_absolute, tr1);
    rot_mul(_FOchop1_var._rotation_absolute, tr1, _FOchop1_var._rotation_relative);
    _FOchop1_var._rotation_is_identity =  rot_test_identity(_FOchop1_var._rotation_relative);
    tc1 = coords_set(
      0, 0, _instrument_var._parameters.FO1_DIST);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _FOchop1_var._position_absolute = coords_add(_Origin_var._position_absolute, tc2);
    tc1 = coords_sub(_PSDslow1_var._position_absolute, _FOchop1_var._position_absolute);
    _FOchop1_var._position_relative = rot_apply(_FOchop1_var._rotation_absolute, tc1);
  } /* FOchop1=DiskChopper() AT ROTATED */
  DEBUG_COMPONENT("FOchop1", _FOchop1_var._position_absolute, _FOchop1_var._rotation_absolute);
  instrument->_position_absolute[10] = _FOchop1_var._position_absolute;
  instrument->_position_relative[10] = _FOchop1_var._position_relative;
    _FOchop1_var._position_relative_is_zero =  coords_test_zero(_FOchop1_var._position_relative);
  instrument->counter_N[10]  = instrument->counter_P[10] = instrument->counter_P2[10] = 0;
  instrument->counter_AbsorbProp[10]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0009_FOchop1", _FOchop1_var._position_absolute, _FOchop1_var._rotation_absolute, "DiskChopper");
        mccomp_param_nexus(nxhandle,"0009_FOchop1", "theta_0", "0", "_instrument_var._parameters.SLOW1_THETA","MCNUM");
        mccomp_param_nexus(nxhandle,"0009_FOchop1", "radius", "0.5", "0.6","MCNUM");
        mccomp_param_nexus(nxhandle,"0009_FOchop1", "yheight", "NONE", "0.20","MCNUM");
        mccomp_param_nexus(nxhandle,"0009_FOchop1", "nu", "NONE", "_instrument_var._parameters.F_slow1","MCNUM");
        mccomp_param_nexus(nxhandle,"0009_FOchop1", "nslit", "3", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0009_FOchop1", "jitter", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0009_FOchop1", "delay", "0", "t_FO1","MCNUM");
        mccomp_param_nexus(nxhandle,"0009_FOchop1", "isfirst", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0009_FOchop1", "n_pulse", "1", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0009_FOchop1", "abs_out", "1", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0009_FOchop1", "phase", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0009_FOchop1", "xwidth", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0009_FOchop1", "verbose", "0", "0","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _FOchop1_setpos */

/* component TOFLmon1=TOFLambda_monitor() SETTING, POSITION/ROTATION */
int _TOFLmon1_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_TOFLmon1_setpos] component TOFLmon1=TOFLambda_monitor() SETTING [TOFLambda_monitor:0]");
  stracpy(_TOFLmon1_var._name, "TOFLmon1", 16384);
  stracpy(_TOFLmon1_var._type, "TOFLambda_monitor", 16384);
  _TOFLmon1_var._index=11;
  int current_setpos_index = 11;
  _TOFLmon1_var._parameters.nowritefile = 0;
  _TOFLmon1_var._parameters.nL = 200;
  _TOFLmon1_var._parameters.nt = 200;
  _TOFLmon1_var._parameters.tmin = 0;
  _TOFLmon1_var._parameters.tmax = 3e5;
  if("TOFLmon1.dat" && strlen("TOFLmon1.dat"))
    stracpy(_TOFLmon1_var._parameters.filename, "TOFLmon1.dat" ? "TOFLmon1.dat" : "", 16384);
  else 
  _TOFLmon1_var._parameters.filename[0]='\0';
  _TOFLmon1_var._parameters.xmin = -0.05;
  _TOFLmon1_var._parameters.xmax = 0.05;
  _TOFLmon1_var._parameters.ymin = -0.05;
  _TOFLmon1_var._parameters.ymax = 0.05;
  _TOFLmon1_var._parameters.xwidth = 0.05;
  _TOFLmon1_var._parameters.yheight = 0.21;
  _TOFLmon1_var._parameters.Lmin = 0;
  _TOFLmon1_var._parameters.Lmax = _instrument_var._parameters.Lmax + 1;
  _TOFLmon1_var._parameters.restore_neutron = 0;


  /* component TOFLmon1=TOFLambda_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _FOchop1_var._rotation_absolute, _TOFLmon1_var._rotation_absolute);
    rot_transpose(_FOchop1_var._rotation_absolute, tr1);
    rot_mul(_TOFLmon1_var._rotation_absolute, tr1, _TOFLmon1_var._rotation_relative);
    _TOFLmon1_var._rotation_is_identity =  rot_test_identity(_TOFLmon1_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1e-6);
    rot_transpose(_FOchop1_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _TOFLmon1_var._position_absolute = coords_add(_FOchop1_var._position_absolute, tc2);
    tc1 = coords_sub(_FOchop1_var._position_absolute, _TOFLmon1_var._position_absolute);
    _TOFLmon1_var._position_relative = rot_apply(_TOFLmon1_var._rotation_absolute, tc1);
  } /* TOFLmon1=TOFLambda_monitor() AT ROTATED */
  DEBUG_COMPONENT("TOFLmon1", _TOFLmon1_var._position_absolute, _TOFLmon1_var._rotation_absolute);
  instrument->_position_absolute[11] = _TOFLmon1_var._position_absolute;
  instrument->_position_relative[11] = _TOFLmon1_var._position_relative;
    _TOFLmon1_var._position_relative_is_zero =  coords_test_zero(_TOFLmon1_var._position_relative);
  instrument->counter_N[11]  = instrument->counter_P[11] = instrument->counter_P2[11] = 0;
  instrument->counter_AbsorbProp[11]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0010_TOFLmon1", _TOFLmon1_var._position_absolute, _TOFLmon1_var._rotation_absolute, "TOFLambda_monitor");
        mccomp_param_nexus(nxhandle,"0010_TOFLmon1", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0010_TOFLmon1", "nL", "20", "200","int");
        mccomp_param_nexus(nxhandle,"0010_TOFLmon1", "nt", "128", "200","int");
        mccomp_param_nexus(nxhandle,"0010_TOFLmon1", "tmin", "NONE", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_TOFLmon1", "tmax", "NONE", "3e5","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_TOFLmon1", "filename", 0, "TOFLmon1.dat", "char*");
        mccomp_param_nexus(nxhandle,"0010_TOFLmon1", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_TOFLmon1", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_TOFLmon1", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_TOFLmon1", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_TOFLmon1", "xwidth", "0", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_TOFLmon1", "yheight", "0", "0.21","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_TOFLmon1", "Lmin", "NONE", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_TOFLmon1", "Lmax", "NONE", "_instrument_var._parameters.Lmax + 1","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_TOFLmon1", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _TOFLmon1_setpos */

/* component Lmon_afterslow1=L_monitor() SETTING, POSITION/ROTATION */
int _Lmon_afterslow1_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Lmon_afterslow1_setpos] component Lmon_afterslow1=L_monitor() SETTING [L_monitor:0]");
  stracpy(_Lmon_afterslow1_var._name, "Lmon_afterslow1", 16384);
  stracpy(_Lmon_afterslow1_var._type, "L_monitor", 16384);
  _Lmon_afterslow1_var._index=12;
  int current_setpos_index = 12;
  _Lmon_afterslow1_var._parameters.nL = 200;
  if("Lmon_afterslow1.dat" && strlen("Lmon_afterslow1.dat"))
    stracpy(_Lmon_afterslow1_var._parameters.filename, "Lmon_afterslow1.dat" ? "Lmon_afterslow1.dat" : "", 16384);
  else 
  _Lmon_afterslow1_var._parameters.filename[0]='\0';
  _Lmon_afterslow1_var._parameters.nowritefile = 0;
  _Lmon_afterslow1_var._parameters.xmin = -0.05;
  _Lmon_afterslow1_var._parameters.xmax = 0.05;
  _Lmon_afterslow1_var._parameters.ymin = -0.05;
  _Lmon_afterslow1_var._parameters.ymax = 0.05;
  _Lmon_afterslow1_var._parameters.xwidth = 0.06;
  _Lmon_afterslow1_var._parameters.yheight = 0.21;
  _Lmon_afterslow1_var._parameters.Lmin = 0;
  _Lmon_afterslow1_var._parameters.Lmax = _instrument_var._parameters.Lmax + 1;
  _Lmon_afterslow1_var._parameters.restore_neutron = 0;


  /* component Lmon_afterslow1=L_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _FOchop1_var._rotation_absolute, _Lmon_afterslow1_var._rotation_absolute);
    rot_transpose(_TOFLmon1_var._rotation_absolute, tr1);
    rot_mul(_Lmon_afterslow1_var._rotation_absolute, tr1, _Lmon_afterslow1_var._rotation_relative);
    _Lmon_afterslow1_var._rotation_is_identity =  rot_test_identity(_Lmon_afterslow1_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 2e-6);
    rot_transpose(_FOchop1_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Lmon_afterslow1_var._position_absolute = coords_add(_FOchop1_var._position_absolute, tc2);
    tc1 = coords_sub(_TOFLmon1_var._position_absolute, _Lmon_afterslow1_var._position_absolute);
    _Lmon_afterslow1_var._position_relative = rot_apply(_Lmon_afterslow1_var._rotation_absolute, tc1);
  } /* Lmon_afterslow1=L_monitor() AT ROTATED */
  DEBUG_COMPONENT("Lmon_afterslow1", _Lmon_afterslow1_var._position_absolute, _Lmon_afterslow1_var._rotation_absolute);
  instrument->_position_absolute[12] = _Lmon_afterslow1_var._position_absolute;
  instrument->_position_relative[12] = _Lmon_afterslow1_var._position_relative;
    _Lmon_afterslow1_var._position_relative_is_zero =  coords_test_zero(_Lmon_afterslow1_var._position_relative);
  instrument->counter_N[12]  = instrument->counter_P[12] = instrument->counter_P2[12] = 0;
  instrument->counter_AbsorbProp[12]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0011_Lmon_afterslow1", _Lmon_afterslow1_var._position_absolute, _Lmon_afterslow1_var._rotation_absolute, "L_monitor");
        mccomp_param_nexus(nxhandle,"0011_Lmon_afterslow1", "nL", "20", "200","int");
        mccomp_param_nexus(nxhandle,"0011_Lmon_afterslow1", "filename", 0, "Lmon_afterslow1.dat", "char*");
        mccomp_param_nexus(nxhandle,"0011_Lmon_afterslow1", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0011_Lmon_afterslow1", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0011_Lmon_afterslow1", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0011_Lmon_afterslow1", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0011_Lmon_afterslow1", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0011_Lmon_afterslow1", "xwidth", "0", "0.06","MCNUM");
        mccomp_param_nexus(nxhandle,"0011_Lmon_afterslow1", "yheight", "0", "0.21","MCNUM");
        mccomp_param_nexus(nxhandle,"0011_Lmon_afterslow1", "Lmin", "NONE", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0011_Lmon_afterslow1", "Lmax", "NONE", "_instrument_var._parameters.Lmax + 1","MCNUM");
        mccomp_param_nexus(nxhandle,"0011_Lmon_afterslow1", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Lmon_afterslow1_setpos */

/* component PSD_afterslow1=PSD_monitor() SETTING, POSITION/ROTATION */
int _PSD_afterslow1_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_PSD_afterslow1_setpos] component PSD_afterslow1=PSD_monitor() SETTING [PSD_monitor:0]");
  stracpy(_PSD_afterslow1_var._name, "PSD_afterslow1", 16384);
  stracpy(_PSD_afterslow1_var._type, "PSD_monitor", 16384);
  _PSD_afterslow1_var._index=13;
  int current_setpos_index = 13;
  _PSD_afterslow1_var._parameters.nx = 90;
  _PSD_afterslow1_var._parameters.ny = 90;
  if("PSD_afterslow1.dat" && strlen("PSD_afterslow1.dat"))
    stracpy(_PSD_afterslow1_var._parameters.filename, "PSD_afterslow1.dat" ? "PSD_afterslow1.dat" : "", 16384);
  else 
  _PSD_afterslow1_var._parameters.filename[0]='\0';
  _PSD_afterslow1_var._parameters.xmin = -0.05;
  _PSD_afterslow1_var._parameters.xmax = 0.05;
  _PSD_afterslow1_var._parameters.ymin = -0.05;
  _PSD_afterslow1_var._parameters.ymax = 0.05;
  _PSD_afterslow1_var._parameters.xwidth = 0.1;
  _PSD_afterslow1_var._parameters.yheight = 0.25;
  _PSD_afterslow1_var._parameters.restore_neutron = 0;
  _PSD_afterslow1_var._parameters.nowritefile = 0;


  /* component PSD_afterslow1=PSD_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _FOchop1_var._rotation_absolute, _PSD_afterslow1_var._rotation_absolute);
    rot_transpose(_Lmon_afterslow1_var._rotation_absolute, tr1);
    rot_mul(_PSD_afterslow1_var._rotation_absolute, tr1, _PSD_afterslow1_var._rotation_relative);
    _PSD_afterslow1_var._rotation_is_identity =  rot_test_identity(_PSD_afterslow1_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 3e-6);
    rot_transpose(_FOchop1_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _PSD_afterslow1_var._position_absolute = coords_add(_FOchop1_var._position_absolute, tc2);
    tc1 = coords_sub(_Lmon_afterslow1_var._position_absolute, _PSD_afterslow1_var._position_absolute);
    _PSD_afterslow1_var._position_relative = rot_apply(_PSD_afterslow1_var._rotation_absolute, tc1);
  } /* PSD_afterslow1=PSD_monitor() AT ROTATED */
  DEBUG_COMPONENT("PSD_afterslow1", _PSD_afterslow1_var._position_absolute, _PSD_afterslow1_var._rotation_absolute);
  instrument->_position_absolute[13] = _PSD_afterslow1_var._position_absolute;
  instrument->_position_relative[13] = _PSD_afterslow1_var._position_relative;
    _PSD_afterslow1_var._position_relative_is_zero =  coords_test_zero(_PSD_afterslow1_var._position_relative);
  instrument->counter_N[13]  = instrument->counter_P[13] = instrument->counter_P2[13] = 0;
  instrument->counter_AbsorbProp[13]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0012_PSD_afterslow1", _PSD_afterslow1_var._position_absolute, _PSD_afterslow1_var._rotation_absolute, "PSD_monitor");
        mccomp_param_nexus(nxhandle,"0012_PSD_afterslow1", "nx", "90", "90","int");
        mccomp_param_nexus(nxhandle,"0012_PSD_afterslow1", "ny", "90", "90","int");
        mccomp_param_nexus(nxhandle,"0012_PSD_afterslow1", "filename", 0, "PSD_afterslow1.dat", "char*");
        mccomp_param_nexus(nxhandle,"0012_PSD_afterslow1", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0012_PSD_afterslow1", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0012_PSD_afterslow1", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0012_PSD_afterslow1", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0012_PSD_afterslow1", "xwidth", "0", "0.1","MCNUM");
        mccomp_param_nexus(nxhandle,"0012_PSD_afterslow1", "yheight", "0", "0.25","MCNUM");
        mccomp_param_nexus(nxhandle,"0012_PSD_afterslow1", "restore_neutron", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0012_PSD_afterslow1", "nowritefile", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _PSD_afterslow1_setpos */

/* component Guidelong1=Guide() SETTING, POSITION/ROTATION */
int _Guidelong1_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Guidelong1_setpos] component Guidelong1=Guide() SETTING [Guide:0]");
  stracpy(_Guidelong1_var._name, "Guidelong1", 16384);
  stracpy(_Guidelong1_var._type, "Guide", 16384);
  _Guidelong1_var._index=14;
  int current_setpos_index = 14;
  _Guidelong1_var._parameters.reflect[0]='\0';
  _Guidelong1_var._parameters.w1 = _instrument_var._parameters.W1;
  _Guidelong1_var._parameters.h1 = _instrument_var._parameters.H1;
  _Guidelong1_var._parameters.w2 = _instrument_var._parameters.W2;
  _Guidelong1_var._parameters.h2 = _instrument_var._parameters.H2;
  _Guidelong1_var._parameters.l = _instrument_var._parameters.L_ballistic_begin + _instrument_var._parameters.GUI_start - _instrument_var._parameters.FO1_DIST - _instrument_var._parameters.GUI_GAP / 2;
  _Guidelong1_var._parameters.R0 = 1;
  _Guidelong1_var._parameters.Qc = 0.0219;
  _Guidelong1_var._parameters.alpha = _instrument_var._parameters.ALPHA;
  _Guidelong1_var._parameters.m = _instrument_var._parameters.M;
  _Guidelong1_var._parameters.W = 0.003;


  /* component Guidelong1=Guide() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _FOchop1_var._rotation_absolute, _Guidelong1_var._rotation_absolute);
    rot_transpose(_PSD_afterslow1_var._rotation_absolute, tr1);
    rot_mul(_Guidelong1_var._rotation_absolute, tr1, _Guidelong1_var._rotation_relative);
    _Guidelong1_var._rotation_is_identity =  rot_test_identity(_Guidelong1_var._rotation_relative);
    tc1 = coords_set(
      0, 0, _instrument_var._parameters.GUI_GAP / 2);
    rot_transpose(_FOchop1_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Guidelong1_var._position_absolute = coords_add(_FOchop1_var._position_absolute, tc2);
    tc1 = coords_sub(_PSD_afterslow1_var._position_absolute, _Guidelong1_var._position_absolute);
    _Guidelong1_var._position_relative = rot_apply(_Guidelong1_var._rotation_absolute, tc1);
  } /* Guidelong1=Guide() AT ROTATED */
  DEBUG_COMPONENT("Guidelong1", _Guidelong1_var._position_absolute, _Guidelong1_var._rotation_absolute);
  instrument->_position_absolute[14] = _Guidelong1_var._position_absolute;
  instrument->_position_relative[14] = _Guidelong1_var._position_relative;
    _Guidelong1_var._position_relative_is_zero =  coords_test_zero(_Guidelong1_var._position_relative);
  instrument->counter_N[14]  = instrument->counter_P[14] = instrument->counter_P2[14] = 0;
  instrument->counter_AbsorbProp[14]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0013_Guidelong1", _Guidelong1_var._position_absolute, _Guidelong1_var._rotation_absolute, "Guide");
        mccomp_param_nexus(nxhandle,"0013_Guidelong1", "reflect", 0, 0, "char*");
        mccomp_param_nexus(nxhandle,"0013_Guidelong1", "w1", "NONE", "_instrument_var._parameters.W1","MCNUM");
        mccomp_param_nexus(nxhandle,"0013_Guidelong1", "h1", "NONE", "_instrument_var._parameters.H1","MCNUM");
        mccomp_param_nexus(nxhandle,"0013_Guidelong1", "w2", "0", "_instrument_var._parameters.W2","MCNUM");
        mccomp_param_nexus(nxhandle,"0013_Guidelong1", "h2", "0", "_instrument_var._parameters.H2","MCNUM");
        mccomp_param_nexus(nxhandle,"0013_Guidelong1", "l", "NONE", "_instrument_var._parameters.L_ballistic_begin + _instrument_var._parameters.GUI_start - _instrument_var._parameters.FO1_DIST - _instrument_var._parameters.GUI_GAP / 2","MCNUM");
        mccomp_param_nexus(nxhandle,"0013_Guidelong1", "R0", "0.99", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0013_Guidelong1", "Qc", "0.0219", "0.0219","MCNUM");
        mccomp_param_nexus(nxhandle,"0013_Guidelong1", "alpha", "6.07", "_instrument_var._parameters.ALPHA","MCNUM");
        mccomp_param_nexus(nxhandle,"0013_Guidelong1", "m", "2", "_instrument_var._parameters.M","MCNUM");
        mccomp_param_nexus(nxhandle,"0013_Guidelong1", "W", "0.003", "0.003","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Guidelong1_setpos */

/* component Guidelong1b=Guide() SETTING, POSITION/ROTATION */
int _Guidelong1b_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Guidelong1b_setpos] component Guidelong1b=Guide() SETTING [Guide:0]");
  stracpy(_Guidelong1b_var._name, "Guidelong1b", 16384);
  stracpy(_Guidelong1b_var._type, "Guide", 16384);
  _Guidelong1b_var._index=15;
  int current_setpos_index = 15;
  _Guidelong1b_var._parameters.reflect[0]='\0';
  _Guidelong1b_var._parameters.w1 = _instrument_var._parameters.W2;
  _Guidelong1b_var._parameters.h1 = _instrument_var._parameters.H2;
  _Guidelong1b_var._parameters.w2 = _instrument_var._parameters.W3;
  _Guidelong1b_var._parameters.h2 = _instrument_var._parameters.H3;
  _Guidelong1b_var._parameters.l = _instrument_var._parameters.Length / 2 - _instrument_var._parameters.L_ballistic_begin - _instrument_var._parameters.GUI_start - _instrument_var._parameters.GUI_GAP / 2;
  _Guidelong1b_var._parameters.R0 = 1;
  _Guidelong1b_var._parameters.Qc = 0.0219;
  _Guidelong1b_var._parameters.alpha = _instrument_var._parameters.ALPHA;
  _Guidelong1b_var._parameters.m = _instrument_var._parameters.M;
  _Guidelong1b_var._parameters.W = 0.003;


  /* component Guidelong1b=Guide() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Origin_var._rotation_absolute, _Guidelong1b_var._rotation_absolute);
    rot_transpose(_Guidelong1_var._rotation_absolute, tr1);
    rot_mul(_Guidelong1b_var._rotation_absolute, tr1, _Guidelong1b_var._rotation_relative);
    _Guidelong1b_var._rotation_is_identity =  rot_test_identity(_Guidelong1b_var._rotation_relative);
    tc1 = coords_set(
      0, 0, _instrument_var._parameters.L_ballistic_begin + _instrument_var._parameters.GUI_start);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Guidelong1b_var._position_absolute = coords_add(_Origin_var._position_absolute, tc2);
    tc1 = coords_sub(_Guidelong1_var._position_absolute, _Guidelong1b_var._position_absolute);
    _Guidelong1b_var._position_relative = rot_apply(_Guidelong1b_var._rotation_absolute, tc1);
  } /* Guidelong1b=Guide() AT ROTATED */
  DEBUG_COMPONENT("Guidelong1b", _Guidelong1b_var._position_absolute, _Guidelong1b_var._rotation_absolute);
  instrument->_position_absolute[15] = _Guidelong1b_var._position_absolute;
  instrument->_position_relative[15] = _Guidelong1b_var._position_relative;
    _Guidelong1b_var._position_relative_is_zero =  coords_test_zero(_Guidelong1b_var._position_relative);
  instrument->counter_N[15]  = instrument->counter_P[15] = instrument->counter_P2[15] = 0;
  instrument->counter_AbsorbProp[15]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0014_Guidelong1b", _Guidelong1b_var._position_absolute, _Guidelong1b_var._rotation_absolute, "Guide");
        mccomp_param_nexus(nxhandle,"0014_Guidelong1b", "reflect", 0, 0, "char*");
        mccomp_param_nexus(nxhandle,"0014_Guidelong1b", "w1", "NONE", "_instrument_var._parameters.W2","MCNUM");
        mccomp_param_nexus(nxhandle,"0014_Guidelong1b", "h1", "NONE", "_instrument_var._parameters.H2","MCNUM");
        mccomp_param_nexus(nxhandle,"0014_Guidelong1b", "w2", "0", "_instrument_var._parameters.W3","MCNUM");
        mccomp_param_nexus(nxhandle,"0014_Guidelong1b", "h2", "0", "_instrument_var._parameters.H3","MCNUM");
        mccomp_param_nexus(nxhandle,"0014_Guidelong1b", "l", "NONE", "_instrument_var._parameters.Length / 2 - _instrument_var._parameters.L_ballistic_begin - _instrument_var._parameters.GUI_start - _instrument_var._parameters.GUI_GAP / 2","MCNUM");
        mccomp_param_nexus(nxhandle,"0014_Guidelong1b", "R0", "0.99", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0014_Guidelong1b", "Qc", "0.0219", "0.0219","MCNUM");
        mccomp_param_nexus(nxhandle,"0014_Guidelong1b", "alpha", "6.07", "_instrument_var._parameters.ALPHA","MCNUM");
        mccomp_param_nexus(nxhandle,"0014_Guidelong1b", "m", "2", "_instrument_var._parameters.M","MCNUM");
        mccomp_param_nexus(nxhandle,"0014_Guidelong1b", "W", "0.003", "0.003","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Guidelong1b_setpos */

/* component Lmon_slow2=L_monitor() SETTING, POSITION/ROTATION */
int _Lmon_slow2_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Lmon_slow2_setpos] component Lmon_slow2=L_monitor() SETTING [L_monitor:0]");
  stracpy(_Lmon_slow2_var._name, "Lmon_slow2", 16384);
  stracpy(_Lmon_slow2_var._type, "L_monitor", 16384);
  _Lmon_slow2_var._index=16;
  int current_setpos_index = 16;
  _Lmon_slow2_var._parameters.nL = 200;
  if("Lmon_slow2.dat" && strlen("Lmon_slow2.dat"))
    stracpy(_Lmon_slow2_var._parameters.filename, "Lmon_slow2.dat" ? "Lmon_slow2.dat" : "", 16384);
  else 
  _Lmon_slow2_var._parameters.filename[0]='\0';
  _Lmon_slow2_var._parameters.nowritefile = 0;
  _Lmon_slow2_var._parameters.xmin = -0.05;
  _Lmon_slow2_var._parameters.xmax = 0.05;
  _Lmon_slow2_var._parameters.ymin = -0.05;
  _Lmon_slow2_var._parameters.ymax = 0.05;
  _Lmon_slow2_var._parameters.xwidth = 0.06;
  _Lmon_slow2_var._parameters.yheight = 0.21;
  _Lmon_slow2_var._parameters.Lmin = 0;
  _Lmon_slow2_var._parameters.Lmax = _instrument_var._parameters.Lmax + 1;
  _Lmon_slow2_var._parameters.restore_neutron = 0;


  /* component Lmon_slow2=L_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Origin_var._rotation_absolute, _Lmon_slow2_var._rotation_absolute);
    rot_transpose(_Guidelong1b_var._rotation_absolute, tr1);
    rot_mul(_Lmon_slow2_var._rotation_absolute, tr1, _Lmon_slow2_var._rotation_relative);
    _Lmon_slow2_var._rotation_is_identity =  rot_test_identity(_Lmon_slow2_var._rotation_relative);
    tc1 = coords_set(
      0, 0, _instrument_var._parameters.Length / 2 -1e-6);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Lmon_slow2_var._position_absolute = coords_add(_Origin_var._position_absolute, tc2);
    tc1 = coords_sub(_Guidelong1b_var._position_absolute, _Lmon_slow2_var._position_absolute);
    _Lmon_slow2_var._position_relative = rot_apply(_Lmon_slow2_var._rotation_absolute, tc1);
  } /* Lmon_slow2=L_monitor() AT ROTATED */
  DEBUG_COMPONENT("Lmon_slow2", _Lmon_slow2_var._position_absolute, _Lmon_slow2_var._rotation_absolute);
  instrument->_position_absolute[16] = _Lmon_slow2_var._position_absolute;
  instrument->_position_relative[16] = _Lmon_slow2_var._position_relative;
    _Lmon_slow2_var._position_relative_is_zero =  coords_test_zero(_Lmon_slow2_var._position_relative);
  instrument->counter_N[16]  = instrument->counter_P[16] = instrument->counter_P2[16] = 0;
  instrument->counter_AbsorbProp[16]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0015_Lmon_slow2", _Lmon_slow2_var._position_absolute, _Lmon_slow2_var._rotation_absolute, "L_monitor");
        mccomp_param_nexus(nxhandle,"0015_Lmon_slow2", "nL", "20", "200","int");
        mccomp_param_nexus(nxhandle,"0015_Lmon_slow2", "filename", 0, "Lmon_slow2.dat", "char*");
        mccomp_param_nexus(nxhandle,"0015_Lmon_slow2", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0015_Lmon_slow2", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0015_Lmon_slow2", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0015_Lmon_slow2", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0015_Lmon_slow2", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0015_Lmon_slow2", "xwidth", "0", "0.06","MCNUM");
        mccomp_param_nexus(nxhandle,"0015_Lmon_slow2", "yheight", "0", "0.21","MCNUM");
        mccomp_param_nexus(nxhandle,"0015_Lmon_slow2", "Lmin", "NONE", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0015_Lmon_slow2", "Lmax", "NONE", "_instrument_var._parameters.Lmax + 1","MCNUM");
        mccomp_param_nexus(nxhandle,"0015_Lmon_slow2", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Lmon_slow2_setpos */

/* component FOchop2=DiskChopper() SETTING, POSITION/ROTATION */
int _FOchop2_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_FOchop2_setpos] component FOchop2=DiskChopper() SETTING [DiskChopper:0]");
  stracpy(_FOchop2_var._name, "FOchop2", 16384);
  stracpy(_FOchop2_var._type, "DiskChopper", 16384);
  _FOchop2_var._index=17;
  int current_setpos_index = 17;
  _FOchop2_var._parameters.theta_0 = 155;
  _FOchop2_var._parameters.radius = 0.6;
  _FOchop2_var._parameters.yheight = 0.6;
  _FOchop2_var._parameters.nu = _instrument_var._parameters.F_slow2;
  _FOchop2_var._parameters.nslit = 1;
  _FOchop2_var._parameters.jitter = 0;
  _FOchop2_var._parameters.delay = t_FO2;
  _FOchop2_var._parameters.isfirst = 0;
  _FOchop2_var._parameters.n_pulse = 1;
  _FOchop2_var._parameters.abs_out = 1;
  _FOchop2_var._parameters.phase = 0;
  _FOchop2_var._parameters.xwidth = 0;
  _FOchop2_var._parameters.verbose = 0;


  /* component FOchop2=DiskChopper() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Origin_var._rotation_absolute, _FOchop2_var._rotation_absolute);
    rot_transpose(_Lmon_slow2_var._rotation_absolute, tr1);
    rot_mul(_FOchop2_var._rotation_absolute, tr1, _FOchop2_var._rotation_relative);
    _FOchop2_var._rotation_is_identity =  rot_test_identity(_FOchop2_var._rotation_relative);
    tc1 = coords_set(
      0, 0.1, _instrument_var._parameters.Length / 2);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _FOchop2_var._position_absolute = coords_add(_Origin_var._position_absolute, tc2);
    tc1 = coords_sub(_Lmon_slow2_var._position_absolute, _FOchop2_var._position_absolute);
    _FOchop2_var._position_relative = rot_apply(_FOchop2_var._rotation_absolute, tc1);
  } /* FOchop2=DiskChopper() AT ROTATED */
  DEBUG_COMPONENT("FOchop2", _FOchop2_var._position_absolute, _FOchop2_var._rotation_absolute);
  instrument->_position_absolute[17] = _FOchop2_var._position_absolute;
  instrument->_position_relative[17] = _FOchop2_var._position_relative;
    _FOchop2_var._position_relative_is_zero =  coords_test_zero(_FOchop2_var._position_relative);
  instrument->counter_N[17]  = instrument->counter_P[17] = instrument->counter_P2[17] = 0;
  instrument->counter_AbsorbProp[17]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0016_FOchop2", _FOchop2_var._position_absolute, _FOchop2_var._rotation_absolute, "DiskChopper");
        mccomp_param_nexus(nxhandle,"0016_FOchop2", "theta_0", "0", "155","MCNUM");
        mccomp_param_nexus(nxhandle,"0016_FOchop2", "radius", "0.5", "0.6","MCNUM");
        mccomp_param_nexus(nxhandle,"0016_FOchop2", "yheight", "NONE", "0.6","MCNUM");
        mccomp_param_nexus(nxhandle,"0016_FOchop2", "nu", "NONE", "_instrument_var._parameters.F_slow2","MCNUM");
        mccomp_param_nexus(nxhandle,"0016_FOchop2", "nslit", "3", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0016_FOchop2", "jitter", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0016_FOchop2", "delay", "0", "t_FO2","MCNUM");
        mccomp_param_nexus(nxhandle,"0016_FOchop2", "isfirst", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0016_FOchop2", "n_pulse", "1", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0016_FOchop2", "abs_out", "1", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0016_FOchop2", "phase", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0016_FOchop2", "xwidth", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0016_FOchop2", "verbose", "0", "0","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _FOchop2_setpos */

/* component Fastchop1=DiskChopper() SETTING, POSITION/ROTATION */
int _Fastchop1_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Fastchop1_setpos] component Fastchop1=DiskChopper() SETTING [DiskChopper:0]");
  stracpy(_Fastchop1_var._name, "Fastchop1", 16384);
  stracpy(_Fastchop1_var._type, "DiskChopper", 16384);
  _Fastchop1_var._index=18;
  int current_setpos_index = 18;
  _Fastchop1_var._parameters.theta_0 = _instrument_var._parameters.THETA_fast1;
  _Fastchop1_var._parameters.radius = 0.5;
  _Fastchop1_var._parameters.yheight = 0.5;
  _Fastchop1_var._parameters.nu = _instrument_var._parameters.F_fast1;
  _Fastchop1_var._parameters.nslit = _instrument_var._parameters.N_fast;
  _Fastchop1_var._parameters.jitter = 0;
  _Fastchop1_var._parameters.delay = t_fast1;
  _Fastchop1_var._parameters.isfirst = 0;
  _Fastchop1_var._parameters.n_pulse = 1;
  _Fastchop1_var._parameters.abs_out = 1;
  _Fastchop1_var._parameters.phase = 0;
  _Fastchop1_var._parameters.xwidth = 0;
  _Fastchop1_var._parameters.verbose = 0;


  /* component Fastchop1=DiskChopper() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Origin_var._rotation_absolute, _Fastchop1_var._rotation_absolute);
    rot_transpose(_FOchop2_var._rotation_absolute, tr1);
    rot_mul(_Fastchop1_var._rotation_absolute, tr1, _Fastchop1_var._rotation_relative);
    _Fastchop1_var._rotation_is_identity =  rot_test_identity(_Fastchop1_var._rotation_relative);
    tc1 = coords_set(
      0, 0.1, _instrument_var._parameters.Length / 2 + _instrument_var._parameters.GUI_GAP);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Fastchop1_var._position_absolute = coords_add(_Origin_var._position_absolute, tc2);
    tc1 = coords_sub(_FOchop2_var._position_absolute, _Fastchop1_var._position_absolute);
    _Fastchop1_var._position_relative = rot_apply(_Fastchop1_var._rotation_absolute, tc1);
  } /* Fastchop1=DiskChopper() AT ROTATED */
  DEBUG_COMPONENT("Fastchop1", _Fastchop1_var._position_absolute, _Fastchop1_var._rotation_absolute);
  instrument->_position_absolute[18] = _Fastchop1_var._position_absolute;
  instrument->_position_relative[18] = _Fastchop1_var._position_relative;
    _Fastchop1_var._position_relative_is_zero =  coords_test_zero(_Fastchop1_var._position_relative);
  instrument->counter_N[18]  = instrument->counter_P[18] = instrument->counter_P2[18] = 0;
  instrument->counter_AbsorbProp[18]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0017_Fastchop1", _Fastchop1_var._position_absolute, _Fastchop1_var._rotation_absolute, "DiskChopper");
        mccomp_param_nexus(nxhandle,"0017_Fastchop1", "theta_0", "0", "_instrument_var._parameters.THETA_fast1","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_Fastchop1", "radius", "0.5", "0.5","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_Fastchop1", "yheight", "NONE", "0.5","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_Fastchop1", "nu", "NONE", "_instrument_var._parameters.F_fast1","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_Fastchop1", "nslit", "3", "_instrument_var._parameters.N_fast","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_Fastchop1", "jitter", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_Fastchop1", "delay", "0", "t_fast1","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_Fastchop1", "isfirst", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_Fastchop1", "n_pulse", "1", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_Fastchop1", "abs_out", "1", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_Fastchop1", "phase", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_Fastchop1", "xwidth", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_Fastchop1", "verbose", "0", "0","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Fastchop1_setpos */

/* component PSD_afterslow2=PSD_monitor() SETTING, POSITION/ROTATION */
int _PSD_afterslow2_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_PSD_afterslow2_setpos] component PSD_afterslow2=PSD_monitor() SETTING [PSD_monitor:0]");
  stracpy(_PSD_afterslow2_var._name, "PSD_afterslow2", 16384);
  stracpy(_PSD_afterslow2_var._type, "PSD_monitor", 16384);
  _PSD_afterslow2_var._index=19;
  int current_setpos_index = 19;
  _PSD_afterslow2_var._parameters.nx = 90;
  _PSD_afterslow2_var._parameters.ny = 90;
  if("PSD_afterslow2.dat" && strlen("PSD_afterslow2.dat"))
    stracpy(_PSD_afterslow2_var._parameters.filename, "PSD_afterslow2.dat" ? "PSD_afterslow2.dat" : "", 16384);
  else 
  _PSD_afterslow2_var._parameters.filename[0]='\0';
  _PSD_afterslow2_var._parameters.xmin = -0.05;
  _PSD_afterslow2_var._parameters.xmax = 0.05;
  _PSD_afterslow2_var._parameters.ymin = -0.05;
  _PSD_afterslow2_var._parameters.ymax = 0.05;
  _PSD_afterslow2_var._parameters.xwidth = 0.1;
  _PSD_afterslow2_var._parameters.yheight = 0.25;
  _PSD_afterslow2_var._parameters.restore_neutron = 0;
  _PSD_afterslow2_var._parameters.nowritefile = 0;


  /* component PSD_afterslow2=PSD_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Fastchop1_var._rotation_absolute, _PSD_afterslow2_var._rotation_absolute);
    rot_transpose(_Fastchop1_var._rotation_absolute, tr1);
    rot_mul(_PSD_afterslow2_var._rotation_absolute, tr1, _PSD_afterslow2_var._rotation_relative);
    _PSD_afterslow2_var._rotation_is_identity =  rot_test_identity(_PSD_afterslow2_var._rotation_relative);
    tc1 = coords_set(
      0, -0.1, 1e-6);
    rot_transpose(_Fastchop1_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _PSD_afterslow2_var._position_absolute = coords_add(_Fastchop1_var._position_absolute, tc2);
    tc1 = coords_sub(_Fastchop1_var._position_absolute, _PSD_afterslow2_var._position_absolute);
    _PSD_afterslow2_var._position_relative = rot_apply(_PSD_afterslow2_var._rotation_absolute, tc1);
  } /* PSD_afterslow2=PSD_monitor() AT ROTATED */
  DEBUG_COMPONENT("PSD_afterslow2", _PSD_afterslow2_var._position_absolute, _PSD_afterslow2_var._rotation_absolute);
  instrument->_position_absolute[19] = _PSD_afterslow2_var._position_absolute;
  instrument->_position_relative[19] = _PSD_afterslow2_var._position_relative;
    _PSD_afterslow2_var._position_relative_is_zero =  coords_test_zero(_PSD_afterslow2_var._position_relative);
  instrument->counter_N[19]  = instrument->counter_P[19] = instrument->counter_P2[19] = 0;
  instrument->counter_AbsorbProp[19]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0018_PSD_afterslow2", _PSD_afterslow2_var._position_absolute, _PSD_afterslow2_var._rotation_absolute, "PSD_monitor");
        mccomp_param_nexus(nxhandle,"0018_PSD_afterslow2", "nx", "90", "90","int");
        mccomp_param_nexus(nxhandle,"0018_PSD_afterslow2", "ny", "90", "90","int");
        mccomp_param_nexus(nxhandle,"0018_PSD_afterslow2", "filename", 0, "PSD_afterslow2.dat", "char*");
        mccomp_param_nexus(nxhandle,"0018_PSD_afterslow2", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0018_PSD_afterslow2", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0018_PSD_afterslow2", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0018_PSD_afterslow2", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0018_PSD_afterslow2", "xwidth", "0", "0.1","MCNUM");
        mccomp_param_nexus(nxhandle,"0018_PSD_afterslow2", "yheight", "0", "0.25","MCNUM");
        mccomp_param_nexus(nxhandle,"0018_PSD_afterslow2", "restore_neutron", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0018_PSD_afterslow2", "nowritefile", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _PSD_afterslow2_setpos */

/* component Lmon_afterslow2=L_monitor() SETTING, POSITION/ROTATION */
int _Lmon_afterslow2_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Lmon_afterslow2_setpos] component Lmon_afterslow2=L_monitor() SETTING [L_monitor:0]");
  stracpy(_Lmon_afterslow2_var._name, "Lmon_afterslow2", 16384);
  stracpy(_Lmon_afterslow2_var._type, "L_monitor", 16384);
  _Lmon_afterslow2_var._index=20;
  int current_setpos_index = 20;
  _Lmon_afterslow2_var._parameters.nL = 200;
  if("Lmon_afterslow2.dat" && strlen("Lmon_afterslow2.dat"))
    stracpy(_Lmon_afterslow2_var._parameters.filename, "Lmon_afterslow2.dat" ? "Lmon_afterslow2.dat" : "", 16384);
  else 
  _Lmon_afterslow2_var._parameters.filename[0]='\0';
  _Lmon_afterslow2_var._parameters.nowritefile = 0;
  _Lmon_afterslow2_var._parameters.xmin = -0.05;
  _Lmon_afterslow2_var._parameters.xmax = 0.05;
  _Lmon_afterslow2_var._parameters.ymin = -0.05;
  _Lmon_afterslow2_var._parameters.ymax = 0.05;
  _Lmon_afterslow2_var._parameters.xwidth = 0.06;
  _Lmon_afterslow2_var._parameters.yheight = 0.21;
  _Lmon_afterslow2_var._parameters.Lmin = 0;
  _Lmon_afterslow2_var._parameters.Lmax = _instrument_var._parameters.Lmax + 1;
  _Lmon_afterslow2_var._parameters.restore_neutron = 0;


  /* component Lmon_afterslow2=L_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _PSD_afterslow2_var._rotation_absolute, _Lmon_afterslow2_var._rotation_absolute);
    rot_transpose(_PSD_afterslow2_var._rotation_absolute, tr1);
    rot_mul(_Lmon_afterslow2_var._rotation_absolute, tr1, _Lmon_afterslow2_var._rotation_relative);
    _Lmon_afterslow2_var._rotation_is_identity =  rot_test_identity(_Lmon_afterslow2_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1e-6);
    rot_transpose(_PSD_afterslow2_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Lmon_afterslow2_var._position_absolute = coords_add(_PSD_afterslow2_var._position_absolute, tc2);
    tc1 = coords_sub(_PSD_afterslow2_var._position_absolute, _Lmon_afterslow2_var._position_absolute);
    _Lmon_afterslow2_var._position_relative = rot_apply(_Lmon_afterslow2_var._rotation_absolute, tc1);
  } /* Lmon_afterslow2=L_monitor() AT ROTATED */
  DEBUG_COMPONENT("Lmon_afterslow2", _Lmon_afterslow2_var._position_absolute, _Lmon_afterslow2_var._rotation_absolute);
  instrument->_position_absolute[20] = _Lmon_afterslow2_var._position_absolute;
  instrument->_position_relative[20] = _Lmon_afterslow2_var._position_relative;
    _Lmon_afterslow2_var._position_relative_is_zero =  coords_test_zero(_Lmon_afterslow2_var._position_relative);
  instrument->counter_N[20]  = instrument->counter_P[20] = instrument->counter_P2[20] = 0;
  instrument->counter_AbsorbProp[20]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0019_Lmon_afterslow2", _Lmon_afterslow2_var._position_absolute, _Lmon_afterslow2_var._rotation_absolute, "L_monitor");
        mccomp_param_nexus(nxhandle,"0019_Lmon_afterslow2", "nL", "20", "200","int");
        mccomp_param_nexus(nxhandle,"0019_Lmon_afterslow2", "filename", 0, "Lmon_afterslow2.dat", "char*");
        mccomp_param_nexus(nxhandle,"0019_Lmon_afterslow2", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0019_Lmon_afterslow2", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0019_Lmon_afterslow2", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0019_Lmon_afterslow2", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0019_Lmon_afterslow2", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0019_Lmon_afterslow2", "xwidth", "0", "0.06","MCNUM");
        mccomp_param_nexus(nxhandle,"0019_Lmon_afterslow2", "yheight", "0", "0.21","MCNUM");
        mccomp_param_nexus(nxhandle,"0019_Lmon_afterslow2", "Lmin", "NONE", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0019_Lmon_afterslow2", "Lmax", "NONE", "_instrument_var._parameters.Lmax + 1","MCNUM");
        mccomp_param_nexus(nxhandle,"0019_Lmon_afterslow2", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Lmon_afterslow2_setpos */

/* component TOFL_afterslow2=TOFLambda_monitor() SETTING, POSITION/ROTATION */
int _TOFL_afterslow2_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_TOFL_afterslow2_setpos] component TOFL_afterslow2=TOFLambda_monitor() SETTING [TOFLambda_monitor:0]");
  stracpy(_TOFL_afterslow2_var._name, "TOFL_afterslow2", 16384);
  stracpy(_TOFL_afterslow2_var._type, "TOFLambda_monitor", 16384);
  _TOFL_afterslow2_var._index=21;
  int current_setpos_index = 21;
  _TOFL_afterslow2_var._parameters.nowritefile = 0;
  _TOFL_afterslow2_var._parameters.nL = 200;
  _TOFL_afterslow2_var._parameters.nt = 200;
  _TOFL_afterslow2_var._parameters.tmin = 0;
  _TOFL_afterslow2_var._parameters.tmax = 3.0e5;
  if("TOFL_afterslow2.dat" && strlen("TOFL_afterslow2.dat"))
    stracpy(_TOFL_afterslow2_var._parameters.filename, "TOFL_afterslow2.dat" ? "TOFL_afterslow2.dat" : "", 16384);
  else 
  _TOFL_afterslow2_var._parameters.filename[0]='\0';
  _TOFL_afterslow2_var._parameters.xmin = -0.05;
  _TOFL_afterslow2_var._parameters.xmax = 0.05;
  _TOFL_afterslow2_var._parameters.ymin = -0.05;
  _TOFL_afterslow2_var._parameters.ymax = 0.05;
  _TOFL_afterslow2_var._parameters.xwidth = 0.05;
  _TOFL_afterslow2_var._parameters.yheight = 0.21;
  _TOFL_afterslow2_var._parameters.Lmin = 0;
  _TOFL_afterslow2_var._parameters.Lmax = _instrument_var._parameters.Lmax + 1;
  _TOFL_afterslow2_var._parameters.restore_neutron = 0;


  /* component TOFL_afterslow2=TOFLambda_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Lmon_afterslow2_var._rotation_absolute, _TOFL_afterslow2_var._rotation_absolute);
    rot_transpose(_Lmon_afterslow2_var._rotation_absolute, tr1);
    rot_mul(_TOFL_afterslow2_var._rotation_absolute, tr1, _TOFL_afterslow2_var._rotation_relative);
    _TOFL_afterslow2_var._rotation_is_identity =  rot_test_identity(_TOFL_afterslow2_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1e-6);
    rot_transpose(_Lmon_afterslow2_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _TOFL_afterslow2_var._position_absolute = coords_add(_Lmon_afterslow2_var._position_absolute, tc2);
    tc1 = coords_sub(_Lmon_afterslow2_var._position_absolute, _TOFL_afterslow2_var._position_absolute);
    _TOFL_afterslow2_var._position_relative = rot_apply(_TOFL_afterslow2_var._rotation_absolute, tc1);
  } /* TOFL_afterslow2=TOFLambda_monitor() AT ROTATED */
  DEBUG_COMPONENT("TOFL_afterslow2", _TOFL_afterslow2_var._position_absolute, _TOFL_afterslow2_var._rotation_absolute);
  instrument->_position_absolute[21] = _TOFL_afterslow2_var._position_absolute;
  instrument->_position_relative[21] = _TOFL_afterslow2_var._position_relative;
    _TOFL_afterslow2_var._position_relative_is_zero =  coords_test_zero(_TOFL_afterslow2_var._position_relative);
  instrument->counter_N[21]  = instrument->counter_P[21] = instrument->counter_P2[21] = 0;
  instrument->counter_AbsorbProp[21]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0020_TOFL_afterslow2", _TOFL_afterslow2_var._position_absolute, _TOFL_afterslow2_var._rotation_absolute, "TOFLambda_monitor");
        mccomp_param_nexus(nxhandle,"0020_TOFL_afterslow2", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0020_TOFL_afterslow2", "nL", "20", "200","int");
        mccomp_param_nexus(nxhandle,"0020_TOFL_afterslow2", "nt", "128", "200","int");
        mccomp_param_nexus(nxhandle,"0020_TOFL_afterslow2", "tmin", "NONE", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_TOFL_afterslow2", "tmax", "NONE", "3.0e5","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_TOFL_afterslow2", "filename", 0, "TOFL_afterslow2.dat", "char*");
        mccomp_param_nexus(nxhandle,"0020_TOFL_afterslow2", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_TOFL_afterslow2", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_TOFL_afterslow2", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_TOFL_afterslow2", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_TOFL_afterslow2", "xwidth", "0", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_TOFL_afterslow2", "yheight", "0", "0.21","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_TOFL_afterslow2", "Lmin", "NONE", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_TOFL_afterslow2", "Lmax", "NONE", "_instrument_var._parameters.Lmax + 1","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_TOFL_afterslow2", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _TOFL_afterslow2_setpos */

/* component Guidelong2=Guide() SETTING, POSITION/ROTATION */
int _Guidelong2_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Guidelong2_setpos] component Guidelong2=Guide() SETTING [Guide:0]");
  stracpy(_Guidelong2_var._name, "Guidelong2", 16384);
  stracpy(_Guidelong2_var._type, "Guide", 16384);
  _Guidelong2_var._index=22;
  int current_setpos_index = 22;
  _Guidelong2_var._parameters.reflect[0]='\0';
  _Guidelong2_var._parameters.w1 = _instrument_var._parameters.W3;
  _Guidelong2_var._parameters.h1 = _instrument_var._parameters.H3;
  _Guidelong2_var._parameters.w2 = _instrument_var._parameters.W4;
  _Guidelong2_var._parameters.h2 = _instrument_var._parameters.H4;
  _Guidelong2_var._parameters.l = _instrument_var._parameters.Length / 2 -2 * _instrument_var._parameters.GUI_GAP - _instrument_var._parameters.L_ballistic_end;
  _Guidelong2_var._parameters.R0 = 1;
  _Guidelong2_var._parameters.Qc = 0.0219;
  _Guidelong2_var._parameters.alpha = _instrument_var._parameters.ALPHA;
  _Guidelong2_var._parameters.m = _instrument_var._parameters.M;
  _Guidelong2_var._parameters.W = 0.003;


  /* component Guidelong2=Guide() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Fastchop1_var._rotation_absolute, _Guidelong2_var._rotation_absolute);
    rot_transpose(_TOFL_afterslow2_var._rotation_absolute, tr1);
    rot_mul(_Guidelong2_var._rotation_absolute, tr1, _Guidelong2_var._rotation_relative);
    _Guidelong2_var._rotation_is_identity =  rot_test_identity(_Guidelong2_var._rotation_relative);
    tc1 = coords_set(
      0, -0.1, _instrument_var._parameters.GUI_GAP / 2);
    rot_transpose(_Fastchop1_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Guidelong2_var._position_absolute = coords_add(_Fastchop1_var._position_absolute, tc2);
    tc1 = coords_sub(_TOFL_afterslow2_var._position_absolute, _Guidelong2_var._position_absolute);
    _Guidelong2_var._position_relative = rot_apply(_Guidelong2_var._rotation_absolute, tc1);
  } /* Guidelong2=Guide() AT ROTATED */
  DEBUG_COMPONENT("Guidelong2", _Guidelong2_var._position_absolute, _Guidelong2_var._rotation_absolute);
  instrument->_position_absolute[22] = _Guidelong2_var._position_absolute;
  instrument->_position_relative[22] = _Guidelong2_var._position_relative;
    _Guidelong2_var._position_relative_is_zero =  coords_test_zero(_Guidelong2_var._position_relative);
  instrument->counter_N[22]  = instrument->counter_P[22] = instrument->counter_P2[22] = 0;
  instrument->counter_AbsorbProp[22]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0021_Guidelong2", _Guidelong2_var._position_absolute, _Guidelong2_var._rotation_absolute, "Guide");
        mccomp_param_nexus(nxhandle,"0021_Guidelong2", "reflect", 0, 0, "char*");
        mccomp_param_nexus(nxhandle,"0021_Guidelong2", "w1", "NONE", "_instrument_var._parameters.W3","MCNUM");
        mccomp_param_nexus(nxhandle,"0021_Guidelong2", "h1", "NONE", "_instrument_var._parameters.H3","MCNUM");
        mccomp_param_nexus(nxhandle,"0021_Guidelong2", "w2", "0", "_instrument_var._parameters.W4","MCNUM");
        mccomp_param_nexus(nxhandle,"0021_Guidelong2", "h2", "0", "_instrument_var._parameters.H4","MCNUM");
        mccomp_param_nexus(nxhandle,"0021_Guidelong2", "l", "NONE", "_instrument_var._parameters.Length / 2 -2 * _instrument_var._parameters.GUI_GAP - _instrument_var._parameters.L_ballistic_end","MCNUM");
        mccomp_param_nexus(nxhandle,"0021_Guidelong2", "R0", "0.99", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0021_Guidelong2", "Qc", "0.0219", "0.0219","MCNUM");
        mccomp_param_nexus(nxhandle,"0021_Guidelong2", "alpha", "6.07", "_instrument_var._parameters.ALPHA","MCNUM");
        mccomp_param_nexus(nxhandle,"0021_Guidelong2", "m", "2", "_instrument_var._parameters.M","MCNUM");
        mccomp_param_nexus(nxhandle,"0021_Guidelong2", "W", "0.003", "0.003","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Guidelong2_setpos */

/* component Lmon_beforeballistic=L_monitor() SETTING, POSITION/ROTATION */
int _Lmon_beforeballistic_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Lmon_beforeballistic_setpos] component Lmon_beforeballistic=L_monitor() SETTING [L_monitor:0]");
  stracpy(_Lmon_beforeballistic_var._name, "Lmon_beforeballistic", 16384);
  stracpy(_Lmon_beforeballistic_var._type, "L_monitor", 16384);
  _Lmon_beforeballistic_var._index=23;
  int current_setpos_index = 23;
  _Lmon_beforeballistic_var._parameters.nL = 200;
  if("Lmon_before_ballistic.dat" && strlen("Lmon_before_ballistic.dat"))
    stracpy(_Lmon_beforeballistic_var._parameters.filename, "Lmon_before_ballistic.dat" ? "Lmon_before_ballistic.dat" : "", 16384);
  else 
  _Lmon_beforeballistic_var._parameters.filename[0]='\0';
  _Lmon_beforeballistic_var._parameters.nowritefile = 0;
  _Lmon_beforeballistic_var._parameters.xmin = -0.05;
  _Lmon_beforeballistic_var._parameters.xmax = 0.05;
  _Lmon_beforeballistic_var._parameters.ymin = -0.05;
  _Lmon_beforeballistic_var._parameters.ymax = 0.05;
  _Lmon_beforeballistic_var._parameters.xwidth = 0.06;
  _Lmon_beforeballistic_var._parameters.yheight = 0.18;
  _Lmon_beforeballistic_var._parameters.Lmin = 0;
  _Lmon_beforeballistic_var._parameters.Lmax = _instrument_var._parameters.Lmax + 1;
  _Lmon_beforeballistic_var._parameters.restore_neutron = 0;


  /* component Lmon_beforeballistic=L_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Guidelong2_var._rotation_absolute, _Lmon_beforeballistic_var._rotation_absolute);
    rot_transpose(_Guidelong2_var._rotation_absolute, tr1);
    rot_mul(_Lmon_beforeballistic_var._rotation_absolute, tr1, _Lmon_beforeballistic_var._rotation_relative);
    _Lmon_beforeballistic_var._rotation_is_identity =  rot_test_identity(_Lmon_beforeballistic_var._rotation_relative);
    tc1 = coords_set(
      0, 0, _instrument_var._parameters.Length / 2 -2 * _instrument_var._parameters.GUI_GAP - _instrument_var._parameters.L_ballistic_end + 1e-6);
    rot_transpose(_Guidelong2_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Lmon_beforeballistic_var._position_absolute = coords_add(_Guidelong2_var._position_absolute, tc2);
    tc1 = coords_sub(_Guidelong2_var._position_absolute, _Lmon_beforeballistic_var._position_absolute);
    _Lmon_beforeballistic_var._position_relative = rot_apply(_Lmon_beforeballistic_var._rotation_absolute, tc1);
  } /* Lmon_beforeballistic=L_monitor() AT ROTATED */
  DEBUG_COMPONENT("Lmon_beforeballistic", _Lmon_beforeballistic_var._position_absolute, _Lmon_beforeballistic_var._rotation_absolute);
  instrument->_position_absolute[23] = _Lmon_beforeballistic_var._position_absolute;
  instrument->_position_relative[23] = _Lmon_beforeballistic_var._position_relative;
    _Lmon_beforeballistic_var._position_relative_is_zero =  coords_test_zero(_Lmon_beforeballistic_var._position_relative);
  instrument->counter_N[23]  = instrument->counter_P[23] = instrument->counter_P2[23] = 0;
  instrument->counter_AbsorbProp[23]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0022_Lmon_beforeballistic", _Lmon_beforeballistic_var._position_absolute, _Lmon_beforeballistic_var._rotation_absolute, "L_monitor");
        mccomp_param_nexus(nxhandle,"0022_Lmon_beforeballistic", "nL", "20", "200","int");
        mccomp_param_nexus(nxhandle,"0022_Lmon_beforeballistic", "filename", 0, "Lmon_before_ballistic.dat", "char*");
        mccomp_param_nexus(nxhandle,"0022_Lmon_beforeballistic", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0022_Lmon_beforeballistic", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0022_Lmon_beforeballistic", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0022_Lmon_beforeballistic", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0022_Lmon_beforeballistic", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0022_Lmon_beforeballistic", "xwidth", "0", "0.06","MCNUM");
        mccomp_param_nexus(nxhandle,"0022_Lmon_beforeballistic", "yheight", "0", "0.18","MCNUM");
        mccomp_param_nexus(nxhandle,"0022_Lmon_beforeballistic", "Lmin", "NONE", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0022_Lmon_beforeballistic", "Lmax", "NONE", "_instrument_var._parameters.Lmax + 1","MCNUM");
        mccomp_param_nexus(nxhandle,"0022_Lmon_beforeballistic", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Lmon_beforeballistic_setpos */

/* component PSD_beforeballistic=PSD_monitor() SETTING, POSITION/ROTATION */
int _PSD_beforeballistic_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_PSD_beforeballistic_setpos] component PSD_beforeballistic=PSD_monitor() SETTING [PSD_monitor:0]");
  stracpy(_PSD_beforeballistic_var._name, "PSD_beforeballistic", 16384);
  stracpy(_PSD_beforeballistic_var._type, "PSD_monitor", 16384);
  _PSD_beforeballistic_var._index=24;
  int current_setpos_index = 24;
  _PSD_beforeballistic_var._parameters.nx = 90;
  _PSD_beforeballistic_var._parameters.ny = 90;
  if("PSD_beforeballistic.dat" && strlen("PSD_beforeballistic.dat"))
    stracpy(_PSD_beforeballistic_var._parameters.filename, "PSD_beforeballistic.dat" ? "PSD_beforeballistic.dat" : "", 16384);
  else 
  _PSD_beforeballistic_var._parameters.filename[0]='\0';
  _PSD_beforeballistic_var._parameters.xmin = -0.05;
  _PSD_beforeballistic_var._parameters.xmax = 0.05;
  _PSD_beforeballistic_var._parameters.ymin = -0.05;
  _PSD_beforeballistic_var._parameters.ymax = 0.05;
  _PSD_beforeballistic_var._parameters.xwidth = 0.1;
  _PSD_beforeballistic_var._parameters.yheight = 0.25;
  _PSD_beforeballistic_var._parameters.restore_neutron = 0;
  _PSD_beforeballistic_var._parameters.nowritefile = 0;


  /* component PSD_beforeballistic=PSD_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Lmon_beforeballistic_var._rotation_absolute, _PSD_beforeballistic_var._rotation_absolute);
    rot_transpose(_Lmon_beforeballistic_var._rotation_absolute, tr1);
    rot_mul(_PSD_beforeballistic_var._rotation_absolute, tr1, _PSD_beforeballistic_var._rotation_relative);
    _PSD_beforeballistic_var._rotation_is_identity =  rot_test_identity(_PSD_beforeballistic_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1e-6);
    rot_transpose(_Lmon_beforeballistic_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _PSD_beforeballistic_var._position_absolute = coords_add(_Lmon_beforeballistic_var._position_absolute, tc2);
    tc1 = coords_sub(_Lmon_beforeballistic_var._position_absolute, _PSD_beforeballistic_var._position_absolute);
    _PSD_beforeballistic_var._position_relative = rot_apply(_PSD_beforeballistic_var._rotation_absolute, tc1);
  } /* PSD_beforeballistic=PSD_monitor() AT ROTATED */
  DEBUG_COMPONENT("PSD_beforeballistic", _PSD_beforeballistic_var._position_absolute, _PSD_beforeballistic_var._rotation_absolute);
  instrument->_position_absolute[24] = _PSD_beforeballistic_var._position_absolute;
  instrument->_position_relative[24] = _PSD_beforeballistic_var._position_relative;
    _PSD_beforeballistic_var._position_relative_is_zero =  coords_test_zero(_PSD_beforeballistic_var._position_relative);
  instrument->counter_N[24]  = instrument->counter_P[24] = instrument->counter_P2[24] = 0;
  instrument->counter_AbsorbProp[24]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0023_PSD_beforeballistic", _PSD_beforeballistic_var._position_absolute, _PSD_beforeballistic_var._rotation_absolute, "PSD_monitor");
        mccomp_param_nexus(nxhandle,"0023_PSD_beforeballistic", "nx", "90", "90","int");
        mccomp_param_nexus(nxhandle,"0023_PSD_beforeballistic", "ny", "90", "90","int");
        mccomp_param_nexus(nxhandle,"0023_PSD_beforeballistic", "filename", 0, "PSD_beforeballistic.dat", "char*");
        mccomp_param_nexus(nxhandle,"0023_PSD_beforeballistic", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0023_PSD_beforeballistic", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0023_PSD_beforeballistic", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0023_PSD_beforeballistic", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0023_PSD_beforeballistic", "xwidth", "0", "0.1","MCNUM");
        mccomp_param_nexus(nxhandle,"0023_PSD_beforeballistic", "yheight", "0", "0.25","MCNUM");
        mccomp_param_nexus(nxhandle,"0023_PSD_beforeballistic", "restore_neutron", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0023_PSD_beforeballistic", "nowritefile", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _PSD_beforeballistic_setpos */

/* component Guidelong2a=Guide() SETTING, POSITION/ROTATION */
int _Guidelong2a_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Guidelong2a_setpos] component Guidelong2a=Guide() SETTING [Guide:0]");
  stracpy(_Guidelong2a_var._name, "Guidelong2a", 16384);
  stracpy(_Guidelong2a_var._type, "Guide", 16384);
  _Guidelong2a_var._index=25;
  int current_setpos_index = 25;
  _Guidelong2a_var._parameters.reflect[0]='\0';
  _Guidelong2a_var._parameters.w1 = _instrument_var._parameters.W4;
  _Guidelong2a_var._parameters.h1 = _instrument_var._parameters.H4;
  _Guidelong2a_var._parameters.w2 = _instrument_var._parameters.W_chop;
  _Guidelong2a_var._parameters.h2 = _instrument_var._parameters.H_chop;
  _Guidelong2a_var._parameters.l = _instrument_var._parameters.L_ballistic_end;
  _Guidelong2a_var._parameters.R0 = 1;
  _Guidelong2a_var._parameters.Qc = 0.0219;
  _Guidelong2a_var._parameters.alpha = _instrument_var._parameters.ALPHA;
  _Guidelong2a_var._parameters.m = _instrument_var._parameters.M;
  _Guidelong2a_var._parameters.W = 0.003;


  /* component Guidelong2a=Guide() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _PSD_beforeballistic_var._rotation_absolute, _Guidelong2a_var._rotation_absolute);
    rot_transpose(_PSD_beforeballistic_var._rotation_absolute, tr1);
    rot_mul(_Guidelong2a_var._rotation_absolute, tr1, _Guidelong2a_var._rotation_relative);
    _Guidelong2a_var._rotation_is_identity =  rot_test_identity(_Guidelong2a_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1e-6);
    rot_transpose(_PSD_beforeballistic_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Guidelong2a_var._position_absolute = coords_add(_PSD_beforeballistic_var._position_absolute, tc2);
    tc1 = coords_sub(_PSD_beforeballistic_var._position_absolute, _Guidelong2a_var._position_absolute);
    _Guidelong2a_var._position_relative = rot_apply(_Guidelong2a_var._rotation_absolute, tc1);
  } /* Guidelong2a=Guide() AT ROTATED */
  DEBUG_COMPONENT("Guidelong2a", _Guidelong2a_var._position_absolute, _Guidelong2a_var._rotation_absolute);
  instrument->_position_absolute[25] = _Guidelong2a_var._position_absolute;
  instrument->_position_relative[25] = _Guidelong2a_var._position_relative;
    _Guidelong2a_var._position_relative_is_zero =  coords_test_zero(_Guidelong2a_var._position_relative);
  instrument->counter_N[25]  = instrument->counter_P[25] = instrument->counter_P2[25] = 0;
  instrument->counter_AbsorbProp[25]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0024_Guidelong2a", _Guidelong2a_var._position_absolute, _Guidelong2a_var._rotation_absolute, "Guide");
        mccomp_param_nexus(nxhandle,"0024_Guidelong2a", "reflect", 0, 0, "char*");
        mccomp_param_nexus(nxhandle,"0024_Guidelong2a", "w1", "NONE", "_instrument_var._parameters.W4","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Guidelong2a", "h1", "NONE", "_instrument_var._parameters.H4","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Guidelong2a", "w2", "0", "_instrument_var._parameters.W_chop","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Guidelong2a", "h2", "0", "_instrument_var._parameters.H_chop","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Guidelong2a", "l", "NONE", "_instrument_var._parameters.L_ballistic_end","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Guidelong2a", "R0", "0.99", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Guidelong2a", "Qc", "0.0219", "0.0219","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Guidelong2a", "alpha", "6.07", "_instrument_var._parameters.ALPHA","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Guidelong2a", "m", "2", "_instrument_var._parameters.M","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Guidelong2a", "W", "0.003", "0.003","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Guidelong2a_setpos */

/* component Lmonfast2=L_monitor() SETTING, POSITION/ROTATION */
int _Lmonfast2_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Lmonfast2_setpos] component Lmonfast2=L_monitor() SETTING [L_monitor:0]");
  stracpy(_Lmonfast2_var._name, "Lmonfast2", 16384);
  stracpy(_Lmonfast2_var._type, "L_monitor", 16384);
  _Lmonfast2_var._index=26;
  int current_setpos_index = 26;
  _Lmonfast2_var._parameters.nL = 200;
  if("Lmonfast2.dat" && strlen("Lmonfast2.dat"))
    stracpy(_Lmonfast2_var._parameters.filename, "Lmonfast2.dat" ? "Lmonfast2.dat" : "", 16384);
  else 
  _Lmonfast2_var._parameters.filename[0]='\0';
  _Lmonfast2_var._parameters.nowritefile = 0;
  _Lmonfast2_var._parameters.xmin = -0.05;
  _Lmonfast2_var._parameters.xmax = 0.05;
  _Lmonfast2_var._parameters.ymin = -0.05;
  _Lmonfast2_var._parameters.ymax = 0.05;
  _Lmonfast2_var._parameters.xwidth = 0.06;
  _Lmonfast2_var._parameters.yheight = 0.18;
  _Lmonfast2_var._parameters.Lmin = 0;
  _Lmonfast2_var._parameters.Lmax = _instrument_var._parameters.Lmax + 1;
  _Lmonfast2_var._parameters.restore_neutron = 0;


  /* component Lmonfast2=L_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Guidelong2a_var._rotation_absolute, _Lmonfast2_var._rotation_absolute);
    rot_transpose(_Guidelong2a_var._rotation_absolute, tr1);
    rot_mul(_Lmonfast2_var._rotation_absolute, tr1, _Lmonfast2_var._rotation_relative);
    _Lmonfast2_var._rotation_is_identity =  rot_test_identity(_Lmonfast2_var._rotation_relative);
    tc1 = coords_set(
      0, 0, _instrument_var._parameters.L_ballistic_end + 1e-6);
    rot_transpose(_Guidelong2a_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Lmonfast2_var._position_absolute = coords_add(_Guidelong2a_var._position_absolute, tc2);
    tc1 = coords_sub(_Guidelong2a_var._position_absolute, _Lmonfast2_var._position_absolute);
    _Lmonfast2_var._position_relative = rot_apply(_Lmonfast2_var._rotation_absolute, tc1);
  } /* Lmonfast2=L_monitor() AT ROTATED */
  DEBUG_COMPONENT("Lmonfast2", _Lmonfast2_var._position_absolute, _Lmonfast2_var._rotation_absolute);
  instrument->_position_absolute[26] = _Lmonfast2_var._position_absolute;
  instrument->_position_relative[26] = _Lmonfast2_var._position_relative;
    _Lmonfast2_var._position_relative_is_zero =  coords_test_zero(_Lmonfast2_var._position_relative);
  instrument->counter_N[26]  = instrument->counter_P[26] = instrument->counter_P2[26] = 0;
  instrument->counter_AbsorbProp[26]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0025_Lmonfast2", _Lmonfast2_var._position_absolute, _Lmonfast2_var._rotation_absolute, "L_monitor");
        mccomp_param_nexus(nxhandle,"0025_Lmonfast2", "nL", "20", "200","int");
        mccomp_param_nexus(nxhandle,"0025_Lmonfast2", "filename", 0, "Lmonfast2.dat", "char*");
        mccomp_param_nexus(nxhandle,"0025_Lmonfast2", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0025_Lmonfast2", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0025_Lmonfast2", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0025_Lmonfast2", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0025_Lmonfast2", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0025_Lmonfast2", "xwidth", "0", "0.06","MCNUM");
        mccomp_param_nexus(nxhandle,"0025_Lmonfast2", "yheight", "0", "0.18","MCNUM");
        mccomp_param_nexus(nxhandle,"0025_Lmonfast2", "Lmin", "NONE", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0025_Lmonfast2", "Lmax", "NONE", "_instrument_var._parameters.Lmax + 1","MCNUM");
        mccomp_param_nexus(nxhandle,"0025_Lmonfast2", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Lmonfast2_setpos */

/* component Lmonfast2_zoom=L_monitor() SETTING, POSITION/ROTATION */
int _Lmonfast2_zoom_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Lmonfast2_zoom_setpos] component Lmonfast2_zoom=L_monitor() SETTING [L_monitor:0]");
  stracpy(_Lmonfast2_zoom_var._name, "Lmonfast2_zoom", 16384);
  stracpy(_Lmonfast2_zoom_var._type, "L_monitor", 16384);
  _Lmonfast2_zoom_var._index=27;
  int current_setpos_index = 27;
  _Lmonfast2_zoom_var._parameters.nL = 200;
  if("Lmonfast2_zoom.dat" && strlen("Lmonfast2_zoom.dat"))
    stracpy(_Lmonfast2_zoom_var._parameters.filename, "Lmonfast2_zoom.dat" ? "Lmonfast2_zoom.dat" : "", 16384);
  else 
  _Lmonfast2_zoom_var._parameters.filename[0]='\0';
  _Lmonfast2_zoom_var._parameters.nowritefile = 0;
  _Lmonfast2_zoom_var._parameters.xmin = -0.05;
  _Lmonfast2_zoom_var._parameters.xmax = 0.05;
  _Lmonfast2_zoom_var._parameters.ymin = -0.05;
  _Lmonfast2_zoom_var._parameters.ymax = 0.05;
  _Lmonfast2_zoom_var._parameters.xwidth = 0.06;
  _Lmonfast2_zoom_var._parameters.yheight = 0.18;
  _Lmonfast2_zoom_var._parameters.Lmin = _instrument_var._parameters.lambda0 -0.2;
  _Lmonfast2_zoom_var._parameters.Lmax = _instrument_var._parameters.lambda0 + 0.2;
  _Lmonfast2_zoom_var._parameters.restore_neutron = 0;


  /* component Lmonfast2_zoom=L_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Lmonfast2_var._rotation_absolute, _Lmonfast2_zoom_var._rotation_absolute);
    rot_transpose(_Lmonfast2_var._rotation_absolute, tr1);
    rot_mul(_Lmonfast2_zoom_var._rotation_absolute, tr1, _Lmonfast2_zoom_var._rotation_relative);
    _Lmonfast2_zoom_var._rotation_is_identity =  rot_test_identity(_Lmonfast2_zoom_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1e-6);
    rot_transpose(_Lmonfast2_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Lmonfast2_zoom_var._position_absolute = coords_add(_Lmonfast2_var._position_absolute, tc2);
    tc1 = coords_sub(_Lmonfast2_var._position_absolute, _Lmonfast2_zoom_var._position_absolute);
    _Lmonfast2_zoom_var._position_relative = rot_apply(_Lmonfast2_zoom_var._rotation_absolute, tc1);
  } /* Lmonfast2_zoom=L_monitor() AT ROTATED */
  DEBUG_COMPONENT("Lmonfast2_zoom", _Lmonfast2_zoom_var._position_absolute, _Lmonfast2_zoom_var._rotation_absolute);
  instrument->_position_absolute[27] = _Lmonfast2_zoom_var._position_absolute;
  instrument->_position_relative[27] = _Lmonfast2_zoom_var._position_relative;
    _Lmonfast2_zoom_var._position_relative_is_zero =  coords_test_zero(_Lmonfast2_zoom_var._position_relative);
  instrument->counter_N[27]  = instrument->counter_P[27] = instrument->counter_P2[27] = 0;
  instrument->counter_AbsorbProp[27]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0026_Lmonfast2_zoom", _Lmonfast2_zoom_var._position_absolute, _Lmonfast2_zoom_var._rotation_absolute, "L_monitor");
        mccomp_param_nexus(nxhandle,"0026_Lmonfast2_zoom", "nL", "20", "200","int");
        mccomp_param_nexus(nxhandle,"0026_Lmonfast2_zoom", "filename", 0, "Lmonfast2_zoom.dat", "char*");
        mccomp_param_nexus(nxhandle,"0026_Lmonfast2_zoom", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0026_Lmonfast2_zoom", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0026_Lmonfast2_zoom", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0026_Lmonfast2_zoom", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0026_Lmonfast2_zoom", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0026_Lmonfast2_zoom", "xwidth", "0", "0.06","MCNUM");
        mccomp_param_nexus(nxhandle,"0026_Lmonfast2_zoom", "yheight", "0", "0.18","MCNUM");
        mccomp_param_nexus(nxhandle,"0026_Lmonfast2_zoom", "Lmin", "NONE", "_instrument_var._parameters.lambda0 -0.2","MCNUM");
        mccomp_param_nexus(nxhandle,"0026_Lmonfast2_zoom", "Lmax", "NONE", "_instrument_var._parameters.lambda0 + 0.2","MCNUM");
        mccomp_param_nexus(nxhandle,"0026_Lmonfast2_zoom", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Lmonfast2_zoom_setpos */

/* component TOFLfast2=TOFLambda_monitor() SETTING, POSITION/ROTATION */
int _TOFLfast2_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_TOFLfast2_setpos] component TOFLfast2=TOFLambda_monitor() SETTING [TOFLambda_monitor:0]");
  stracpy(_TOFLfast2_var._name, "TOFLfast2", 16384);
  stracpy(_TOFLfast2_var._type, "TOFLambda_monitor", 16384);
  _TOFLfast2_var._index=28;
  int current_setpos_index = 28;
  _TOFLfast2_var._parameters.nowritefile = 0;
  _TOFLfast2_var._parameters.nL = 200;
  _TOFLfast2_var._parameters.nt = 200;
  _TOFLfast2_var._parameters.tmin = 0;
  _TOFLfast2_var._parameters.tmax = 3.0e5;
  if("TOFLfast2.dat" && strlen("TOFLfast2.dat"))
    stracpy(_TOFLfast2_var._parameters.filename, "TOFLfast2.dat" ? "TOFLfast2.dat" : "", 16384);
  else 
  _TOFLfast2_var._parameters.filename[0]='\0';
  _TOFLfast2_var._parameters.xmin = -0.05;
  _TOFLfast2_var._parameters.xmax = 0.05;
  _TOFLfast2_var._parameters.ymin = -0.05;
  _TOFLfast2_var._parameters.ymax = 0.05;
  _TOFLfast2_var._parameters.xwidth = 0.05;
  _TOFLfast2_var._parameters.yheight = 0.12;
  _TOFLfast2_var._parameters.Lmin = 0;
  _TOFLfast2_var._parameters.Lmax = _instrument_var._parameters.Lmax + 1;
  _TOFLfast2_var._parameters.restore_neutron = 0;


  /* component TOFLfast2=TOFLambda_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Lmonfast2_zoom_var._rotation_absolute, _TOFLfast2_var._rotation_absolute);
    rot_transpose(_Lmonfast2_zoom_var._rotation_absolute, tr1);
    rot_mul(_TOFLfast2_var._rotation_absolute, tr1, _TOFLfast2_var._rotation_relative);
    _TOFLfast2_var._rotation_is_identity =  rot_test_identity(_TOFLfast2_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1e-6);
    rot_transpose(_Lmonfast2_zoom_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _TOFLfast2_var._position_absolute = coords_add(_Lmonfast2_zoom_var._position_absolute, tc2);
    tc1 = coords_sub(_Lmonfast2_zoom_var._position_absolute, _TOFLfast2_var._position_absolute);
    _TOFLfast2_var._position_relative = rot_apply(_TOFLfast2_var._rotation_absolute, tc1);
  } /* TOFLfast2=TOFLambda_monitor() AT ROTATED */
  DEBUG_COMPONENT("TOFLfast2", _TOFLfast2_var._position_absolute, _TOFLfast2_var._rotation_absolute);
  instrument->_position_absolute[28] = _TOFLfast2_var._position_absolute;
  instrument->_position_relative[28] = _TOFLfast2_var._position_relative;
    _TOFLfast2_var._position_relative_is_zero =  coords_test_zero(_TOFLfast2_var._position_relative);
  instrument->counter_N[28]  = instrument->counter_P[28] = instrument->counter_P2[28] = 0;
  instrument->counter_AbsorbProp[28]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0027_TOFLfast2", _TOFLfast2_var._position_absolute, _TOFLfast2_var._rotation_absolute, "TOFLambda_monitor");
        mccomp_param_nexus(nxhandle,"0027_TOFLfast2", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0027_TOFLfast2", "nL", "20", "200","int");
        mccomp_param_nexus(nxhandle,"0027_TOFLfast2", "nt", "128", "200","int");
        mccomp_param_nexus(nxhandle,"0027_TOFLfast2", "tmin", "NONE", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0027_TOFLfast2", "tmax", "NONE", "3.0e5","MCNUM");
        mccomp_param_nexus(nxhandle,"0027_TOFLfast2", "filename", 0, "TOFLfast2.dat", "char*");
        mccomp_param_nexus(nxhandle,"0027_TOFLfast2", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0027_TOFLfast2", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0027_TOFLfast2", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0027_TOFLfast2", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0027_TOFLfast2", "xwidth", "0", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0027_TOFLfast2", "yheight", "0", "0.12","MCNUM");
        mccomp_param_nexus(nxhandle,"0027_TOFLfast2", "Lmin", "NONE", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0027_TOFLfast2", "Lmax", "NONE", "_instrument_var._parameters.Lmax + 1","MCNUM");
        mccomp_param_nexus(nxhandle,"0027_TOFLfast2", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _TOFLfast2_setpos */

/* component TOFLfast2zoom=TOFLambda_monitor() SETTING, POSITION/ROTATION */
int _TOFLfast2zoom_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_TOFLfast2zoom_setpos] component TOFLfast2zoom=TOFLambda_monitor() SETTING [TOFLambda_monitor:0]");
  stracpy(_TOFLfast2zoom_var._name, "TOFLfast2zoom", 16384);
  stracpy(_TOFLfast2zoom_var._type, "TOFLambda_monitor", 16384);
  _TOFLfast2zoom_var._index=29;
  int current_setpos_index = 29;
  _TOFLfast2zoom_var._parameters.nowritefile = 0;
  _TOFLfast2zoom_var._parameters.nL = 200;
  _TOFLfast2zoom_var._parameters.nt = 200;
  _TOFLfast2zoom_var._parameters.tmin = tmin_zoom;
  _TOFLfast2zoom_var._parameters.tmax = tmax_zoom;
  if("TOFLfast2_zoom.dat" && strlen("TOFLfast2_zoom.dat"))
    stracpy(_TOFLfast2zoom_var._parameters.filename, "TOFLfast2_zoom.dat" ? "TOFLfast2_zoom.dat" : "", 16384);
  else 
  _TOFLfast2zoom_var._parameters.filename[0]='\0';
  _TOFLfast2zoom_var._parameters.xmin = -0.05;
  _TOFLfast2zoom_var._parameters.xmax = 0.05;
  _TOFLfast2zoom_var._parameters.ymin = -0.05;
  _TOFLfast2zoom_var._parameters.ymax = 0.05;
  _TOFLfast2zoom_var._parameters.xwidth = 0.05;
  _TOFLfast2zoom_var._parameters.yheight = 0.12;
  _TOFLfast2zoom_var._parameters.Lmin = _instrument_var._parameters.lambda0 -0.2;
  _TOFLfast2zoom_var._parameters.Lmax = _instrument_var._parameters.lambda0 + 0.2;
  _TOFLfast2zoom_var._parameters.restore_neutron = 0;


  /* component TOFLfast2zoom=TOFLambda_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _TOFLfast2_var._rotation_absolute, _TOFLfast2zoom_var._rotation_absolute);
    rot_transpose(_TOFLfast2_var._rotation_absolute, tr1);
    rot_mul(_TOFLfast2zoom_var._rotation_absolute, tr1, _TOFLfast2zoom_var._rotation_relative);
    _TOFLfast2zoom_var._rotation_is_identity =  rot_test_identity(_TOFLfast2zoom_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1e-6);
    rot_transpose(_TOFLfast2_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _TOFLfast2zoom_var._position_absolute = coords_add(_TOFLfast2_var._position_absolute, tc2);
    tc1 = coords_sub(_TOFLfast2_var._position_absolute, _TOFLfast2zoom_var._position_absolute);
    _TOFLfast2zoom_var._position_relative = rot_apply(_TOFLfast2zoom_var._rotation_absolute, tc1);
  } /* TOFLfast2zoom=TOFLambda_monitor() AT ROTATED */
  DEBUG_COMPONENT("TOFLfast2zoom", _TOFLfast2zoom_var._position_absolute, _TOFLfast2zoom_var._rotation_absolute);
  instrument->_position_absolute[29] = _TOFLfast2zoom_var._position_absolute;
  instrument->_position_relative[29] = _TOFLfast2zoom_var._position_relative;
    _TOFLfast2zoom_var._position_relative_is_zero =  coords_test_zero(_TOFLfast2zoom_var._position_relative);
  instrument->counter_N[29]  = instrument->counter_P[29] = instrument->counter_P2[29] = 0;
  instrument->counter_AbsorbProp[29]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0028_TOFLfast2zoom", _TOFLfast2zoom_var._position_absolute, _TOFLfast2zoom_var._rotation_absolute, "TOFLambda_monitor");
        mccomp_param_nexus(nxhandle,"0028_TOFLfast2zoom", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0028_TOFLfast2zoom", "nL", "20", "200","int");
        mccomp_param_nexus(nxhandle,"0028_TOFLfast2zoom", "nt", "128", "200","int");
        mccomp_param_nexus(nxhandle,"0028_TOFLfast2zoom", "tmin", "NONE", "tmin_zoom","MCNUM");
        mccomp_param_nexus(nxhandle,"0028_TOFLfast2zoom", "tmax", "NONE", "tmax_zoom","MCNUM");
        mccomp_param_nexus(nxhandle,"0028_TOFLfast2zoom", "filename", 0, "TOFLfast2_zoom.dat", "char*");
        mccomp_param_nexus(nxhandle,"0028_TOFLfast2zoom", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0028_TOFLfast2zoom", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0028_TOFLfast2zoom", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0028_TOFLfast2zoom", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0028_TOFLfast2zoom", "xwidth", "0", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0028_TOFLfast2zoom", "yheight", "0", "0.12","MCNUM");
        mccomp_param_nexus(nxhandle,"0028_TOFLfast2zoom", "Lmin", "NONE", "_instrument_var._parameters.lambda0 -0.2","MCNUM");
        mccomp_param_nexus(nxhandle,"0028_TOFLfast2zoom", "Lmax", "NONE", "_instrument_var._parameters.lambda0 + 0.2","MCNUM");
        mccomp_param_nexus(nxhandle,"0028_TOFLfast2zoom", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _TOFLfast2zoom_setpos */

/* component PSDfast2=PSD_monitor() SETTING, POSITION/ROTATION */
int _PSDfast2_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_PSDfast2_setpos] component PSDfast2=PSD_monitor() SETTING [PSD_monitor:0]");
  stracpy(_PSDfast2_var._name, "PSDfast2", 16384);
  stracpy(_PSDfast2_var._type, "PSD_monitor", 16384);
  _PSDfast2_var._index=30;
  int current_setpos_index = 30;
  _PSDfast2_var._parameters.nx = 90;
  _PSDfast2_var._parameters.ny = 90;
  if("PSDfast2.dat" && strlen("PSDfast2.dat"))
    stracpy(_PSDfast2_var._parameters.filename, "PSDfast2.dat" ? "PSDfast2.dat" : "", 16384);
  else 
  _PSDfast2_var._parameters.filename[0]='\0';
  _PSDfast2_var._parameters.xmin = -0.05;
  _PSDfast2_var._parameters.xmax = 0.05;
  _PSDfast2_var._parameters.ymin = -0.05;
  _PSDfast2_var._parameters.ymax = 0.05;
  _PSDfast2_var._parameters.xwidth = 0.1;
  _PSDfast2_var._parameters.yheight = 0.25;
  _PSDfast2_var._parameters.restore_neutron = 0;
  _PSDfast2_var._parameters.nowritefile = 0;


  /* component PSDfast2=PSD_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _TOFLfast2zoom_var._rotation_absolute, _PSDfast2_var._rotation_absolute);
    rot_transpose(_TOFLfast2zoom_var._rotation_absolute, tr1);
    rot_mul(_PSDfast2_var._rotation_absolute, tr1, _PSDfast2_var._rotation_relative);
    _PSDfast2_var._rotation_is_identity =  rot_test_identity(_PSDfast2_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1e-6);
    rot_transpose(_TOFLfast2zoom_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _PSDfast2_var._position_absolute = coords_add(_TOFLfast2zoom_var._position_absolute, tc2);
    tc1 = coords_sub(_TOFLfast2zoom_var._position_absolute, _PSDfast2_var._position_absolute);
    _PSDfast2_var._position_relative = rot_apply(_PSDfast2_var._rotation_absolute, tc1);
  } /* PSDfast2=PSD_monitor() AT ROTATED */
  DEBUG_COMPONENT("PSDfast2", _PSDfast2_var._position_absolute, _PSDfast2_var._rotation_absolute);
  instrument->_position_absolute[30] = _PSDfast2_var._position_absolute;
  instrument->_position_relative[30] = _PSDfast2_var._position_relative;
    _PSDfast2_var._position_relative_is_zero =  coords_test_zero(_PSDfast2_var._position_relative);
  instrument->counter_N[30]  = instrument->counter_P[30] = instrument->counter_P2[30] = 0;
  instrument->counter_AbsorbProp[30]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0029_PSDfast2", _PSDfast2_var._position_absolute, _PSDfast2_var._rotation_absolute, "PSD_monitor");
        mccomp_param_nexus(nxhandle,"0029_PSDfast2", "nx", "90", "90","int");
        mccomp_param_nexus(nxhandle,"0029_PSDfast2", "ny", "90", "90","int");
        mccomp_param_nexus(nxhandle,"0029_PSDfast2", "filename", 0, "PSDfast2.dat", "char*");
        mccomp_param_nexus(nxhandle,"0029_PSDfast2", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0029_PSDfast2", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0029_PSDfast2", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0029_PSDfast2", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0029_PSDfast2", "xwidth", "0", "0.1","MCNUM");
        mccomp_param_nexus(nxhandle,"0029_PSDfast2", "yheight", "0", "0.25","MCNUM");
        mccomp_param_nexus(nxhandle,"0029_PSDfast2", "restore_neutron", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0029_PSDfast2", "nowritefile", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _PSDfast2_setpos */

/* component Fastchop2=DiskChopper() SETTING, POSITION/ROTATION */
int _Fastchop2_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Fastchop2_setpos] component Fastchop2=DiskChopper() SETTING [DiskChopper:0]");
  stracpy(_Fastchop2_var._name, "Fastchop2", 16384);
  stracpy(_Fastchop2_var._type, "DiskChopper", 16384);
  _Fastchop2_var._index=31;
  int current_setpos_index = 31;
  _Fastchop2_var._parameters.theta_0 = _instrument_var._parameters.FAST_THETA;
  _Fastchop2_var._parameters.radius = 0.35;
  _Fastchop2_var._parameters.yheight = 0.35;
  _Fastchop2_var._parameters.nu = _instrument_var._parameters.F_fast2;
  _Fastchop2_var._parameters.nslit = _instrument_var._parameters.N_fast;
  _Fastchop2_var._parameters.jitter = 0;
  _Fastchop2_var._parameters.delay = t_fast2;
  _Fastchop2_var._parameters.isfirst = 0;
  _Fastchop2_var._parameters.n_pulse = 1;
  _Fastchop2_var._parameters.abs_out = 1;
  _Fastchop2_var._parameters.phase = 0;
  _Fastchop2_var._parameters.xwidth = 0;
  _Fastchop2_var._parameters.verbose = 0;


  /* component Fastchop2=DiskChopper() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Origin_var._rotation_absolute, _Fastchop2_var._rotation_absolute);
    rot_transpose(_PSDfast2_var._rotation_absolute, tr1);
    rot_mul(_Fastchop2_var._rotation_absolute, tr1, _Fastchop2_var._rotation_relative);
    _Fastchop2_var._rotation_is_identity =  rot_test_identity(_Fastchop2_var._rotation_relative);
    tc1 = coords_set(
      0, 0.04, _instrument_var._parameters.Length);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Fastchop2_var._position_absolute = coords_add(_Origin_var._position_absolute, tc2);
    tc1 = coords_sub(_PSDfast2_var._position_absolute, _Fastchop2_var._position_absolute);
    _Fastchop2_var._position_relative = rot_apply(_Fastchop2_var._rotation_absolute, tc1);
  } /* Fastchop2=DiskChopper() AT ROTATED */
  DEBUG_COMPONENT("Fastchop2", _Fastchop2_var._position_absolute, _Fastchop2_var._rotation_absolute);
  instrument->_position_absolute[31] = _Fastchop2_var._position_absolute;
  instrument->_position_relative[31] = _Fastchop2_var._position_relative;
    _Fastchop2_var._position_relative_is_zero =  coords_test_zero(_Fastchop2_var._position_relative);
  instrument->counter_N[31]  = instrument->counter_P[31] = instrument->counter_P2[31] = 0;
  instrument->counter_AbsorbProp[31]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0030_Fastchop2", _Fastchop2_var._position_absolute, _Fastchop2_var._rotation_absolute, "DiskChopper");
        mccomp_param_nexus(nxhandle,"0030_Fastchop2", "theta_0", "0", "_instrument_var._parameters.FAST_THETA","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_Fastchop2", "radius", "0.5", "0.35","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_Fastchop2", "yheight", "NONE", "0.35","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_Fastchop2", "nu", "NONE", "_instrument_var._parameters.F_fast2","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_Fastchop2", "nslit", "3", "_instrument_var._parameters.N_fast","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_Fastchop2", "jitter", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_Fastchop2", "delay", "0", "t_fast2","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_Fastchop2", "isfirst", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_Fastchop2", "n_pulse", "1", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_Fastchop2", "abs_out", "1", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_Fastchop2", "phase", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_Fastchop2", "xwidth", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_Fastchop2", "verbose", "0", "0","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Fastchop2_setpos */

/* component Fastchop2counter=DiskChopper() SETTING, POSITION/ROTATION */
int _Fastchop2counter_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Fastchop2counter_setpos] component Fastchop2counter=DiskChopper() SETTING [DiskChopper:0]");
  stracpy(_Fastchop2counter_var._name, "Fastchop2counter", 16384);
  stracpy(_Fastchop2counter_var._type, "DiskChopper", 16384);
  _Fastchop2counter_var._index=32;
  int current_setpos_index = 32;
  _Fastchop2counter_var._parameters.theta_0 = _instrument_var._parameters.FAST_THETA;
  _Fastchop2counter_var._parameters.radius = 0.35;
  _Fastchop2counter_var._parameters.yheight = 0.35;
  _Fastchop2counter_var._parameters.nu = - _instrument_var._parameters.F_fast2;
  _Fastchop2counter_var._parameters.nslit = _instrument_var._parameters.N_fast;
  _Fastchop2counter_var._parameters.jitter = 0;
  _Fastchop2counter_var._parameters.delay = - t_fast2a;
  _Fastchop2counter_var._parameters.isfirst = 0;
  _Fastchop2counter_var._parameters.n_pulse = 1;
  _Fastchop2counter_var._parameters.abs_out = 1;
  _Fastchop2counter_var._parameters.phase = 0;
  _Fastchop2counter_var._parameters.xwidth = 0;
  _Fastchop2counter_var._parameters.verbose = 0;


  /* component Fastchop2counter=DiskChopper() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Origin_var._rotation_absolute, _Fastchop2counter_var._rotation_absolute);
    rot_transpose(_Fastchop2_var._rotation_absolute, tr1);
    rot_mul(_Fastchop2counter_var._rotation_absolute, tr1, _Fastchop2counter_var._rotation_relative);
    _Fastchop2counter_var._rotation_is_identity =  rot_test_identity(_Fastchop2counter_var._rotation_relative);
    tc1 = coords_set(
      0, 0.04, _instrument_var._parameters.Length + _instrument_var._parameters.GUI_GAP);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Fastchop2counter_var._position_absolute = coords_add(_Origin_var._position_absolute, tc2);
    tc1 = coords_sub(_Fastchop2_var._position_absolute, _Fastchop2counter_var._position_absolute);
    _Fastchop2counter_var._position_relative = rot_apply(_Fastchop2counter_var._rotation_absolute, tc1);
  } /* Fastchop2counter=DiskChopper() AT ROTATED */
  DEBUG_COMPONENT("Fastchop2counter", _Fastchop2counter_var._position_absolute, _Fastchop2counter_var._rotation_absolute);
  instrument->_position_absolute[32] = _Fastchop2counter_var._position_absolute;
  instrument->_position_relative[32] = _Fastchop2counter_var._position_relative;
    _Fastchop2counter_var._position_relative_is_zero =  coords_test_zero(_Fastchop2counter_var._position_relative);
  instrument->counter_N[32]  = instrument->counter_P[32] = instrument->counter_P2[32] = 0;
  instrument->counter_AbsorbProp[32]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0031_Fastchop2counter", _Fastchop2counter_var._position_absolute, _Fastchop2counter_var._rotation_absolute, "DiskChopper");
        mccomp_param_nexus(nxhandle,"0031_Fastchop2counter", "theta_0", "0", "_instrument_var._parameters.FAST_THETA","MCNUM");
        mccomp_param_nexus(nxhandle,"0031_Fastchop2counter", "radius", "0.5", "0.35","MCNUM");
        mccomp_param_nexus(nxhandle,"0031_Fastchop2counter", "yheight", "NONE", "0.35","MCNUM");
        mccomp_param_nexus(nxhandle,"0031_Fastchop2counter", "nu", "NONE", "- _instrument_var._parameters.F_fast2","MCNUM");
        mccomp_param_nexus(nxhandle,"0031_Fastchop2counter", "nslit", "3", "_instrument_var._parameters.N_fast","MCNUM");
        mccomp_param_nexus(nxhandle,"0031_Fastchop2counter", "jitter", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0031_Fastchop2counter", "delay", "0", "- t_fast2a","MCNUM");
        mccomp_param_nexus(nxhandle,"0031_Fastchop2counter", "isfirst", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0031_Fastchop2counter", "n_pulse", "1", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0031_Fastchop2counter", "abs_out", "1", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0031_Fastchop2counter", "phase", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0031_Fastchop2counter", "xwidth", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0031_Fastchop2counter", "verbose", "0", "0","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Fastchop2counter_setpos */

/* component FOchop3=DiskChopper() SETTING, POSITION/ROTATION */
int _FOchop3_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_FOchop3_setpos] component FOchop3=DiskChopper() SETTING [DiskChopper:0]");
  stracpy(_FOchop3_var._name, "FOchop3", 16384);
  stracpy(_FOchop3_var._type, "DiskChopper", 16384);
  _FOchop3_var._index=33;
  int current_setpos_index = 33;
  _FOchop3_var._parameters.theta_0 = 2 * _instrument_var._parameters.FAST_THETA;
  _FOchop3_var._parameters.radius = 0.35;
  _FOchop3_var._parameters.yheight = 0.35;
  _FOchop3_var._parameters.nu = _instrument_var._parameters.F_fast2 / _instrument_var._parameters.FO3;
  _FOchop3_var._parameters.nslit = _instrument_var._parameters.N_fast;
  _FOchop3_var._parameters.jitter = 0;
  _FOchop3_var._parameters.delay = t_fast3;
  _FOchop3_var._parameters.isfirst = 0;
  _FOchop3_var._parameters.n_pulse = 1;
  _FOchop3_var._parameters.abs_out = 1;
  _FOchop3_var._parameters.phase = 0;
  _FOchop3_var._parameters.xwidth = 0;
  _FOchop3_var._parameters.verbose = 0;


  /* component FOchop3=DiskChopper() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Origin_var._rotation_absolute, _FOchop3_var._rotation_absolute);
    rot_transpose(_Fastchop2counter_var._rotation_absolute, tr1);
    rot_mul(_FOchop3_var._rotation_absolute, tr1, _FOchop3_var._rotation_relative);
    _FOchop3_var._rotation_is_identity =  rot_test_identity(_FOchop3_var._rotation_relative);
    tc1 = coords_set(
      0, 0.04, _instrument_var._parameters.Length + 2 * _instrument_var._parameters.GUI_GAP);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _FOchop3_var._position_absolute = coords_add(_Origin_var._position_absolute, tc2);
    tc1 = coords_sub(_Fastchop2counter_var._position_absolute, _FOchop3_var._position_absolute);
    _FOchop3_var._position_relative = rot_apply(_FOchop3_var._rotation_absolute, tc1);
  } /* FOchop3=DiskChopper() AT ROTATED */
  DEBUG_COMPONENT("FOchop3", _FOchop3_var._position_absolute, _FOchop3_var._rotation_absolute);
  instrument->_position_absolute[33] = _FOchop3_var._position_absolute;
  instrument->_position_relative[33] = _FOchop3_var._position_relative;
    _FOchop3_var._position_relative_is_zero =  coords_test_zero(_FOchop3_var._position_relative);
  instrument->counter_N[33]  = instrument->counter_P[33] = instrument->counter_P2[33] = 0;
  instrument->counter_AbsorbProp[33]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0032_FOchop3", _FOchop3_var._position_absolute, _FOchop3_var._rotation_absolute, "DiskChopper");
        mccomp_param_nexus(nxhandle,"0032_FOchop3", "theta_0", "0", "2 * _instrument_var._parameters.FAST_THETA","MCNUM");
        mccomp_param_nexus(nxhandle,"0032_FOchop3", "radius", "0.5", "0.35","MCNUM");
        mccomp_param_nexus(nxhandle,"0032_FOchop3", "yheight", "NONE", "0.35","MCNUM");
        mccomp_param_nexus(nxhandle,"0032_FOchop3", "nu", "NONE", "_instrument_var._parameters.F_fast2 / _instrument_var._parameters.FO3","MCNUM");
        mccomp_param_nexus(nxhandle,"0032_FOchop3", "nslit", "3", "_instrument_var._parameters.N_fast","MCNUM");
        mccomp_param_nexus(nxhandle,"0032_FOchop3", "jitter", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0032_FOchop3", "delay", "0", "t_fast3","MCNUM");
        mccomp_param_nexus(nxhandle,"0032_FOchop3", "isfirst", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0032_FOchop3", "n_pulse", "1", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0032_FOchop3", "abs_out", "1", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0032_FOchop3", "phase", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0032_FOchop3", "xwidth", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0032_FOchop3", "verbose", "0", "0","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _FOchop3_setpos */

/* component TOFfast2_zoom=TOF_monitor() SETTING, POSITION/ROTATION */
int _TOFfast2_zoom_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_TOFfast2_zoom_setpos] component TOFfast2_zoom=TOF_monitor() SETTING [TOF_monitor:0]");
  stracpy(_TOFfast2_zoom_var._name, "TOFfast2_zoom", 16384);
  stracpy(_TOFfast2_zoom_var._type, "TOF_monitor", 16384);
  _TOFfast2_zoom_var._index=34;
  int current_setpos_index = 34;
  _TOFfast2_zoom_var._parameters.nt = 100;
  if("TOF_fast2.dat" && strlen("TOF_fast2.dat"))
    stracpy(_TOFfast2_zoom_var._parameters.filename, "TOF_fast2.dat" ? "TOF_fast2.dat" : "", 16384);
  else 
  _TOFfast2_zoom_var._parameters.filename[0]='\0';
  _TOFfast2_zoom_var._parameters.xmin = -0.05;
  _TOFfast2_zoom_var._parameters.xmax = 0.05;
  _TOFfast2_zoom_var._parameters.ymin = -0.05;
  _TOFfast2_zoom_var._parameters.ymax = 0.05;
  _TOFfast2_zoom_var._parameters.xwidth = 1.1;
  _TOFfast2_zoom_var._parameters.yheight = 1.2;
  _TOFfast2_zoom_var._parameters.tmin = 1e6 * t_fast3 -2e2;
  _TOFfast2_zoom_var._parameters.tmax = 1e6 * t_fast3 + 2e2;
  _TOFfast2_zoom_var._parameters.dt = 1.0;
  _TOFfast2_zoom_var._parameters.restore_neutron = 0;
  _TOFfast2_zoom_var._parameters.nowritefile = 0;


  /* component TOFfast2_zoom=TOF_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _FOchop3_var._rotation_absolute, _TOFfast2_zoom_var._rotation_absolute);
    rot_transpose(_FOchop3_var._rotation_absolute, tr1);
    rot_mul(_TOFfast2_zoom_var._rotation_absolute, tr1, _TOFfast2_zoom_var._rotation_relative);
    _TOFfast2_zoom_var._rotation_is_identity =  rot_test_identity(_TOFfast2_zoom_var._rotation_relative);
    tc1 = coords_set(
      0, -0.04, 1e-6);
    rot_transpose(_FOchop3_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _TOFfast2_zoom_var._position_absolute = coords_add(_FOchop3_var._position_absolute, tc2);
    tc1 = coords_sub(_FOchop3_var._position_absolute, _TOFfast2_zoom_var._position_absolute);
    _TOFfast2_zoom_var._position_relative = rot_apply(_TOFfast2_zoom_var._rotation_absolute, tc1);
  } /* TOFfast2_zoom=TOF_monitor() AT ROTATED */
  DEBUG_COMPONENT("TOFfast2_zoom", _TOFfast2_zoom_var._position_absolute, _TOFfast2_zoom_var._rotation_absolute);
  instrument->_position_absolute[34] = _TOFfast2_zoom_var._position_absolute;
  instrument->_position_relative[34] = _TOFfast2_zoom_var._position_relative;
    _TOFfast2_zoom_var._position_relative_is_zero =  coords_test_zero(_TOFfast2_zoom_var._position_relative);
  instrument->counter_N[34]  = instrument->counter_P[34] = instrument->counter_P2[34] = 0;
  instrument->counter_AbsorbProp[34]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0033_TOFfast2_zoom", _TOFfast2_zoom_var._position_absolute, _TOFfast2_zoom_var._rotation_absolute, "TOF_monitor");
        mccomp_param_nexus(nxhandle,"0033_TOFfast2_zoom", "nt", "20", "100","int");
        mccomp_param_nexus(nxhandle,"0033_TOFfast2_zoom", "filename", 0, "TOF_fast2.dat", "char*");
        mccomp_param_nexus(nxhandle,"0033_TOFfast2_zoom", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0033_TOFfast2_zoom", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0033_TOFfast2_zoom", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0033_TOFfast2_zoom", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0033_TOFfast2_zoom", "xwidth", "0", "1.1","MCNUM");
        mccomp_param_nexus(nxhandle,"0033_TOFfast2_zoom", "yheight", "0", "1.2","MCNUM");
        mccomp_param_nexus(nxhandle,"0033_TOFfast2_zoom", "tmin", "0", "1e6 * t_fast3 -2e2","MCNUM");
        mccomp_param_nexus(nxhandle,"0033_TOFfast2_zoom", "tmax", "0", "1e6 * t_fast3 + 2e2","MCNUM");
        mccomp_param_nexus(nxhandle,"0033_TOFfast2_zoom", "dt", "1.0", "1.0","MCNUM");
        mccomp_param_nexus(nxhandle,"0033_TOFfast2_zoom", "restore_neutron", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0033_TOFfast2_zoom", "nowritefile", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _TOFfast2_zoom_setpos */

/* component Lmon_afterfast2=L_monitor() SETTING, POSITION/ROTATION */
int _Lmon_afterfast2_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Lmon_afterfast2_setpos] component Lmon_afterfast2=L_monitor() SETTING [L_monitor:0]");
  stracpy(_Lmon_afterfast2_var._name, "Lmon_afterfast2", 16384);
  stracpy(_Lmon_afterfast2_var._type, "L_monitor", 16384);
  _Lmon_afterfast2_var._index=35;
  int current_setpos_index = 35;
  _Lmon_afterfast2_var._parameters.nL = 500;
  if("Lmon_afterfast2.dat" && strlen("Lmon_afterfast2.dat"))
    stracpy(_Lmon_afterfast2_var._parameters.filename, "Lmon_afterfast2.dat" ? "Lmon_afterfast2.dat" : "", 16384);
  else 
  _Lmon_afterfast2_var._parameters.filename[0]='\0';
  _Lmon_afterfast2_var._parameters.nowritefile = 0;
  _Lmon_afterfast2_var._parameters.xmin = -0.05;
  _Lmon_afterfast2_var._parameters.xmax = 0.05;
  _Lmon_afterfast2_var._parameters.ymin = -0.05;
  _Lmon_afterfast2_var._parameters.ymax = 0.05;
  _Lmon_afterfast2_var._parameters.xwidth = 0.06;
  _Lmon_afterfast2_var._parameters.yheight = 0.18;
  _Lmon_afterfast2_var._parameters.Lmin = 0;
  _Lmon_afterfast2_var._parameters.Lmax = _instrument_var._parameters.Lmax + 1;
  _Lmon_afterfast2_var._parameters.restore_neutron = 0;


  /* component Lmon_afterfast2=L_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _TOFfast2_zoom_var._rotation_absolute, _Lmon_afterfast2_var._rotation_absolute);
    rot_transpose(_TOFfast2_zoom_var._rotation_absolute, tr1);
    rot_mul(_Lmon_afterfast2_var._rotation_absolute, tr1, _Lmon_afterfast2_var._rotation_relative);
    _Lmon_afterfast2_var._rotation_is_identity =  rot_test_identity(_Lmon_afterfast2_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1e-6);
    rot_transpose(_TOFfast2_zoom_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Lmon_afterfast2_var._position_absolute = coords_add(_TOFfast2_zoom_var._position_absolute, tc2);
    tc1 = coords_sub(_TOFfast2_zoom_var._position_absolute, _Lmon_afterfast2_var._position_absolute);
    _Lmon_afterfast2_var._position_relative = rot_apply(_Lmon_afterfast2_var._rotation_absolute, tc1);
  } /* Lmon_afterfast2=L_monitor() AT ROTATED */
  DEBUG_COMPONENT("Lmon_afterfast2", _Lmon_afterfast2_var._position_absolute, _Lmon_afterfast2_var._rotation_absolute);
  instrument->_position_absolute[35] = _Lmon_afterfast2_var._position_absolute;
  instrument->_position_relative[35] = _Lmon_afterfast2_var._position_relative;
    _Lmon_afterfast2_var._position_relative_is_zero =  coords_test_zero(_Lmon_afterfast2_var._position_relative);
  instrument->counter_N[35]  = instrument->counter_P[35] = instrument->counter_P2[35] = 0;
  instrument->counter_AbsorbProp[35]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0034_Lmon_afterfast2", _Lmon_afterfast2_var._position_absolute, _Lmon_afterfast2_var._rotation_absolute, "L_monitor");
        mccomp_param_nexus(nxhandle,"0034_Lmon_afterfast2", "nL", "20", "500","int");
        mccomp_param_nexus(nxhandle,"0034_Lmon_afterfast2", "filename", 0, "Lmon_afterfast2.dat", "char*");
        mccomp_param_nexus(nxhandle,"0034_Lmon_afterfast2", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0034_Lmon_afterfast2", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0034_Lmon_afterfast2", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0034_Lmon_afterfast2", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0034_Lmon_afterfast2", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0034_Lmon_afterfast2", "xwidth", "0", "0.06","MCNUM");
        mccomp_param_nexus(nxhandle,"0034_Lmon_afterfast2", "yheight", "0", "0.18","MCNUM");
        mccomp_param_nexus(nxhandle,"0034_Lmon_afterfast2", "Lmin", "NONE", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0034_Lmon_afterfast2", "Lmax", "NONE", "_instrument_var._parameters.Lmax + 1","MCNUM");
        mccomp_param_nexus(nxhandle,"0034_Lmon_afterfast2", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Lmon_afterfast2_setpos */

/* component TOFL_afterfast2=TOFLambda_monitor() SETTING, POSITION/ROTATION */
int _TOFL_afterfast2_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_TOFL_afterfast2_setpos] component TOFL_afterfast2=TOFLambda_monitor() SETTING [TOFLambda_monitor:0]");
  stracpy(_TOFL_afterfast2_var._name, "TOFL_afterfast2", 16384);
  stracpy(_TOFL_afterfast2_var._type, "TOFLambda_monitor", 16384);
  _TOFL_afterfast2_var._index=36;
  int current_setpos_index = 36;
  _TOFL_afterfast2_var._parameters.nowritefile = 0;
  _TOFL_afterfast2_var._parameters.nL = 200;
  _TOFL_afterfast2_var._parameters.nt = 200;
  _TOFL_afterfast2_var._parameters.tmin = 0;
  _TOFL_afterfast2_var._parameters.tmax = 3.0e5;
  if("TOF_afterfast2.dat" && strlen("TOF_afterfast2.dat"))
    stracpy(_TOFL_afterfast2_var._parameters.filename, "TOF_afterfast2.dat" ? "TOF_afterfast2.dat" : "", 16384);
  else 
  _TOFL_afterfast2_var._parameters.filename[0]='\0';
  _TOFL_afterfast2_var._parameters.xmin = -0.05;
  _TOFL_afterfast2_var._parameters.xmax = 0.05;
  _TOFL_afterfast2_var._parameters.ymin = -0.05;
  _TOFL_afterfast2_var._parameters.ymax = 0.05;
  _TOFL_afterfast2_var._parameters.xwidth = 0.05;
  _TOFL_afterfast2_var._parameters.yheight = 0.12;
  _TOFL_afterfast2_var._parameters.Lmin = 0;
  _TOFL_afterfast2_var._parameters.Lmax = _instrument_var._parameters.Lmax + 1;
  _TOFL_afterfast2_var._parameters.restore_neutron = 0;


  /* component TOFL_afterfast2=TOFLambda_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Lmon_afterfast2_var._rotation_absolute, _TOFL_afterfast2_var._rotation_absolute);
    rot_transpose(_Lmon_afterfast2_var._rotation_absolute, tr1);
    rot_mul(_TOFL_afterfast2_var._rotation_absolute, tr1, _TOFL_afterfast2_var._rotation_relative);
    _TOFL_afterfast2_var._rotation_is_identity =  rot_test_identity(_TOFL_afterfast2_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1e-6);
    rot_transpose(_Lmon_afterfast2_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _TOFL_afterfast2_var._position_absolute = coords_add(_Lmon_afterfast2_var._position_absolute, tc2);
    tc1 = coords_sub(_Lmon_afterfast2_var._position_absolute, _TOFL_afterfast2_var._position_absolute);
    _TOFL_afterfast2_var._position_relative = rot_apply(_TOFL_afterfast2_var._rotation_absolute, tc1);
  } /* TOFL_afterfast2=TOFLambda_monitor() AT ROTATED */
  DEBUG_COMPONENT("TOFL_afterfast2", _TOFL_afterfast2_var._position_absolute, _TOFL_afterfast2_var._rotation_absolute);
  instrument->_position_absolute[36] = _TOFL_afterfast2_var._position_absolute;
  instrument->_position_relative[36] = _TOFL_afterfast2_var._position_relative;
    _TOFL_afterfast2_var._position_relative_is_zero =  coords_test_zero(_TOFL_afterfast2_var._position_relative);
  instrument->counter_N[36]  = instrument->counter_P[36] = instrument->counter_P2[36] = 0;
  instrument->counter_AbsorbProp[36]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0035_TOFL_afterfast2", _TOFL_afterfast2_var._position_absolute, _TOFL_afterfast2_var._rotation_absolute, "TOFLambda_monitor");
        mccomp_param_nexus(nxhandle,"0035_TOFL_afterfast2", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0035_TOFL_afterfast2", "nL", "20", "200","int");
        mccomp_param_nexus(nxhandle,"0035_TOFL_afterfast2", "nt", "128", "200","int");
        mccomp_param_nexus(nxhandle,"0035_TOFL_afterfast2", "tmin", "NONE", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0035_TOFL_afterfast2", "tmax", "NONE", "3.0e5","MCNUM");
        mccomp_param_nexus(nxhandle,"0035_TOFL_afterfast2", "filename", 0, "TOF_afterfast2.dat", "char*");
        mccomp_param_nexus(nxhandle,"0035_TOFL_afterfast2", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0035_TOFL_afterfast2", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0035_TOFL_afterfast2", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0035_TOFL_afterfast2", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0035_TOFL_afterfast2", "xwidth", "0", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0035_TOFL_afterfast2", "yheight", "0", "0.12","MCNUM");
        mccomp_param_nexus(nxhandle,"0035_TOFL_afterfast2", "Lmin", "NONE", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0035_TOFL_afterfast2", "Lmax", "NONE", "_instrument_var._parameters.Lmax + 1","MCNUM");
        mccomp_param_nexus(nxhandle,"0035_TOFL_afterfast2", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _TOFL_afterfast2_setpos */

/* component TOFL_afterfast2_zoom=TOFLambda_monitor() SETTING, POSITION/ROTATION */
int _TOFL_afterfast2_zoom_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_TOFL_afterfast2_zoom_setpos] component TOFL_afterfast2_zoom=TOFLambda_monitor() SETTING [TOFLambda_monitor:0]");
  stracpy(_TOFL_afterfast2_zoom_var._name, "TOFL_afterfast2_zoom", 16384);
  stracpy(_TOFL_afterfast2_zoom_var._type, "TOFLambda_monitor", 16384);
  _TOFL_afterfast2_zoom_var._index=37;
  int current_setpos_index = 37;
  _TOFL_afterfast2_zoom_var._parameters.nowritefile = 0;
  _TOFL_afterfast2_zoom_var._parameters.nL = 200;
  _TOFL_afterfast2_zoom_var._parameters.nt = 200;
  _TOFL_afterfast2_zoom_var._parameters.tmin = tmin_zoom;
  _TOFL_afterfast2_zoom_var._parameters.tmax = tmax_zoom;
  if("TOFL_afterfast2_zoom.dat" && strlen("TOFL_afterfast2_zoom.dat"))
    stracpy(_TOFL_afterfast2_zoom_var._parameters.filename, "TOFL_afterfast2_zoom.dat" ? "TOFL_afterfast2_zoom.dat" : "", 16384);
  else 
  _TOFL_afterfast2_zoom_var._parameters.filename[0]='\0';
  _TOFL_afterfast2_zoom_var._parameters.xmin = -0.05;
  _TOFL_afterfast2_zoom_var._parameters.xmax = 0.05;
  _TOFL_afterfast2_zoom_var._parameters.ymin = -0.05;
  _TOFL_afterfast2_zoom_var._parameters.ymax = 0.05;
  _TOFL_afterfast2_zoom_var._parameters.xwidth = 0.05;
  _TOFL_afterfast2_zoom_var._parameters.yheight = 0.12;
  _TOFL_afterfast2_zoom_var._parameters.Lmin = _instrument_var._parameters.lambda0 -0.2;
  _TOFL_afterfast2_zoom_var._parameters.Lmax = _instrument_var._parameters.lambda0 + 0.2;
  _TOFL_afterfast2_zoom_var._parameters.restore_neutron = 0;


  /* component TOFL_afterfast2_zoom=TOFLambda_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _TOFL_afterfast2_var._rotation_absolute, _TOFL_afterfast2_zoom_var._rotation_absolute);
    rot_transpose(_TOFL_afterfast2_var._rotation_absolute, tr1);
    rot_mul(_TOFL_afterfast2_zoom_var._rotation_absolute, tr1, _TOFL_afterfast2_zoom_var._rotation_relative);
    _TOFL_afterfast2_zoom_var._rotation_is_identity =  rot_test_identity(_TOFL_afterfast2_zoom_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1e-6);
    rot_transpose(_TOFL_afterfast2_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _TOFL_afterfast2_zoom_var._position_absolute = coords_add(_TOFL_afterfast2_var._position_absolute, tc2);
    tc1 = coords_sub(_TOFL_afterfast2_var._position_absolute, _TOFL_afterfast2_zoom_var._position_absolute);
    _TOFL_afterfast2_zoom_var._position_relative = rot_apply(_TOFL_afterfast2_zoom_var._rotation_absolute, tc1);
  } /* TOFL_afterfast2_zoom=TOFLambda_monitor() AT ROTATED */
  DEBUG_COMPONENT("TOFL_afterfast2_zoom", _TOFL_afterfast2_zoom_var._position_absolute, _TOFL_afterfast2_zoom_var._rotation_absolute);
  instrument->_position_absolute[37] = _TOFL_afterfast2_zoom_var._position_absolute;
  instrument->_position_relative[37] = _TOFL_afterfast2_zoom_var._position_relative;
    _TOFL_afterfast2_zoom_var._position_relative_is_zero =  coords_test_zero(_TOFL_afterfast2_zoom_var._position_relative);
  instrument->counter_N[37]  = instrument->counter_P[37] = instrument->counter_P2[37] = 0;
  instrument->counter_AbsorbProp[37]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0036_TOFL_afterfast2_zoom", _TOFL_afterfast2_zoom_var._position_absolute, _TOFL_afterfast2_zoom_var._rotation_absolute, "TOFLambda_monitor");
        mccomp_param_nexus(nxhandle,"0036_TOFL_afterfast2_zoom", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0036_TOFL_afterfast2_zoom", "nL", "20", "200","int");
        mccomp_param_nexus(nxhandle,"0036_TOFL_afterfast2_zoom", "nt", "128", "200","int");
        mccomp_param_nexus(nxhandle,"0036_TOFL_afterfast2_zoom", "tmin", "NONE", "tmin_zoom","MCNUM");
        mccomp_param_nexus(nxhandle,"0036_TOFL_afterfast2_zoom", "tmax", "NONE", "tmax_zoom","MCNUM");
        mccomp_param_nexus(nxhandle,"0036_TOFL_afterfast2_zoom", "filename", 0, "TOFL_afterfast2_zoom.dat", "char*");
        mccomp_param_nexus(nxhandle,"0036_TOFL_afterfast2_zoom", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0036_TOFL_afterfast2_zoom", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0036_TOFL_afterfast2_zoom", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0036_TOFL_afterfast2_zoom", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0036_TOFL_afterfast2_zoom", "xwidth", "0", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0036_TOFL_afterfast2_zoom", "yheight", "0", "0.12","MCNUM");
        mccomp_param_nexus(nxhandle,"0036_TOFL_afterfast2_zoom", "Lmin", "NONE", "_instrument_var._parameters.lambda0 -0.2","MCNUM");
        mccomp_param_nexus(nxhandle,"0036_TOFL_afterfast2_zoom", "Lmax", "NONE", "_instrument_var._parameters.lambda0 + 0.2","MCNUM");
        mccomp_param_nexus(nxhandle,"0036_TOFL_afterfast2_zoom", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _TOFL_afterfast2_zoom_setpos */

/* component PSD_afterfast2=PSD_monitor() SETTING, POSITION/ROTATION */
int _PSD_afterfast2_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_PSD_afterfast2_setpos] component PSD_afterfast2=PSD_monitor() SETTING [PSD_monitor:0]");
  stracpy(_PSD_afterfast2_var._name, "PSD_afterfast2", 16384);
  stracpy(_PSD_afterfast2_var._type, "PSD_monitor", 16384);
  _PSD_afterfast2_var._index=38;
  int current_setpos_index = 38;
  _PSD_afterfast2_var._parameters.nx = 90;
  _PSD_afterfast2_var._parameters.ny = 90;
  if("PSD_afterfast2.dat" && strlen("PSD_afterfast2.dat"))
    stracpy(_PSD_afterfast2_var._parameters.filename, "PSD_afterfast2.dat" ? "PSD_afterfast2.dat" : "", 16384);
  else 
  _PSD_afterfast2_var._parameters.filename[0]='\0';
  _PSD_afterfast2_var._parameters.xmin = -0.05;
  _PSD_afterfast2_var._parameters.xmax = 0.05;
  _PSD_afterfast2_var._parameters.ymin = -0.05;
  _PSD_afterfast2_var._parameters.ymax = 0.05;
  _PSD_afterfast2_var._parameters.xwidth = 0.1;
  _PSD_afterfast2_var._parameters.yheight = 0.25;
  _PSD_afterfast2_var._parameters.restore_neutron = 0;
  _PSD_afterfast2_var._parameters.nowritefile = 0;


  /* component PSD_afterfast2=PSD_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _TOFL_afterfast2_zoom_var._rotation_absolute, _PSD_afterfast2_var._rotation_absolute);
    rot_transpose(_TOFL_afterfast2_zoom_var._rotation_absolute, tr1);
    rot_mul(_PSD_afterfast2_var._rotation_absolute, tr1, _PSD_afterfast2_var._rotation_relative);
    _PSD_afterfast2_var._rotation_is_identity =  rot_test_identity(_PSD_afterfast2_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1e-6);
    rot_transpose(_TOFL_afterfast2_zoom_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _PSD_afterfast2_var._position_absolute = coords_add(_TOFL_afterfast2_zoom_var._position_absolute, tc2);
    tc1 = coords_sub(_TOFL_afterfast2_zoom_var._position_absolute, _PSD_afterfast2_var._position_absolute);
    _PSD_afterfast2_var._position_relative = rot_apply(_PSD_afterfast2_var._rotation_absolute, tc1);
  } /* PSD_afterfast2=PSD_monitor() AT ROTATED */
  DEBUG_COMPONENT("PSD_afterfast2", _PSD_afterfast2_var._position_absolute, _PSD_afterfast2_var._rotation_absolute);
  instrument->_position_absolute[38] = _PSD_afterfast2_var._position_absolute;
  instrument->_position_relative[38] = _PSD_afterfast2_var._position_relative;
    _PSD_afterfast2_var._position_relative_is_zero =  coords_test_zero(_PSD_afterfast2_var._position_relative);
  instrument->counter_N[38]  = instrument->counter_P[38] = instrument->counter_P2[38] = 0;
  instrument->counter_AbsorbProp[38]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0037_PSD_afterfast2", _PSD_afterfast2_var._position_absolute, _PSD_afterfast2_var._rotation_absolute, "PSD_monitor");
        mccomp_param_nexus(nxhandle,"0037_PSD_afterfast2", "nx", "90", "90","int");
        mccomp_param_nexus(nxhandle,"0037_PSD_afterfast2", "ny", "90", "90","int");
        mccomp_param_nexus(nxhandle,"0037_PSD_afterfast2", "filename", 0, "PSD_afterfast2.dat", "char*");
        mccomp_param_nexus(nxhandle,"0037_PSD_afterfast2", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0037_PSD_afterfast2", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0037_PSD_afterfast2", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0037_PSD_afterfast2", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0037_PSD_afterfast2", "xwidth", "0", "0.1","MCNUM");
        mccomp_param_nexus(nxhandle,"0037_PSD_afterfast2", "yheight", "0", "0.25","MCNUM");
        mccomp_param_nexus(nxhandle,"0037_PSD_afterfast2", "restore_neutron", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0037_PSD_afterfast2", "nowritefile", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _PSD_afterfast2_setpos */

/* component Guidesample=Guide() SETTING, POSITION/ROTATION */
int _Guidesample_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Guidesample_setpos] component Guidesample=Guide() SETTING [Guide:0]");
  stracpy(_Guidesample_var._name, "Guidesample", 16384);
  stracpy(_Guidesample_var._type, "Guide", 16384);
  _Guidesample_var._index=39;
  int current_setpos_index = 39;
  _Guidesample_var._parameters.reflect[0]='\0';
  _Guidesample_var._parameters.w1 = _instrument_var._parameters.W_chop;
  _Guidesample_var._parameters.h1 = _instrument_var._parameters.H_chop;
  _Guidesample_var._parameters.w2 = _instrument_var._parameters.W_end;
  _Guidesample_var._parameters.h2 = _instrument_var._parameters.H_end;
  _Guidesample_var._parameters.l = _instrument_var._parameters.SAMPLE_DIST -4 * _instrument_var._parameters.GUI_GAP;
  _Guidesample_var._parameters.R0 = 1;
  _Guidesample_var._parameters.Qc = 0.0219;
  _Guidesample_var._parameters.alpha = _instrument_var._parameters.ALPHA;
  _Guidesample_var._parameters.m = _instrument_var._parameters.M;
  _Guidesample_var._parameters.W = 0.003;


  /* component Guidesample=Guide() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _PSD_afterfast2_var._rotation_absolute, _Guidesample_var._rotation_absolute);
    rot_transpose(_PSD_afterfast2_var._rotation_absolute, tr1);
    rot_mul(_Guidesample_var._rotation_absolute, tr1, _Guidesample_var._rotation_relative);
    _Guidesample_var._rotation_is_identity =  rot_test_identity(_Guidesample_var._rotation_relative);
    tc1 = coords_set(
      0, 0, _instrument_var._parameters.GUI_GAP / 2);
    rot_transpose(_PSD_afterfast2_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Guidesample_var._position_absolute = coords_add(_PSD_afterfast2_var._position_absolute, tc2);
    tc1 = coords_sub(_PSD_afterfast2_var._position_absolute, _Guidesample_var._position_absolute);
    _Guidesample_var._position_relative = rot_apply(_Guidesample_var._rotation_absolute, tc1);
  } /* Guidesample=Guide() AT ROTATED */
  DEBUG_COMPONENT("Guidesample", _Guidesample_var._position_absolute, _Guidesample_var._rotation_absolute);
  instrument->_position_absolute[39] = _Guidesample_var._position_absolute;
  instrument->_position_relative[39] = _Guidesample_var._position_relative;
    _Guidesample_var._position_relative_is_zero =  coords_test_zero(_Guidesample_var._position_relative);
  instrument->counter_N[39]  = instrument->counter_P[39] = instrument->counter_P2[39] = 0;
  instrument->counter_AbsorbProp[39]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0038_Guidesample", _Guidesample_var._position_absolute, _Guidesample_var._rotation_absolute, "Guide");
        mccomp_param_nexus(nxhandle,"0038_Guidesample", "reflect", 0, 0, "char*");
        mccomp_param_nexus(nxhandle,"0038_Guidesample", "w1", "NONE", "_instrument_var._parameters.W_chop","MCNUM");
        mccomp_param_nexus(nxhandle,"0038_Guidesample", "h1", "NONE", "_instrument_var._parameters.H_chop","MCNUM");
        mccomp_param_nexus(nxhandle,"0038_Guidesample", "w2", "0", "_instrument_var._parameters.W_end","MCNUM");
        mccomp_param_nexus(nxhandle,"0038_Guidesample", "h2", "0", "_instrument_var._parameters.H_end","MCNUM");
        mccomp_param_nexus(nxhandle,"0038_Guidesample", "l", "NONE", "_instrument_var._parameters.SAMPLE_DIST -4 * _instrument_var._parameters.GUI_GAP","MCNUM");
        mccomp_param_nexus(nxhandle,"0038_Guidesample", "R0", "0.99", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0038_Guidesample", "Qc", "0.0219", "0.0219","MCNUM");
        mccomp_param_nexus(nxhandle,"0038_Guidesample", "alpha", "6.07", "_instrument_var._parameters.ALPHA","MCNUM");
        mccomp_param_nexus(nxhandle,"0038_Guidesample", "m", "2", "_instrument_var._parameters.M","MCNUM");
        mccomp_param_nexus(nxhandle,"0038_Guidesample", "W", "0.003", "0.003","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Guidesample_setpos */

/* component Lmon_guideend=L_monitor() SETTING, POSITION/ROTATION */
int _Lmon_guideend_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Lmon_guideend_setpos] component Lmon_guideend=L_monitor() SETTING [L_monitor:0]");
  stracpy(_Lmon_guideend_var._name, "Lmon_guideend", 16384);
  stracpy(_Lmon_guideend_var._type, "L_monitor", 16384);
  _Lmon_guideend_var._index=40;
  int current_setpos_index = 40;
  _Lmon_guideend_var._parameters.nL = 1000;
  if("Lmon_guideend.dat" && strlen("Lmon_guideend.dat"))
    stracpy(_Lmon_guideend_var._parameters.filename, "Lmon_guideend.dat" ? "Lmon_guideend.dat" : "", 16384);
  else 
  _Lmon_guideend_var._parameters.filename[0]='\0';
  _Lmon_guideend_var._parameters.nowritefile = 0;
  _Lmon_guideend_var._parameters.xmin = -0.05;
  _Lmon_guideend_var._parameters.xmax = 0.05;
  _Lmon_guideend_var._parameters.ymin = -0.05;
  _Lmon_guideend_var._parameters.ymax = 0.05;
  _Lmon_guideend_var._parameters.xwidth = _instrument_var._parameters.W_end + 0.01;
  _Lmon_guideend_var._parameters.yheight = _instrument_var._parameters.H_end + 0.01;
  _Lmon_guideend_var._parameters.Lmin = 0;
  _Lmon_guideend_var._parameters.Lmax = _instrument_var._parameters.Lmax + 1;
  _Lmon_guideend_var._parameters.restore_neutron = 0;


  /* component Lmon_guideend=L_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Guidesample_var._rotation_absolute, _Lmon_guideend_var._rotation_absolute);
    rot_transpose(_Guidesample_var._rotation_absolute, tr1);
    rot_mul(_Lmon_guideend_var._rotation_absolute, tr1, _Lmon_guideend_var._rotation_relative);
    _Lmon_guideend_var._rotation_is_identity =  rot_test_identity(_Lmon_guideend_var._rotation_relative);
    tc1 = coords_set(
      0, 0, _instrument_var._parameters.SAMPLE_DIST -4 * _instrument_var._parameters.GUI_GAP + 1e-6);
    rot_transpose(_Guidesample_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Lmon_guideend_var._position_absolute = coords_add(_Guidesample_var._position_absolute, tc2);
    tc1 = coords_sub(_Guidesample_var._position_absolute, _Lmon_guideend_var._position_absolute);
    _Lmon_guideend_var._position_relative = rot_apply(_Lmon_guideend_var._rotation_absolute, tc1);
  } /* Lmon_guideend=L_monitor() AT ROTATED */
  DEBUG_COMPONENT("Lmon_guideend", _Lmon_guideend_var._position_absolute, _Lmon_guideend_var._rotation_absolute);
  instrument->_position_absolute[40] = _Lmon_guideend_var._position_absolute;
  instrument->_position_relative[40] = _Lmon_guideend_var._position_relative;
    _Lmon_guideend_var._position_relative_is_zero =  coords_test_zero(_Lmon_guideend_var._position_relative);
  instrument->counter_N[40]  = instrument->counter_P[40] = instrument->counter_P2[40] = 0;
  instrument->counter_AbsorbProp[40]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0039_Lmon_guideend", _Lmon_guideend_var._position_absolute, _Lmon_guideend_var._rotation_absolute, "L_monitor");
        mccomp_param_nexus(nxhandle,"0039_Lmon_guideend", "nL", "20", "1000","int");
        mccomp_param_nexus(nxhandle,"0039_Lmon_guideend", "filename", 0, "Lmon_guideend.dat", "char*");
        mccomp_param_nexus(nxhandle,"0039_Lmon_guideend", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0039_Lmon_guideend", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0039_Lmon_guideend", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0039_Lmon_guideend", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0039_Lmon_guideend", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0039_Lmon_guideend", "xwidth", "0", "_instrument_var._parameters.W_end + 0.01","MCNUM");
        mccomp_param_nexus(nxhandle,"0039_Lmon_guideend", "yheight", "0", "_instrument_var._parameters.H_end + 0.01","MCNUM");
        mccomp_param_nexus(nxhandle,"0039_Lmon_guideend", "Lmin", "NONE", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0039_Lmon_guideend", "Lmax", "NONE", "_instrument_var._parameters.Lmax + 1","MCNUM");
        mccomp_param_nexus(nxhandle,"0039_Lmon_guideend", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Lmon_guideend_setpos */

/* component PSDsample=PSD_monitor() SETTING, POSITION/ROTATION */
int _PSDsample_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_PSDsample_setpos] component PSDsample=PSD_monitor() SETTING [PSD_monitor:0]");
  stracpy(_PSDsample_var._name, "PSDsample", 16384);
  stracpy(_PSDsample_var._type, "PSD_monitor", 16384);
  _PSDsample_var._index=41;
  int current_setpos_index = 41;
  _PSDsample_var._parameters.nx = 90;
  _PSDsample_var._parameters.ny = 90;
  if("PSDsample.dat" && strlen("PSDsample.dat"))
    stracpy(_PSDsample_var._parameters.filename, "PSDsample.dat" ? "PSDsample.dat" : "", 16384);
  else 
  _PSDsample_var._parameters.filename[0]='\0';
  _PSDsample_var._parameters.xmin = -0.05;
  _PSDsample_var._parameters.xmax = 0.05;
  _PSDsample_var._parameters.ymin = -0.05;
  _PSDsample_var._parameters.ymax = 0.05;
  _PSDsample_var._parameters.xwidth = 0.1;
  _PSDsample_var._parameters.yheight = 0.25;
  _PSDsample_var._parameters.restore_neutron = 0;
  _PSDsample_var._parameters.nowritefile = 0;


  /* component PSDsample=PSD_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Lmon_guideend_var._rotation_absolute, _PSDsample_var._rotation_absolute);
    rot_transpose(_Lmon_guideend_var._rotation_absolute, tr1);
    rot_mul(_PSDsample_var._rotation_absolute, tr1, _PSDsample_var._rotation_relative);
    _PSDsample_var._rotation_is_identity =  rot_test_identity(_PSDsample_var._rotation_relative);
    tc1 = coords_set(
      0, 0, _instrument_var._parameters.SAMPLE_DIST -3 * _instrument_var._parameters.GUI_GAP + 1e-6);
    rot_transpose(_Lmon_guideend_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _PSDsample_var._position_absolute = coords_add(_Lmon_guideend_var._position_absolute, tc2);
    tc1 = coords_sub(_Lmon_guideend_var._position_absolute, _PSDsample_var._position_absolute);
    _PSDsample_var._position_relative = rot_apply(_PSDsample_var._rotation_absolute, tc1);
  } /* PSDsample=PSD_monitor() AT ROTATED */
  DEBUG_COMPONENT("PSDsample", _PSDsample_var._position_absolute, _PSDsample_var._rotation_absolute);
  instrument->_position_absolute[41] = _PSDsample_var._position_absolute;
  instrument->_position_relative[41] = _PSDsample_var._position_relative;
    _PSDsample_var._position_relative_is_zero =  coords_test_zero(_PSDsample_var._position_relative);
  instrument->counter_N[41]  = instrument->counter_P[41] = instrument->counter_P2[41] = 0;
  instrument->counter_AbsorbProp[41]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0040_PSDsample", _PSDsample_var._position_absolute, _PSDsample_var._rotation_absolute, "PSD_monitor");
        mccomp_param_nexus(nxhandle,"0040_PSDsample", "nx", "90", "90","int");
        mccomp_param_nexus(nxhandle,"0040_PSDsample", "ny", "90", "90","int");
        mccomp_param_nexus(nxhandle,"0040_PSDsample", "filename", 0, "PSDsample.dat", "char*");
        mccomp_param_nexus(nxhandle,"0040_PSDsample", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0040_PSDsample", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0040_PSDsample", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0040_PSDsample", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0040_PSDsample", "xwidth", "0", "0.1","MCNUM");
        mccomp_param_nexus(nxhandle,"0040_PSDsample", "yheight", "0", "0.25","MCNUM");
        mccomp_param_nexus(nxhandle,"0040_PSDsample", "restore_neutron", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0040_PSDsample", "nowritefile", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _PSDsample_setpos */

/* component TOFsample_zoom=TOF_monitor() SETTING, POSITION/ROTATION */
int _TOFsample_zoom_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_TOFsample_zoom_setpos] component TOFsample_zoom=TOF_monitor() SETTING [TOF_monitor:0]");
  stracpy(_TOFsample_zoom_var._name, "TOFsample_zoom", 16384);
  stracpy(_TOFsample_zoom_var._type, "TOF_monitor", 16384);
  _TOFsample_zoom_var._index=42;
  int current_setpos_index = 42;
  _TOFsample_zoom_var._parameters.nt = 500;
  if("TOF_sample.dat" && strlen("TOF_sample.dat"))
    stracpy(_TOFsample_zoom_var._parameters.filename, "TOF_sample.dat" ? "TOF_sample.dat" : "", 16384);
  else 
  _TOFsample_zoom_var._parameters.filename[0]='\0';
  _TOFsample_zoom_var._parameters.xmin = -0.05;
  _TOFsample_zoom_var._parameters.xmax = 0.05;
  _TOFsample_zoom_var._parameters.ymin = -0.05;
  _TOFsample_zoom_var._parameters.ymax = 0.05;
  _TOFsample_zoom_var._parameters.xwidth = 0.02;
  _TOFsample_zoom_var._parameters.yheight = 0.04;
  _TOFsample_zoom_var._parameters.tmin = 1e6 * t_sample -5e4;
  _TOFsample_zoom_var._parameters.tmax = 1e6 * t_sample + 5e4;
  _TOFsample_zoom_var._parameters.dt = 1.0;
  _TOFsample_zoom_var._parameters.restore_neutron = 0;
  _TOFsample_zoom_var._parameters.nowritefile = 0;


  /* component TOFsample_zoom=TOF_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _PSDsample_var._rotation_absolute, _TOFsample_zoom_var._rotation_absolute);
    rot_transpose(_PSDsample_var._rotation_absolute, tr1);
    rot_mul(_TOFsample_zoom_var._rotation_absolute, tr1, _TOFsample_zoom_var._rotation_relative);
    _TOFsample_zoom_var._rotation_is_identity =  rot_test_identity(_TOFsample_zoom_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1e-6);
    rot_transpose(_PSDsample_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _TOFsample_zoom_var._position_absolute = coords_add(_PSDsample_var._position_absolute, tc2);
    tc1 = coords_sub(_PSDsample_var._position_absolute, _TOFsample_zoom_var._position_absolute);
    _TOFsample_zoom_var._position_relative = rot_apply(_TOFsample_zoom_var._rotation_absolute, tc1);
  } /* TOFsample_zoom=TOF_monitor() AT ROTATED */
  DEBUG_COMPONENT("TOFsample_zoom", _TOFsample_zoom_var._position_absolute, _TOFsample_zoom_var._rotation_absolute);
  instrument->_position_absolute[42] = _TOFsample_zoom_var._position_absolute;
  instrument->_position_relative[42] = _TOFsample_zoom_var._position_relative;
    _TOFsample_zoom_var._position_relative_is_zero =  coords_test_zero(_TOFsample_zoom_var._position_relative);
  instrument->counter_N[42]  = instrument->counter_P[42] = instrument->counter_P2[42] = 0;
  instrument->counter_AbsorbProp[42]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0041_TOFsample_zoom", _TOFsample_zoom_var._position_absolute, _TOFsample_zoom_var._rotation_absolute, "TOF_monitor");
        mccomp_param_nexus(nxhandle,"0041_TOFsample_zoom", "nt", "20", "500","int");
        mccomp_param_nexus(nxhandle,"0041_TOFsample_zoom", "filename", 0, "TOF_sample.dat", "char*");
        mccomp_param_nexus(nxhandle,"0041_TOFsample_zoom", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0041_TOFsample_zoom", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0041_TOFsample_zoom", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0041_TOFsample_zoom", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0041_TOFsample_zoom", "xwidth", "0", "0.02","MCNUM");
        mccomp_param_nexus(nxhandle,"0041_TOFsample_zoom", "yheight", "0", "0.04","MCNUM");
        mccomp_param_nexus(nxhandle,"0041_TOFsample_zoom", "tmin", "0", "1e6 * t_sample -5e4","MCNUM");
        mccomp_param_nexus(nxhandle,"0041_TOFsample_zoom", "tmax", "0", "1e6 * t_sample + 5e4","MCNUM");
        mccomp_param_nexus(nxhandle,"0041_TOFsample_zoom", "dt", "1.0", "1.0","MCNUM");
        mccomp_param_nexus(nxhandle,"0041_TOFsample_zoom", "restore_neutron", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0041_TOFsample_zoom", "nowritefile", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _TOFsample_zoom_setpos */

/* component Esample=E_monitor() SETTING, POSITION/ROTATION */
int _Esample_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Esample_setpos] component Esample=E_monitor() SETTING [E_monitor:0]");
  stracpy(_Esample_var._name, "Esample", 16384);
  stracpy(_Esample_var._type, "E_monitor", 16384);
  _Esample_var._index=43;
  int current_setpos_index = 43;
  _Esample_var._parameters.nE = 400;
  if("Esample" && strlen("Esample"))
    stracpy(_Esample_var._parameters.filename, "Esample" ? "Esample" : "", 16384);
  else 
  _Esample_var._parameters.filename[0]='\0';
  _Esample_var._parameters.xmin = -0.05;
  _Esample_var._parameters.xmax = 0.05;
  _Esample_var._parameters.ymin = -0.05;
  _Esample_var._parameters.ymax = 0.05;
  _Esample_var._parameters.nowritefile = 0;
  _Esample_var._parameters.xwidth = 0.02;
  _Esample_var._parameters.yheight = 0.04;
  _Esample_var._parameters.Emin = E_target - _instrument_var._parameters.RES_DE;
  _Esample_var._parameters.Emax = E_target + _instrument_var._parameters.RES_DE;
  _Esample_var._parameters.restore_neutron = 0;


  /* component Esample=E_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _TOFsample_zoom_var._rotation_absolute, _Esample_var._rotation_absolute);
    rot_transpose(_TOFsample_zoom_var._rotation_absolute, tr1);
    rot_mul(_Esample_var._rotation_absolute, tr1, _Esample_var._rotation_relative);
    _Esample_var._rotation_is_identity =  rot_test_identity(_Esample_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1e-6);
    rot_transpose(_TOFsample_zoom_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Esample_var._position_absolute = coords_add(_TOFsample_zoom_var._position_absolute, tc2);
    tc1 = coords_sub(_TOFsample_zoom_var._position_absolute, _Esample_var._position_absolute);
    _Esample_var._position_relative = rot_apply(_Esample_var._rotation_absolute, tc1);
  } /* Esample=E_monitor() AT ROTATED */
  DEBUG_COMPONENT("Esample", _Esample_var._position_absolute, _Esample_var._rotation_absolute);
  instrument->_position_absolute[43] = _Esample_var._position_absolute;
  instrument->_position_relative[43] = _Esample_var._position_relative;
    _Esample_var._position_relative_is_zero =  coords_test_zero(_Esample_var._position_relative);
  instrument->counter_N[43]  = instrument->counter_P[43] = instrument->counter_P2[43] = 0;
  instrument->counter_AbsorbProp[43]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0042_Esample", _Esample_var._position_absolute, _Esample_var._rotation_absolute, "E_monitor");
        mccomp_param_nexus(nxhandle,"0042_Esample", "nE", "20", "400","int");
        mccomp_param_nexus(nxhandle,"0042_Esample", "filename", 0, "Esample", "char*");
        mccomp_param_nexus(nxhandle,"0042_Esample", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0042_Esample", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0042_Esample", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0042_Esample", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0042_Esample", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0042_Esample", "xwidth", "0", "0.02","MCNUM");
        mccomp_param_nexus(nxhandle,"0042_Esample", "yheight", "0", "0.04","MCNUM");
        mccomp_param_nexus(nxhandle,"0042_Esample", "Emin", "NONE", "E_target - _instrument_var._parameters.RES_DE","MCNUM");
        mccomp_param_nexus(nxhandle,"0042_Esample", "Emax", "NONE", "E_target + _instrument_var._parameters.RES_DE","MCNUM");
        mccomp_param_nexus(nxhandle,"0042_Esample", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Esample_setpos */

/* component Lmon_sample_zoom=L_monitor() SETTING, POSITION/ROTATION */
int _Lmon_sample_zoom_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Lmon_sample_zoom_setpos] component Lmon_sample_zoom=L_monitor() SETTING [L_monitor:0]");
  stracpy(_Lmon_sample_zoom_var._name, "Lmon_sample_zoom", 16384);
  stracpy(_Lmon_sample_zoom_var._type, "L_monitor", 16384);
  _Lmon_sample_zoom_var._index=44;
  int current_setpos_index = 44;
  _Lmon_sample_zoom_var._parameters.nL = 100;
  if("LMON_sample_zoom.dat" && strlen("LMON_sample_zoom.dat"))
    stracpy(_Lmon_sample_zoom_var._parameters.filename, "LMON_sample_zoom.dat" ? "LMON_sample_zoom.dat" : "", 16384);
  else 
  _Lmon_sample_zoom_var._parameters.filename[0]='\0';
  _Lmon_sample_zoom_var._parameters.nowritefile = 0;
  _Lmon_sample_zoom_var._parameters.xmin = -0.05;
  _Lmon_sample_zoom_var._parameters.xmax = 0.05;
  _Lmon_sample_zoom_var._parameters.ymin = -0.05;
  _Lmon_sample_zoom_var._parameters.ymax = 0.05;
  _Lmon_sample_zoom_var._parameters.xwidth = 0.02;
  _Lmon_sample_zoom_var._parameters.yheight = 0.04;
  _Lmon_sample_zoom_var._parameters.Lmin = _instrument_var._parameters.lambda0 -0.1;
  _Lmon_sample_zoom_var._parameters.Lmax = _instrument_var._parameters.lambda0 + 0.1;
  _Lmon_sample_zoom_var._parameters.restore_neutron = 0;


  /* component Lmon_sample_zoom=L_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Esample_var._rotation_absolute, _Lmon_sample_zoom_var._rotation_absolute);
    rot_transpose(_Esample_var._rotation_absolute, tr1);
    rot_mul(_Lmon_sample_zoom_var._rotation_absolute, tr1, _Lmon_sample_zoom_var._rotation_relative);
    _Lmon_sample_zoom_var._rotation_is_identity =  rot_test_identity(_Lmon_sample_zoom_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1e-6);
    rot_transpose(_Esample_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Lmon_sample_zoom_var._position_absolute = coords_add(_Esample_var._position_absolute, tc2);
    tc1 = coords_sub(_Esample_var._position_absolute, _Lmon_sample_zoom_var._position_absolute);
    _Lmon_sample_zoom_var._position_relative = rot_apply(_Lmon_sample_zoom_var._rotation_absolute, tc1);
  } /* Lmon_sample_zoom=L_monitor() AT ROTATED */
  DEBUG_COMPONENT("Lmon_sample_zoom", _Lmon_sample_zoom_var._position_absolute, _Lmon_sample_zoom_var._rotation_absolute);
  instrument->_position_absolute[44] = _Lmon_sample_zoom_var._position_absolute;
  instrument->_position_relative[44] = _Lmon_sample_zoom_var._position_relative;
    _Lmon_sample_zoom_var._position_relative_is_zero =  coords_test_zero(_Lmon_sample_zoom_var._position_relative);
  instrument->counter_N[44]  = instrument->counter_P[44] = instrument->counter_P2[44] = 0;
  instrument->counter_AbsorbProp[44]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0043_Lmon_sample_zoom", _Lmon_sample_zoom_var._position_absolute, _Lmon_sample_zoom_var._rotation_absolute, "L_monitor");
        mccomp_param_nexus(nxhandle,"0043_Lmon_sample_zoom", "nL", "20", "100","int");
        mccomp_param_nexus(nxhandle,"0043_Lmon_sample_zoom", "filename", 0, "LMON_sample_zoom.dat", "char*");
        mccomp_param_nexus(nxhandle,"0043_Lmon_sample_zoom", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0043_Lmon_sample_zoom", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0043_Lmon_sample_zoom", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0043_Lmon_sample_zoom", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0043_Lmon_sample_zoom", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0043_Lmon_sample_zoom", "xwidth", "0", "0.02","MCNUM");
        mccomp_param_nexus(nxhandle,"0043_Lmon_sample_zoom", "yheight", "0", "0.04","MCNUM");
        mccomp_param_nexus(nxhandle,"0043_Lmon_sample_zoom", "Lmin", "NONE", "_instrument_var._parameters.lambda0 -0.1","MCNUM");
        mccomp_param_nexus(nxhandle,"0043_Lmon_sample_zoom", "Lmax", "NONE", "_instrument_var._parameters.lambda0 + 0.1","MCNUM");
        mccomp_param_nexus(nxhandle,"0043_Lmon_sample_zoom", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Lmon_sample_zoom_setpos */

/* component sample=Tunneling_sample() SETTING, POSITION/ROTATION */
int _sample_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_sample_setpos] component sample=Tunneling_sample() SETTING [Tunneling_sample:0]");
  stracpy(_sample_var._name, "sample", 16384);
  stracpy(_sample_var._type, "Tunneling_sample", 16384);
  _sample_var._index=45;
  int current_setpos_index = 45;
  _sample_var._parameters.thickness = 0.01 - _instrument_var._parameters.V_HOLE;
  _sample_var._parameters.radius = 0.01;
  _sample_var._parameters.focus_r = 0;
  _sample_var._parameters.p_interact = 1;
  _sample_var._parameters.f_QE = _instrument_var._parameters.FRAC_QUASIEL;
  _sample_var._parameters.f_tun = _instrument_var._parameters.FRAC_TUNNEL;
  _sample_var._parameters.gamma = _instrument_var._parameters.Gamma;
  _sample_var._parameters.E_tun = _instrument_var._parameters.Etun;
  _sample_var._parameters.target_x = 0;
  _sample_var._parameters.target_y = 0;
  _sample_var._parameters.target_z = 0.235;
  _sample_var._parameters.focus_xw = 0.015;
  _sample_var._parameters.focus_yh = 0.015;
  _sample_var._parameters.focus_aw = 0;
  _sample_var._parameters.focus_ah = 0;
  _sample_var._parameters.xwidth = 0;
  _sample_var._parameters.yheight = 0.04;
  _sample_var._parameters.zdepth = 0;
  _sample_var._parameters.sigma_abs = 5.08;
  _sample_var._parameters.sigma_inc = 4.935;
  _sample_var._parameters.Vc = 13.827;
  _sample_var._parameters.target_index = 2;


  /* component sample=Tunneling_sample() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Fastchop2_var._rotation_absolute, _sample_var._rotation_absolute);
    rot_transpose(_Lmon_sample_zoom_var._rotation_absolute, tr1);
    rot_mul(_sample_var._rotation_absolute, tr1, _sample_var._rotation_relative);
    _sample_var._rotation_is_identity =  rot_test_identity(_sample_var._rotation_relative);
    tc1 = coords_set(
      0, -0.04, _instrument_var._parameters.SAMPLE_DIST);
    rot_transpose(_Fastchop2_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _sample_var._position_absolute = coords_add(_Fastchop2_var._position_absolute, tc2);
    tc1 = coords_sub(_Lmon_sample_zoom_var._position_absolute, _sample_var._position_absolute);
    _sample_var._position_relative = rot_apply(_sample_var._rotation_absolute, tc1);
  } /* sample=Tunneling_sample() AT ROTATED */
  DEBUG_COMPONENT("sample", _sample_var._position_absolute, _sample_var._rotation_absolute);
  instrument->_position_absolute[45] = _sample_var._position_absolute;
  instrument->_position_relative[45] = _sample_var._position_relative;
    _sample_var._position_relative_is_zero =  coords_test_zero(_sample_var._position_relative);
  instrument->counter_N[45]  = instrument->counter_P[45] = instrument->counter_P2[45] = 0;
  instrument->counter_AbsorbProp[45]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0044_sample", _sample_var._position_absolute, _sample_var._rotation_absolute, "Tunneling_sample");
        mccomp_param_nexus(nxhandle,"0044_sample", "thickness", "0", "0.01 - _instrument_var._parameters.V_HOLE","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "radius", "0.01", "0.01","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "focus_r", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "p_interact", "1", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "f_QE", "0", "_instrument_var._parameters.FRAC_QUASIEL","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "f_tun", "0", "_instrument_var._parameters.FRAC_TUNNEL","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "gamma", "0", "_instrument_var._parameters.Gamma","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "E_tun", "0", "_instrument_var._parameters.Etun","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "target_x", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "target_y", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "target_z", "0.235", "0.235","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "focus_xw", "0", "0.015","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "focus_yh", "0", "0.015","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "focus_aw", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "focus_ah", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "xwidth", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "yheight", "0.05", "0.04","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "zdepth", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "sigma_abs", "5.08", "5.08","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "sigma_inc", "4.935", "4.935","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "Vc", "13.827", "13.827","MCNUM");
        mccomp_param_nexus(nxhandle,"0044_sample", "target_index", "0", "2","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _sample_setpos */

/* component detectorarm=Arm() SETTING, POSITION/ROTATION */
int _detectorarm_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_detectorarm_setpos] component detectorarm=Arm() SETTING [Arm:0]");
  stracpy(_detectorarm_var._name, "detectorarm", 16384);
  stracpy(_detectorarm_var._type, "Arm", 16384);
  _detectorarm_var._index=46;
  int current_setpos_index = 46;
  /* component detectorarm=Arm() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (_instrument_var._parameters.TT)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _sample_var._rotation_absolute, _detectorarm_var._rotation_absolute);
    rot_transpose(_sample_var._rotation_absolute, tr1);
    rot_mul(_detectorarm_var._rotation_absolute, tr1, _detectorarm_var._rotation_relative);
    _detectorarm_var._rotation_is_identity =  rot_test_identity(_detectorarm_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0);
    rot_transpose(_sample_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _detectorarm_var._position_absolute = coords_add(_sample_var._position_absolute, tc2);
    tc1 = coords_sub(_sample_var._position_absolute, _detectorarm_var._position_absolute);
    _detectorarm_var._position_relative = rot_apply(_detectorarm_var._rotation_absolute, tc1);
  } /* detectorarm=Arm() AT ROTATED */
  DEBUG_COMPONENT("detectorarm", _detectorarm_var._position_absolute, _detectorarm_var._rotation_absolute);
  instrument->_position_absolute[46] = _detectorarm_var._position_absolute;
  instrument->_position_relative[46] = _detectorarm_var._position_relative;
    _detectorarm_var._position_relative_is_zero =  coords_test_zero(_detectorarm_var._position_relative);
  instrument->counter_N[46]  = instrument->counter_P[46] = instrument->counter_P2[46] = 0;
  instrument->counter_AbsorbProp[46]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0045_detectorarm", _detectorarm_var._position_absolute, _detectorarm_var._rotation_absolute, "Arm");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _detectorarm_setpos */

/* component TOFdetector=TOF_monitor() SETTING, POSITION/ROTATION */
int _TOFdetector_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_TOFdetector_setpos] component TOFdetector=TOF_monitor() SETTING [TOF_monitor:0]");
  stracpy(_TOFdetector_var._name, "TOFdetector", 16384);
  stracpy(_TOFdetector_var._type, "TOF_monitor", 16384);
  _TOFdetector_var._index=47;
  int current_setpos_index = 47;
  _TOFdetector_var._parameters.nt = 512;
  if("TOF.dat" && strlen("TOF.dat"))
    stracpy(_TOFdetector_var._parameters.filename, "TOF.dat" ? "TOF.dat" : "", 16384);
  else 
  _TOFdetector_var._parameters.filename[0]='\0';
  _TOFdetector_var._parameters.xmin = -0.05;
  _TOFdetector_var._parameters.xmax = 0.05;
  _TOFdetector_var._parameters.ymin = -0.05;
  _TOFdetector_var._parameters.ymax = 0.05;
  _TOFdetector_var._parameters.xwidth = 0.015;
  _TOFdetector_var._parameters.yheight = 0.015;
  _TOFdetector_var._parameters.tmin = 0;
  _TOFdetector_var._parameters.tmax = 2e5;
  _TOFdetector_var._parameters.dt = 1.0;
  _TOFdetector_var._parameters.restore_neutron = 0;
  _TOFdetector_var._parameters.nowritefile = 0;


  /* component TOFdetector=TOF_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _detectorarm_var._rotation_absolute, _TOFdetector_var._rotation_absolute);
    rot_transpose(_sample_var._rotation_absolute, tr1);
    rot_mul(_TOFdetector_var._rotation_absolute, tr1, _TOFdetector_var._rotation_relative);
    _TOFdetector_var._rotation_is_identity =  rot_test_identity(_TOFdetector_var._rotation_relative);
    tc1 = coords_set(
      0, 0, _instrument_var._parameters.DETECTOR_DIST);
    rot_transpose(_detectorarm_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _TOFdetector_var._position_absolute = coords_add(_detectorarm_var._position_absolute, tc2);
    tc1 = coords_sub(_sample_var._position_absolute, _TOFdetector_var._position_absolute);
    _TOFdetector_var._position_relative = rot_apply(_TOFdetector_var._rotation_absolute, tc1);
  } /* TOFdetector=TOF_monitor() AT ROTATED */
  DEBUG_COMPONENT("TOFdetector", _TOFdetector_var._position_absolute, _TOFdetector_var._rotation_absolute);
  instrument->_position_absolute[47] = _TOFdetector_var._position_absolute;
  instrument->_position_relative[47] = _TOFdetector_var._position_relative;
    _TOFdetector_var._position_relative_is_zero =  coords_test_zero(_TOFdetector_var._position_relative);
  instrument->counter_N[47]  = instrument->counter_P[47] = instrument->counter_P2[47] = 0;
  instrument->counter_AbsorbProp[47]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0046_TOFdetector", _TOFdetector_var._position_absolute, _TOFdetector_var._rotation_absolute, "TOF_monitor");
        mccomp_param_nexus(nxhandle,"0046_TOFdetector", "nt", "20", "512","int");
        mccomp_param_nexus(nxhandle,"0046_TOFdetector", "filename", 0, "TOF.dat", "char*");
        mccomp_param_nexus(nxhandle,"0046_TOFdetector", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0046_TOFdetector", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0046_TOFdetector", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0046_TOFdetector", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0046_TOFdetector", "xwidth", "0", "0.015","MCNUM");
        mccomp_param_nexus(nxhandle,"0046_TOFdetector", "yheight", "0", "0.015","MCNUM");
        mccomp_param_nexus(nxhandle,"0046_TOFdetector", "tmin", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0046_TOFdetector", "tmax", "0", "2e5","MCNUM");
        mccomp_param_nexus(nxhandle,"0046_TOFdetector", "dt", "1.0", "1.0","MCNUM");
        mccomp_param_nexus(nxhandle,"0046_TOFdetector", "restore_neutron", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0046_TOFdetector", "nowritefile", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _TOFdetector_setpos */

/* component TOFdetector_zoom=TOF_monitor() SETTING, POSITION/ROTATION */
int _TOFdetector_zoom_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_TOFdetector_zoom_setpos] component TOFdetector_zoom=TOF_monitor() SETTING [TOF_monitor:0]");
  stracpy(_TOFdetector_zoom_var._name, "TOFdetector_zoom", 16384);
  stracpy(_TOFdetector_zoom_var._type, "TOF_monitor", 16384);
  _TOFdetector_zoom_var._index=48;
  int current_setpos_index = 48;
  _TOFdetector_zoom_var._parameters.nt = 100;
  if("TOF_zoom.dat" && strlen("TOF_zoom.dat"))
    stracpy(_TOFdetector_zoom_var._parameters.filename, "TOF_zoom.dat" ? "TOF_zoom.dat" : "", 16384);
  else 
  _TOFdetector_zoom_var._parameters.filename[0]='\0';
  _TOFdetector_zoom_var._parameters.xmin = -0.05;
  _TOFdetector_zoom_var._parameters.xmax = 0.05;
  _TOFdetector_zoom_var._parameters.ymin = -0.05;
  _TOFdetector_zoom_var._parameters.ymax = 0.05;
  _TOFdetector_zoom_var._parameters.xwidth = 0.015;
  _TOFdetector_zoom_var._parameters.yheight = 0.015;
  _TOFdetector_zoom_var._parameters.tmin = 1e6 * t_detector -10e2;
  _TOFdetector_zoom_var._parameters.tmax = 1e6 * t_detector + 10e2;
  _TOFdetector_zoom_var._parameters.dt = 1.0;
  _TOFdetector_zoom_var._parameters.restore_neutron = 0;
  _TOFdetector_zoom_var._parameters.nowritefile = 0;


  /* component TOFdetector_zoom=TOF_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _TOFdetector_var._rotation_absolute, _TOFdetector_zoom_var._rotation_absolute);
    rot_transpose(_TOFdetector_var._rotation_absolute, tr1);
    rot_mul(_TOFdetector_zoom_var._rotation_absolute, tr1, _TOFdetector_zoom_var._rotation_relative);
    _TOFdetector_zoom_var._rotation_is_identity =  rot_test_identity(_TOFdetector_zoom_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1e-6);
    rot_transpose(_TOFdetector_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _TOFdetector_zoom_var._position_absolute = coords_add(_TOFdetector_var._position_absolute, tc2);
    tc1 = coords_sub(_TOFdetector_var._position_absolute, _TOFdetector_zoom_var._position_absolute);
    _TOFdetector_zoom_var._position_relative = rot_apply(_TOFdetector_zoom_var._rotation_absolute, tc1);
  } /* TOFdetector_zoom=TOF_monitor() AT ROTATED */
  DEBUG_COMPONENT("TOFdetector_zoom", _TOFdetector_zoom_var._position_absolute, _TOFdetector_zoom_var._rotation_absolute);
  instrument->_position_absolute[48] = _TOFdetector_zoom_var._position_absolute;
  instrument->_position_relative[48] = _TOFdetector_zoom_var._position_relative;
    _TOFdetector_zoom_var._position_relative_is_zero =  coords_test_zero(_TOFdetector_zoom_var._position_relative);
  instrument->counter_N[48]  = instrument->counter_P[48] = instrument->counter_P2[48] = 0;
  instrument->counter_AbsorbProp[48]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0047_TOFdetector_zoom", _TOFdetector_zoom_var._position_absolute, _TOFdetector_zoom_var._rotation_absolute, "TOF_monitor");
        mccomp_param_nexus(nxhandle,"0047_TOFdetector_zoom", "nt", "20", "100","int");
        mccomp_param_nexus(nxhandle,"0047_TOFdetector_zoom", "filename", 0, "TOF_zoom.dat", "char*");
        mccomp_param_nexus(nxhandle,"0047_TOFdetector_zoom", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0047_TOFdetector_zoom", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0047_TOFdetector_zoom", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0047_TOFdetector_zoom", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0047_TOFdetector_zoom", "xwidth", "0", "0.015","MCNUM");
        mccomp_param_nexus(nxhandle,"0047_TOFdetector_zoom", "yheight", "0", "0.015","MCNUM");
        mccomp_param_nexus(nxhandle,"0047_TOFdetector_zoom", "tmin", "0", "1e6 * t_detector -10e2","MCNUM");
        mccomp_param_nexus(nxhandle,"0047_TOFdetector_zoom", "tmax", "0", "1e6 * t_detector + 10e2","MCNUM");
        mccomp_param_nexus(nxhandle,"0047_TOFdetector_zoom", "dt", "1.0", "1.0","MCNUM");
        mccomp_param_nexus(nxhandle,"0047_TOFdetector_zoom", "restore_neutron", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0047_TOFdetector_zoom", "nowritefile", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _TOFdetector_zoom_setpos */

/* component Edetector=E_monitor() SETTING, POSITION/ROTATION */
int _Edetector_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Edetector_setpos] component Edetector=E_monitor() SETTING [E_monitor:0]");
  stracpy(_Edetector_var._name, "Edetector", 16384);
  stracpy(_Edetector_var._type, "E_monitor", 16384);
  _Edetector_var._index=49;
  int current_setpos_index = 49;
  _Edetector_var._parameters.nE = 400;
  if("Edet" && strlen("Edet"))
    stracpy(_Edetector_var._parameters.filename, "Edet" ? "Edet" : "", 16384);
  else 
  _Edetector_var._parameters.filename[0]='\0';
  _Edetector_var._parameters.xmin = -0.05;
  _Edetector_var._parameters.xmax = 0.05;
  _Edetector_var._parameters.ymin = -0.05;
  _Edetector_var._parameters.ymax = 0.05;
  _Edetector_var._parameters.nowritefile = 0;
  _Edetector_var._parameters.xwidth = 0.015;
  _Edetector_var._parameters.yheight = 0.015;
  _Edetector_var._parameters.Emin = E_target - _instrument_var._parameters.RES_DE;
  _Edetector_var._parameters.Emax = E_target + _instrument_var._parameters.RES_DE;
  _Edetector_var._parameters.restore_neutron = 0;


  /* component Edetector=E_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _TOFdetector_zoom_var._rotation_absolute, _Edetector_var._rotation_absolute);
    rot_transpose(_TOFdetector_zoom_var._rotation_absolute, tr1);
    rot_mul(_Edetector_var._rotation_absolute, tr1, _Edetector_var._rotation_relative);
    _Edetector_var._rotation_is_identity =  rot_test_identity(_Edetector_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1e-6);
    rot_transpose(_TOFdetector_zoom_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Edetector_var._position_absolute = coords_add(_TOFdetector_zoom_var._position_absolute, tc2);
    tc1 = coords_sub(_TOFdetector_zoom_var._position_absolute, _Edetector_var._position_absolute);
    _Edetector_var._position_relative = rot_apply(_Edetector_var._rotation_absolute, tc1);
  } /* Edetector=E_monitor() AT ROTATED */
  DEBUG_COMPONENT("Edetector", _Edetector_var._position_absolute, _Edetector_var._rotation_absolute);
  instrument->_position_absolute[49] = _Edetector_var._position_absolute;
  instrument->_position_relative[49] = _Edetector_var._position_relative;
    _Edetector_var._position_relative_is_zero =  coords_test_zero(_Edetector_var._position_relative);
  instrument->counter_N[49]  = instrument->counter_P[49] = instrument->counter_P2[49] = 0;
  instrument->counter_AbsorbProp[49]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0048_Edetector", _Edetector_var._position_absolute, _Edetector_var._rotation_absolute, "E_monitor");
        mccomp_param_nexus(nxhandle,"0048_Edetector", "nE", "20", "400","int");
        mccomp_param_nexus(nxhandle,"0048_Edetector", "filename", 0, "Edet", "char*");
        mccomp_param_nexus(nxhandle,"0048_Edetector", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0048_Edetector", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0048_Edetector", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0048_Edetector", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0048_Edetector", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0048_Edetector", "xwidth", "0", "0.015","MCNUM");
        mccomp_param_nexus(nxhandle,"0048_Edetector", "yheight", "0", "0.015","MCNUM");
        mccomp_param_nexus(nxhandle,"0048_Edetector", "Emin", "NONE", "E_target - _instrument_var._parameters.RES_DE","MCNUM");
        mccomp_param_nexus(nxhandle,"0048_Edetector", "Emax", "NONE", "E_target + _instrument_var._parameters.RES_DE","MCNUM");
        mccomp_param_nexus(nxhandle,"0048_Edetector", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Edetector_setpos */

/* component TOF2Edetector=TOF2E_monitor() SETTING, POSITION/ROTATION */
int _TOF2Edetector_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_TOF2Edetector_setpos] component TOF2Edetector=TOF2E_monitor() SETTING [TOF2E_monitor:0]");
  stracpy(_TOF2Edetector_var._name, "TOF2Edetector", 16384);
  stracpy(_TOF2Edetector_var._type, "TOF2E_monitor", 16384);
  _TOF2Edetector_var._index=50;
  int current_setpos_index = 50;
  _TOF2Edetector_var._parameters.nE = 200;
  if("TOF2E.dat" && strlen("TOF2E.dat"))
    stracpy(_TOF2Edetector_var._parameters.filename, "TOF2E.dat" ? "TOF2E.dat" : "", 16384);
  else 
  _TOF2Edetector_var._parameters.filename[0]='\0';
  _TOF2Edetector_var._parameters.nowritefile = 0;
  _TOF2Edetector_var._parameters.xmin = -0.05;
  _TOF2Edetector_var._parameters.xmax = 0.05;
  _TOF2Edetector_var._parameters.ymin = -0.05;
  _TOF2Edetector_var._parameters.ymax = 0.05;
  _TOF2Edetector_var._parameters.xwidth = 0.015;
  _TOF2Edetector_var._parameters.yheight = 0.015;
  _TOF2Edetector_var._parameters.Emin = E_target - _instrument_var._parameters.RES_DE;
  _TOF2Edetector_var._parameters.Emax = E_target + _instrument_var._parameters.RES_DE;
  _TOF2Edetector_var._parameters.T_zero = t_sample;
  _TOF2Edetector_var._parameters.L_flight = _instrument_var._parameters.DETECTOR_DIST;
  _TOF2Edetector_var._parameters.restore_neutron = 0;


  /* component TOF2Edetector=TOF2E_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Edetector_var._rotation_absolute, _TOF2Edetector_var._rotation_absolute);
    rot_transpose(_Edetector_var._rotation_absolute, tr1);
    rot_mul(_TOF2Edetector_var._rotation_absolute, tr1, _TOF2Edetector_var._rotation_relative);
    _TOF2Edetector_var._rotation_is_identity =  rot_test_identity(_TOF2Edetector_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1e-6);
    rot_transpose(_Edetector_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _TOF2Edetector_var._position_absolute = coords_add(_Edetector_var._position_absolute, tc2);
    tc1 = coords_sub(_Edetector_var._position_absolute, _TOF2Edetector_var._position_absolute);
    _TOF2Edetector_var._position_relative = rot_apply(_TOF2Edetector_var._rotation_absolute, tc1);
  } /* TOF2Edetector=TOF2E_monitor() AT ROTATED */
  DEBUG_COMPONENT("TOF2Edetector", _TOF2Edetector_var._position_absolute, _TOF2Edetector_var._rotation_absolute);
  instrument->_position_absolute[50] = _TOF2Edetector_var._position_absolute;
  instrument->_position_relative[50] = _TOF2Edetector_var._position_relative;
    _TOF2Edetector_var._position_relative_is_zero =  coords_test_zero(_TOF2Edetector_var._position_relative);
  instrument->counter_N[50]  = instrument->counter_P[50] = instrument->counter_P2[50] = 0;
  instrument->counter_AbsorbProp[50]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0049_TOF2Edetector", _TOF2Edetector_var._position_absolute, _TOF2Edetector_var._rotation_absolute, "TOF2E_monitor");
        mccomp_param_nexus(nxhandle,"0049_TOF2Edetector", "nE", "20", "200","int");
        mccomp_param_nexus(nxhandle,"0049_TOF2Edetector", "filename", 0, "TOF2E.dat", "char*");
        mccomp_param_nexus(nxhandle,"0049_TOF2Edetector", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0049_TOF2Edetector", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0049_TOF2Edetector", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0049_TOF2Edetector", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0049_TOF2Edetector", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0049_TOF2Edetector", "xwidth", "0", "0.015","MCNUM");
        mccomp_param_nexus(nxhandle,"0049_TOF2Edetector", "yheight", "0", "0.015","MCNUM");
        mccomp_param_nexus(nxhandle,"0049_TOF2Edetector", "Emin", "NONE", "E_target - _instrument_var._parameters.RES_DE","MCNUM");
        mccomp_param_nexus(nxhandle,"0049_TOF2Edetector", "Emax", "NONE", "E_target + _instrument_var._parameters.RES_DE","MCNUM");
        mccomp_param_nexus(nxhandle,"0049_TOF2Edetector", "T_zero", "NONE", "t_sample","MCNUM");
        mccomp_param_nexus(nxhandle,"0049_TOF2Edetector", "L_flight", "NONE", "_instrument_var._parameters.DETECTOR_DIST","MCNUM");
        mccomp_param_nexus(nxhandle,"0049_TOF2Edetector", "restore_neutron", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _TOF2Edetector_setpos */

_class_ESS_butterfly *class_ESS_butterfly_init(_class_ESS_butterfly *_comp
) {
  #define sector (_comp->_parameters.sector)
  #define beamline (_comp->_parameters.beamline)
  #define yheight (_comp->_parameters.yheight)
  #define cold_frac (_comp->_parameters.cold_frac)
  #define target_index (_comp->_parameters.target_index)
  #define dist (_comp->_parameters.dist)
  #define focus_xw (_comp->_parameters.focus_xw)
  #define focus_yh (_comp->_parameters.focus_yh)
  #define c_performance (_comp->_parameters.c_performance)
  #define t_performance (_comp->_parameters.t_performance)
  #define Lmin (_comp->_parameters.Lmin)
  #define Lmax (_comp->_parameters.Lmax)
  #define tmax_multiplier (_comp->_parameters.tmax_multiplier)
  #define n_pulses (_comp->_parameters.n_pulses)
  #define acc_power (_comp->_parameters.acc_power)
  #define tfocus_dist (_comp->_parameters.tfocus_dist)
  #define tfocus_time (_comp->_parameters.tfocus_time)
  #define tfocus_width (_comp->_parameters.tfocus_width)
  #define ColdWidths (_comp->_parameters.ColdWidths)
  #define ThermalWidths (_comp->_parameters.ThermalWidths)
  #define ColdScalars (_comp->_parameters.ColdScalars)
  #define ThermalScalars (_comp->_parameters.ThermalScalars)
  #define Beamlines (_comp->_parameters.Beamlines)
  #define wfrac_cold (_comp->_parameters.wfrac_cold)
  #define wfrac_thermal (_comp->_parameters.wfrac_thermal)
  #define C1_x (_comp->_parameters.C1_x)
  #define C1_z (_comp->_parameters.C1_z)
  #define C2_x (_comp->_parameters.C2_x)
  #define C2_z (_comp->_parameters.C2_z)
  #define C3_x (_comp->_parameters.C3_x)
  #define C3_z (_comp->_parameters.C3_z)
  #define T1_x (_comp->_parameters.T1_x)
  #define T1_z (_comp->_parameters.T1_z)
  #define T2_x (_comp->_parameters.T2_x)
  #define T2_z (_comp->_parameters.T2_z)
  #define T3_x (_comp->_parameters.T3_x)
  #define T3_z (_comp->_parameters.T3_z)
  #define rC1_x (_comp->_parameters.rC1_x)
  #define rC1_z (_comp->_parameters.rC1_z)
  #define rC2_x (_comp->_parameters.rC2_x)
  #define rC2_z (_comp->_parameters.rC2_z)
  #define rC3_x (_comp->_parameters.rC3_x)
  #define rC3_z (_comp->_parameters.rC3_z)
  #define rT1_x (_comp->_parameters.rT1_x)
  #define rT1_z (_comp->_parameters.rT1_z)
  #define rT2_x (_comp->_parameters.rT2_x)
  #define rT2_z (_comp->_parameters.rT2_z)
  #define rT3_x (_comp->_parameters.rT3_x)
  #define rT3_z (_comp->_parameters.rT3_z)
  #define tx (_comp->_parameters.tx)
  #define ty (_comp->_parameters.ty)
  #define tz (_comp->_parameters.tz)
  #define r11 (_comp->_parameters.r11)
  #define r12 (_comp->_parameters.r12)
  #define r21 (_comp->_parameters.r21)
  #define r22 (_comp->_parameters.r22)
  #define delta_y (_comp->_parameters.delta_y)
  #define Mwidth_c (_comp->_parameters.Mwidth_c)
  #define Mwidth_t (_comp->_parameters.Mwidth_t)
  #define beamportangle (_comp->_parameters.beamportangle)
  #define w_mult (_comp->_parameters.w_mult)
  #define w_stat (_comp->_parameters.w_stat)
  #define w_focus (_comp->_parameters.w_focus)
  #define w_tfocus (_comp->_parameters.w_tfocus)
  #define w_geom_c (_comp->_parameters.w_geom_c)
  #define w_geom_t (_comp->_parameters.w_geom_t)
  #define isleft (_comp->_parameters.isleft)
  #define l_range (_comp->_parameters.l_range)
  #define cos_thermal (_comp->_parameters.cos_thermal)
  #define cos_cold (_comp->_parameters.cos_cold)
  #define orientation_angle (_comp->_parameters.orientation_angle)
  #define cx (_comp->_parameters.cx)
  #define cz (_comp->_parameters.cz)
  #define jmax (_comp->_parameters.jmax)
  #define dxC (_comp->_parameters.dxC)
  #define dxT (_comp->_parameters.dxT)
  SIG_MESSAGE("[_source_init] component source=ESS_butterfly() INITIALISE [ESS_butterfly:0]");



  int sign_bl_angle;

  /* Oversampling for widths plus fraction of moderator surface "not around the corner" */
  double oversampT = 1.1;
  double oversampC = 1.0;

  /* variables needed to correct for the emission surface angle */
  double internal_angle;
  double cos_beamport_angle, sin_beamport_angle;

  if (beamline < 4) {
    wfrac_cold = 1.0;
    wfrac_thermal = (1 - 0.072);
  } else {
    wfrac_cold = 1.0;
    wfrac_thermal = 1.0;
  }

  /* Centering-parameters, which sector are we in? */
  if (strcasestr (sector, "N")) {
    cx = 0.117;
    cz = 0.0;
    sign_bl_angle = 1;
    orientation_angle = BeamlinesN[beamline - 1];
    Beamlines = BeamlinesN;
    internal_angle = 90 - fabs (orientation_angle);
    beamportangle = nearest_angle (fabs (internal_angle));
    /* Direction-cosines for use with e.g. Brilliance_monitor */
    cos_beamport_angle = cos (fabs (internal_angle) * DEG2RAD);
    sin_beamport_angle = sin (fabs (internal_angle) * DEG2RAD);
    /* correction for projection along the beam / projection on the z=0 plane */
    cos_thermal = cos_beamport_angle;
    cos_cold = cos ((fabs (internal_angle) - 24.24) * DEG2RAD);
    ColdWidths = ColdWidthNE;
    ThermalWidths = ThermalWidthNE;
    int j;
    for (j = 0; j < 11; j++) {
      ColdScalars[j] = ColdScalarsN[j];
      ThermalScalars[j] = ThermalScalarsN[j];
    }
    jmax = 10;
    T1_x = 0;
    T1_z = 0;
    T2_x = -wfrac_thermal * oversampT * ThermalWidths[beamline - 1] / cos_thermal;
    T2_z = 0;
    T3_x = ((1 - wfrac_thermal) * oversampT * ThermalWidths[beamline - 1] / cos_thermal);
    T3_z = 0;
    C1_x = 0;
    C1_z = 0;
    C2_x = (wfrac_cold * oversampC * ColdWidths[beamline - 1] / cos_cold) * cos (24.24 * DEG2RAD);
    C2_z = -(wfrac_cold * oversampC * ColdWidths[beamline - 1] / cos_cold) * sin (24.24 * DEG2RAD);
    C3_x = -(1 - wfrac_cold) * oversampC * ColdWidths[beamline - 1] / cos_thermal;
    C3_z = 0;
    isleft = 1;
  } else if (strcasestr (sector, "W")) {
    cx = 0.0;
    cz = 0.0;
    sign_bl_angle = -1;
    orientation_angle = BeamlinesW[beamline - 1];
    Beamlines = BeamlinesW;
    internal_angle = 90 - fabs (orientation_angle);
    beamportangle = nearest_angle (fabs (internal_angle));
    /* Direction-cosines for use with e.g. Brilliance_monitor */
    cos_beamport_angle = cos (fabs (internal_angle) * DEG2RAD);
    sin_beamport_angle = sin (fabs (internal_angle) * DEG2RAD);
    /* correction for projection along the beam / projection on the z=0 plane */
    cos_thermal = cos_beamport_angle;
    cos_cold = cos ((fabs (internal_angle) - 24.24) * DEG2RAD);
    ColdWidths = ColdWidthSW;
    ThermalWidths = ThermalWidthSW;
    int j;
    for (j = 0; j < 11; j++) {
      ColdScalars[j] = ColdScalarsW[j];
      ThermalScalars[j] = ThermalScalarsW[j];
    }
    jmax = 11;
    T1_x = 0;
    T1_z = 0;
    T2_x = wfrac_thermal * oversampT * ThermalWidths[beamline - 1] / cos_thermal;
    T2_z = 0;
    T3_x = -((1 - wfrac_thermal) * oversampT * ThermalWidths[beamline - 1] / cos_thermal);
    T3_z = 0;
    C1_x = 0;
    C1_z = 0;
    C2_x = -(wfrac_cold * oversampC * ColdWidths[beamline - 1] / cos_cold) * cos (24.24 * DEG2RAD);
    C2_z = -(wfrac_cold * oversampC * ColdWidths[beamline - 1] / cos_cold) * sin (24.24 * DEG2RAD);
    C3_x = (1 - wfrac_cold) * oversampC * ColdWidths[beamline - 1] / cos_thermal;
    C3_z = 0;
    isleft = -1;
  } else if (strcasestr (sector, "S")) {
    cx = 0.0;
    cz = -0.185;
    sign_bl_angle = 1;
    orientation_angle = BeamlinesS[beamline - 1];
    Beamlines = BeamlinesS;
    internal_angle = 90 - fabs (orientation_angle);
    beamportangle = nearest_angle (fabs (internal_angle));
    /* Direction-cosines for use with e.g. Brilliance_monitor */
    cos_beamport_angle = cos (fabs (internal_angle) * DEG2RAD);
    sin_beamport_angle = sin (fabs (internal_angle) * DEG2RAD);
    /* correction for projection along the beam / projection on the z=0 plane */
    cos_thermal = cos_beamport_angle;
    cos_cold = cos ((fabs (internal_angle) - 24.24) * DEG2RAD);

    ColdWidths = ColdWidthSW;
    ThermalWidths = ThermalWidthSW;
    int j;
    for (j = 0; j < 11; j++) {
      ColdScalars[j] = ColdScalarsS[j];
      ThermalScalars[j] = ThermalScalarsS[j];
    }
    jmax = 11;
    T1_x = 0;
    T1_z = 0;
    T2_x = wfrac_thermal * oversampT * ThermalWidths[beamline - 1] / cos_thermal;
    T2_z = 0;
    T3_x = -((1 - wfrac_thermal) * oversampT * ThermalWidths[beamline - 1] / cos_thermal);
    T3_z = 0;
    C1_x = 0;
    C1_z = 0;
    C2_x = -(wfrac_cold * oversampC * ColdWidths[beamline - 1] / cos_cold) * cos (24.24 * DEG2RAD);
    C2_z = (wfrac_cold * oversampC * ColdWidths[beamline - 1] / cos_cold) * sin (24.24 * DEG2RAD);
    C3_x = (1 - wfrac_cold) * oversampC * ColdWidths[beamline - 1] / cos_thermal;
    C3_z = 0;
    isleft = -1;
  } else if (strcasestr (sector, "E")) {
    cx = 0.117;
    cz = -0.185;
    sign_bl_angle = -1;
    orientation_angle = BeamlinesE[beamline - 1];
    Beamlines = BeamlinesE;
    internal_angle = 90 - fabs (orientation_angle);
    beamportangle = nearest_angle (fabs (internal_angle));
    /* Direction-cosines for use with e.g. Brilliance_monitor */
    cos_beamport_angle = cos (fabs (internal_angle) * DEG2RAD);
    sin_beamport_angle = sin (fabs (internal_angle) * DEG2RAD);
    /* correction for projection along the beam / projection on the z=0 plane */
    cos_thermal = cos_beamport_angle;
    cos_cold = cos ((fabs (internal_angle) - 24.24) * DEG2RAD);
    ColdWidths = ColdWidthNE;
    ThermalWidths = ThermalWidthNE;
    int j;
    for (j = 0; j < 11; j++) {
      ColdScalars[j] = ColdScalarsE[j];
      ThermalScalars[j] = ThermalScalarsE[j];
    }
    jmax = 10;
    T1_x = 0;
    T1_z = 0;
    T2_x = -wfrac_thermal * oversampT * ThermalWidths[beamline - 1] / cos_thermal;
    T2_z = 0;
    T3_x = ((1 - wfrac_thermal) * oversampT * ThermalWidths[beamline - 1] / cos_thermal);
    T3_z = 0;
    C1_x = 0;
    C1_z = 0;
    C2_x = (wfrac_cold * oversampC * ColdWidths[beamline - 1] / cos_cold) * cos (24.24 * DEG2RAD);
    C2_z = (wfrac_cold * oversampC * ColdWidths[beamline - 1] / cos_cold) * sin (24.24 * DEG2RAD);
    C3_x = -(1 - wfrac_cold) * oversampC * ColdWidths[beamline - 1] / cos_thermal;
    C3_z = 0;
    isleft = 1;
  } else {
    MPI_MASTER (fprintf (stderr, "%s: Sector %s is undefined, please use N, W, S or E!\n", NAME_CURRENT_COMP, sector););
    exit (-1);
  }
  if (beamline > jmax || beamline <= 0) {
    MPI_MASTER (fprintf (stderr, "%s: beamline no %i is undefined in sector %s, please use 1 <= beamline <= %i\n", NAME_CURRENT_COMP, beamline, sector, jmax););
    exit (-1);
  }

  MPI_MASTER (printf ("%s: Setting up for sector %s, beamline %i, global orientation angle is %g, internal angle %g\n", NAME_CURRENT_COMP, sector, beamline,
                      orientation_angle, beamportangle););
  if (c_performance <= 0) {
    fprintf (stderr, "%s: Cold performance scalar of %g is not allowed. Please select 0 < c_performance\n", NAME_CURRENT_COMP, c_performance);
    exit (-1);
  }
  if (t_performance <= 0) {
    MPI_MASTER (fprintf (stderr, "%s: Thermal performance scalar of %g is not allowed. Please select 0 < t_performance\n", NAME_CURRENT_COMP, t_performance););
    exit (-1);
  }
  if (Lmin >= Lmax || Lmin <= 0 || Lmax < 0) {
    MPI_MASTER (fprintf (stderr, "%s: Unmeaningful definition of wavelength range!\nPlease select Lmin, Lmax > 0 and Lmax > Lmin.\n ERROR - Exiting\n",
                         NAME_CURRENT_COMP););
    exit (-1);
  }
  /* Figure out where to aim */
  if (target_index && !dist) {
    Coords ToTarget;
    ToTarget = coords_sub (POS_A_COMP_INDEX (INDEX_CURRENT_COMP + target_index), POS_A_CURRENT_COMP);
    ToTarget = rot_apply (ROT_A_CURRENT_COMP, ToTarget);
    coords_get (ToTarget, &tx, &ty, &tz);
    dist = sqrt (tx * tx + ty * ty + tz * tz);
  } else if (!target_index && !dist) {
    MPI_MASTER (fprintf (stderr, "%s: Please choose to set either the dist parameter or specify a target_index.\nExit\n", NAME_CURRENT_COMP););
    exit (-1);
  } else {
    tx = 0;
    ty = 0;
    tz = dist;
  }
  MPI_MASTER (printf ("%s: Focusing at rectagle sized %g x %g \n  - positioned at location (x,y,z)=(%g m, %g m, %g m) \n", NAME_CURRENT_COMP, focus_xw, focus_yh,
                      tx, ty, tz););
  if (target_index) {
    MPI_MASTER (printf (" ( from target_index %i -> distance %g )\n", target_index, dist););
  } else {
    MPI_MASTER (printf (" ( from dist parameter -> distance %g )\n", dist););
  }
  MPI_MASTER (printf ("%s: Cold and Thermal brilliance performance multiplicators are c_performance=%g and t_performance=%g\n", NAME_CURRENT_COMP, c_performance,
                      t_performance););
  /* Calculate orientation matrix for the display and calculations */
  r11 = cos (DEG2RAD * orientation_angle);
  r12 = -sin (DEG2RAD * orientation_angle);
  r21 = sin (DEG2RAD * orientation_angle);
  r22 = cos (DEG2RAD * orientation_angle);

  /* Rotated corrdinates of the emission areas */
  rC1_x = r11 * C1_z + r12 * C1_x;
  rC1_z = r21 * C1_z + r22 * C1_x;
  rC2_x = r11 * C2_z + r12 * C2_x;
  rC2_z = r21 * C2_z + r22 * C2_x;
  rC3_x = r11 * C3_z + r12 * C3_x;
  rC3_z = r21 * C3_z + r22 * C3_x;
  rT1_x = r11 * T1_z + r12 * T1_x;
  rT1_z = r21 * T1_z + r22 * T1_x;
  rT2_x = r11 * T2_z + r12 * T2_x;
  rT2_z = r21 * T2_z + r22 * T2_x;
  rT3_x = r11 * T3_z + r12 * T3_x;
  rT3_z = r21 * T3_z + r22 * T3_x;
  /* Moderator half-height */
  delta_y = yheight / 2.0;
  /* Other moderator parms */
  /* "Measured" moderator widths in cm scale */
  Mwidth_c = 100.0 * ColdWidths[beamline - 1] / cos_cold;
  Mwidth_t = (100.0 * ThermalWidths[beamline - 1] + 0.7) / cos_thermal;

  if (tfocus_width && tfocus_time && tfocus_dist) {
    MPI_MASTER (printf ("%s: Using time focusing: Directing neutrons to this time-window:\n   tfocus_width (%g s) wide at tfocus_time (%g s), tfocus_dist (%g m) "
                        "downstream\n",
                        NAME_CURRENT_COMP, tfocus_width, tfocus_time, tfocus_dist););
  } else if (!tfocus_width && !tfocus_time && !tfocus_dist) {
    MPI_MASTER (printf ("%s: NOT using time focusing\n", NAME_CURRENT_COMP););
  } else {
    MPI_MASTER (fprintf (stderr,
                         "%s: Unmeaningful combination tfocus_width (%g s), tfocus_time (%g s) and tfocus_dist (%g m): \n    All must be either==0 (no time "
                         "focusing) or !=0 (time focusing)\n ERROR - Exiting\n",
                         NAME_CURRENT_COMP, tfocus_width, tfocus_time, tfocus_dist););
    exit (-1);
  }

  l_range = Lmax - Lmin;
  /* Weight multipliers */
  w_mult = acc_power / 5;
  w_stat = 1.0 / mcget_ncount ();
  w_geom_c = 0.072 * yheight * 1.0e4; /* source area correction */
  w_geom_t = 0.108 * yheight * 1.0e4;
  w_mult *= l_range; /* wavelength range correction */
  n_pulses = (double)floor (n_pulses);
  if (n_pulses == 0)
    n_pulses = 1;

  dxC = dxCold[beamline - 1];
  dxT = dxThermal[beamline - 1];
  #undef sector
  #undef beamline
  #undef yheight
  #undef cold_frac
  #undef target_index
  #undef dist
  #undef focus_xw
  #undef focus_yh
  #undef c_performance
  #undef t_performance
  #undef Lmin
  #undef Lmax
  #undef tmax_multiplier
  #undef n_pulses
  #undef acc_power
  #undef tfocus_dist
  #undef tfocus_time
  #undef tfocus_width
  #undef ColdWidths
  #undef ThermalWidths
  #undef ColdScalars
  #undef ThermalScalars
  #undef Beamlines
  #undef wfrac_cold
  #undef wfrac_thermal
  #undef C1_x
  #undef C1_z
  #undef C2_x
  #undef C2_z
  #undef C3_x
  #undef C3_z
  #undef T1_x
  #undef T1_z
  #undef T2_x
  #undef T2_z
  #undef T3_x
  #undef T3_z
  #undef rC1_x
  #undef rC1_z
  #undef rC2_x
  #undef rC2_z
  #undef rC3_x
  #undef rC3_z
  #undef rT1_x
  #undef rT1_z
  #undef rT2_x
  #undef rT2_z
  #undef rT3_x
  #undef rT3_z
  #undef tx
  #undef ty
  #undef tz
  #undef r11
  #undef r12
  #undef r21
  #undef r22
  #undef delta_y
  #undef Mwidth_c
  #undef Mwidth_t
  #undef beamportangle
  #undef w_mult
  #undef w_stat
  #undef w_focus
  #undef w_tfocus
  #undef w_geom_c
  #undef w_geom_t
  #undef isleft
  #undef l_range
  #undef cos_thermal
  #undef cos_cold
  #undef orientation_angle
  #undef cx
  #undef cz
  #undef jmax
  #undef dxC
  #undef dxT
  return(_comp);
} /* class_ESS_butterfly_init */

_class_Progress_bar *class_Progress_bar_init(_class_Progress_bar *_comp
) {
  #define profile (_comp->_parameters.profile)
  #define percent (_comp->_parameters.percent)
  #define flag_save (_comp->_parameters.flag_save)
  #define minutes (_comp->_parameters.minutes)
  #define IntermediateCnts (_comp->_parameters.IntermediateCnts)
  #define StartTime (_comp->_parameters.StartTime)
  #define EndTime (_comp->_parameters.EndTime)
  #define CurrentTime (_comp->_parameters.CurrentTime)
  #define infostring (_comp->_parameters.infostring)
  SIG_MESSAGE("[_Origin_init] component Origin=Progress_bar() INITIALISE [Progress_bar:0]");

  IntermediateCnts = 0;
  StartTime = 0;
  EndTime = 0;
  CurrentTime = 0;

  fprintf (stdout, "[%s] Initialize\n", instrument_name);
  if (percent * mcget_ncount () / 100 < 1e5) {
    percent = 1e5 * 100.0 / mcget_ncount ();
  }
  #ifdef OPENACC
  time (&StartTime);
  #endif

  #ifdef USE_MPI
  sprintf (infostring, "(%i MPI processes) ", mpi_node_count);
  #else
  sprintf (infostring, "(single process) ");
  #endif
  #undef profile
  #undef percent
  #undef flag_save
  #undef minutes
  #undef IntermediateCnts
  #undef StartTime
  #undef EndTime
  #undef CurrentTime
  #undef infostring
  return(_comp);
} /* class_Progress_bar_init */

_class_TOF_monitor *class_TOF_monitor_init(_class_TOF_monitor *_comp
) {
  #define nt (_comp->_parameters.nt)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define tmin (_comp->_parameters.tmin)
  #define tmax (_comp->_parameters.tmax)
  #define dt (_comp->_parameters.dt)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define TOF_N (_comp->_parameters.TOF_N)
  #define TOF_p (_comp->_parameters.TOF_p)
  #define TOF_p2 (_comp->_parameters.TOF_p2)
  #define t_min (_comp->_parameters.t_min)
  #define t_max (_comp->_parameters.t_max)
  #define delta_t (_comp->_parameters.delta_t)
  SIG_MESSAGE("[_TOFmoderator_zoom_init] component TOFmoderator_zoom=TOF_monitor() INITIALISE [TOF_monitor:0]");

  if (xwidth > 0) {
    xmax = xwidth / 2;
    xmin = -xmax;
  }
  if (yheight > 0) {
    ymax = yheight / 2;
    ymin = -ymax;
  }

  if ((xmin >= xmax) || (ymin >= ymax)) {
    printf ("TOF_monitor: %s: Null detection area !\n"
            "ERROR        (xwidth,yheight,xmin,xmax,ymin,ymax). Exiting",
            NAME_CURRENT_COMP);
    exit (0);
  }

  TOF_N = create_darr1d (nt);
  TOF_p = create_darr1d (nt);
  TOF_p2 = create_darr1d (nt);

  if (tmax != 0) {
    t_max = tmax;
    t_min = tmin;
    delta_t = (t_max - t_min) / nt;
  } else {
    delta_t = dt;
    t_min = 0;
    t_max = nt * dt + tmin;
  }

  // Use instance name for monitor output if no input was given
  if (!strcmp (filename, "\0"))
    sprintf (filename, "%s", NAME_CURRENT_COMP);
  #undef nt
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef tmin
  #undef tmax
  #undef dt
  #undef restore_neutron
  #undef nowritefile
  #undef TOF_N
  #undef TOF_p
  #undef TOF_p2
  #undef t_min
  #undef t_max
  #undef delta_t
  return(_comp);
} /* class_TOF_monitor_init */

_class_L_monitor *class_L_monitor_init(_class_L_monitor *_comp
) {
  #define nL (_comp->_parameters.nL)
  #define filename (_comp->_parameters.filename)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Lmin (_comp->_parameters.Lmin)
  #define Lmax (_comp->_parameters.Lmax)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define L_N (_comp->_parameters.L_N)
  #define L_p (_comp->_parameters.L_p)
  #define L_p2 (_comp->_parameters.L_p2)
  SIG_MESSAGE("[_Lmon_guistart_init] component Lmon_guistart=L_monitor() INITIALISE [L_monitor:0]");

  if (xwidth > 0) {
    xmax = xwidth / 2;
    xmin = -xmax;
  }
  if (yheight > 0) {
    ymax = yheight / 2;
    ymin = -ymax;
  }

  if ((xmin >= xmax) || (ymin >= ymax)) {
    printf ("L_monitor: %s: Null detection area !\n"
            "ERROR      (xwidth,yheight,xmin,xmax,ymin,ymax). Exiting",
            NAME_CURRENT_COMP);
    exit (0);
  }

  L_N = create_darr1d (nL);
  L_p = create_darr1d (nL);
  L_p2 = create_darr1d (nL);

  // Use instance name for monitor output if no input was given
  if (!strcmp (filename, "\0"))
    sprintf (filename, "%s", NAME_CURRENT_COMP);
  #undef nL
  #undef filename
  #undef nowritefile
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef Lmin
  #undef Lmax
  #undef restore_neutron
  #undef L_N
  #undef L_p
  #undef L_p2
  return(_comp);
} /* class_L_monitor_init */

_class_Guide *class_Guide_init(_class_Guide *_comp
) {
  #define reflect (_comp->_parameters.reflect)
  #define w1 (_comp->_parameters.w1)
  #define h1 (_comp->_parameters.h1)
  #define w2 (_comp->_parameters.w2)
  #define h2 (_comp->_parameters.h2)
  #define l (_comp->_parameters.l)
  #define R0 (_comp->_parameters.R0)
  #define Qc (_comp->_parameters.Qc)
  #define alpha (_comp->_parameters.alpha)
  #define m (_comp->_parameters.m)
  #define W (_comp->_parameters.W)
  #define pTable (_comp->_parameters.pTable)
  #define table_present (_comp->_parameters.table_present)
  SIG_MESSAGE("[_Guide1_init] component Guide1=Guide() INITIALISE [Guide:0]");

  if (mcgravitation)
    fprintf (stderr,
             "WARNING: Guide: %s: "
             "This component produces wrong results with gravitation !\n"
             "Use Guide_gravity.\n",
             NAME_CURRENT_COMP);

  if (!w2)
    w2 = w1;
  if (!h2)
    h2 = h1;

  if (reflect && strlen (reflect) && strcmp (reflect, "NULL") && strcmp (reflect, "0")) {
    if (Table_Read (&pTable, reflect, 1) <= 0) /* read 1st block data from file into pTable */
      exit (fprintf (stderr, "Guide: %s: can not read file %s\n", NAME_CURRENT_COMP, reflect));
    table_present = 1;
  } else {
    table_present = 0;
    if (W < 0 || R0 < 0 || Qc < 0 || m < 0) {
      fprintf (stderr, "Guide: %s: W R0 Qc must be >0.\n", NAME_CURRENT_COMP);
      exit (-1);
    }
  }
  #undef reflect
  #undef w1
  #undef h1
  #undef w2
  #undef h2
  #undef l
  #undef R0
  #undef Qc
  #undef alpha
  #undef m
  #undef W
  #undef pTable
  #undef table_present
  return(_comp);
} /* class_Guide_init */

_class_PSD_monitor *class_PSD_monitor_init(_class_PSD_monitor *_comp
) {
  #define nx (_comp->_parameters.nx)
  #define ny (_comp->_parameters.ny)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define PSD_N (_comp->_parameters.PSD_N)
  #define PSD_p (_comp->_parameters.PSD_p)
  #define PSD_p2 (_comp->_parameters.PSD_p2)
  SIG_MESSAGE("[_PSDslow1_init] component PSDslow1=PSD_monitor() INITIALISE [PSD_monitor:0]");

  if (xwidth > 0) {
    xmax = xwidth / 2;
    xmin = -xmax;
  }
  if (yheight > 0) {
    ymax = yheight / 2;
    ymin = -ymax;
  }

  if ((xmin >= xmax) || (ymin >= ymax)) {
    printf ("PSD_monitor: %s: Null detection area !\n"
            "ERROR        (xwidth,yheight,xmin,xmax,ymin,ymax). Exiting",
            NAME_CURRENT_COMP);
    exit (0);
  }

  PSD_N = create_darr2d (nx, ny);
  PSD_p = create_darr2d (nx, ny);
  PSD_p2 = create_darr2d (nx, ny);

  // Use instance name for monitor output if no input was given
  if (!strcmp (filename, "\0"))
    sprintf (filename, "%s", NAME_CURRENT_COMP);
  #undef nx
  #undef ny
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef restore_neutron
  #undef nowritefile
  #undef PSD_N
  #undef PSD_p
  #undef PSD_p2
  return(_comp);
} /* class_PSD_monitor_init */

_class_DiskChopper *class_DiskChopper_init(_class_DiskChopper *_comp
) {
  #define theta_0 (_comp->_parameters.theta_0)
  #define radius (_comp->_parameters.radius)
  #define yheight (_comp->_parameters.yheight)
  #define nu (_comp->_parameters.nu)
  #define nslit (_comp->_parameters.nslit)
  #define jitter (_comp->_parameters.jitter)
  #define delay (_comp->_parameters.delay)
  #define isfirst (_comp->_parameters.isfirst)
  #define n_pulse (_comp->_parameters.n_pulse)
  #define abs_out (_comp->_parameters.abs_out)
  #define phase (_comp->_parameters.phase)
  #define xwidth (_comp->_parameters.xwidth)
  #define verbose (_comp->_parameters.verbose)
  #define Tg (_comp->_parameters.Tg)
  #define To (_comp->_parameters.To)
  #define delta_y (_comp->_parameters.delta_y)
  #define height (_comp->_parameters.height)
  #define omega (_comp->_parameters.omega)
  SIG_MESSAGE("[_FOchop1_init] component FOchop1=DiskChopper() INITIALISE [DiskChopper:0]");

  /* If slit height 'unset', assume full opening */
  if (yheight == 0) {
    height = radius;
  } else {
    height = yheight;
  }
  delta_y = radius - height / 2; /* radius at beam center */
  omega = 2.0 * PI * nu;         /* rad/s */
  if (xwidth && !theta_0 && radius)
    theta_0 = 2 * RAD2DEG * asin (xwidth / 2 / delta_y);

  if (nslit <= 0 || theta_0 <= 0 || radius <= 0) {
    fprintf (stderr, "DiskChopper: %s: nslit, theta_0 and radius must be > 0\n", NAME_CURRENT_COMP);
    exit (-1);
  }
  if (nslit * theta_0 >= 360) {
    fprintf (stderr, "DiskChopper: %s: nslit * theta_0 exceeds 2PI\n", NAME_CURRENT_COMP);
    exit (-1);
  }
  if (yheight && yheight > radius) {
    fprintf (stderr, "DiskChopper: %s: yheight must be < radius\n", NAME_CURRENT_COMP);
    exit (-1);
  }
  if (isfirst && n_pulse <= 0) {
    fprintf (stderr, "DiskChopper: %s: wrong First chopper pulse number (n_pulse=%g)\n", NAME_CURRENT_COMP, n_pulse);
    exit (-1);
  }
  if (!omega) {
    fprintf (stderr, "DiskChopper: %s WARNING: chopper frequency is 0!\n", NAME_CURRENT_COMP);
    omega = 1e-15; /* We should actually use machine epsilon here... */
  }
  if (!abs_out) {
    fprintf (stderr, "DiskChopper: %s WARNING: chopper will NOT absorb neutrons outside radius %g [m]\n", NAME_CURRENT_COMP, radius);
  }

  theta_0 *= DEG2RAD;

  /* Calulate delay from phase and vice versa */
  if (phase) {
    if (delay) {
      fprintf (stderr, "DiskChopper: %s WARNING: delay AND phase specified. Using phase setting\n", NAME_CURRENT_COMP);
    }
    phase *= DEG2RAD;
    /* 'Delay' should always be a delay, taking rotation direction into account: */
    delay = phase / fabs (omega);
  } else {
    phase = delay * omega; /* rad */
  }

  /* Time from opening of slit to next opening of slit */
  Tg = 2.0 * PI / fabs (omega) / nslit;

  /* How long can neutrons pass the Chopper at a single point */
  To = theta_0 / fabs (omega);

  if (!xwidth)
    xwidth = 2 * delta_y * sin (theta_0 / 2);

  if (verbose && nu) {
    printf ("DiskChopper: %s: frequency=%g [Hz] %g [rpm], time frame=%g [s] phase=%g [deg]\n", NAME_CURRENT_COMP, nu, nu * 60, Tg, phase * RAD2DEG);
    printf ("             %g slits, angle=%g [deg] height=%g [m], width=%g [m] at radius=%g [m]\n", nslit, theta_0 * RAD2DEG, height, xwidth, delta_y);
  }
  #undef theta_0
  #undef radius
  #undef yheight
  #undef nu
  #undef nslit
  #undef jitter
  #undef delay
  #undef isfirst
  #undef n_pulse
  #undef abs_out
  #undef phase
  #undef xwidth
  #undef verbose
  #undef Tg
  #undef To
  #undef delta_y
  #undef height
  #undef omega
  return(_comp);
} /* class_DiskChopper_init */

_class_TOFLambda_monitor *class_TOFLambda_monitor_init(_class_TOFLambda_monitor *_comp
) {
  #define nowritefile (_comp->_parameters.nowritefile)
  #define nL (_comp->_parameters.nL)
  #define nt (_comp->_parameters.nt)
  #define tmin (_comp->_parameters.tmin)
  #define tmax (_comp->_parameters.tmax)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Lmin (_comp->_parameters.Lmin)
  #define Lmax (_comp->_parameters.Lmax)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define TOFL_N (_comp->_parameters.TOFL_N)
  #define TOFL_p (_comp->_parameters.TOFL_p)
  #define TOFL_p2 (_comp->_parameters.TOFL_p2)
  #define tt_0 (_comp->_parameters.tt_0)
  #define tt_1 (_comp->_parameters.tt_1)
  SIG_MESSAGE("[_TOFLmon1_init] component TOFLmon1=TOFLambda_monitor() INITIALISE [TOFLambda_monitor:0]");

  if (xwidth > 0) {
    xmax = xwidth / 2;
    xmin = -xmax;
  }
  if (yheight > 0) {
    ymax = yheight / 2;
    ymin = -ymax;
  }

  if ((xmin >= xmax) || (ymin >= ymax)) {
    printf ("TOFlambda_monitor: %s: Null detection area !\n"
            "ERROR              (xwidth,yheight,xmin,xmax,ymin,ymax). Exiting",
            NAME_CURRENT_COMP);
    exit (0);
  }

  TOFL_N = create_darr2d (nt, nL);
  TOFL_p = create_darr2d (nt, nL);
  TOFL_p2 = create_darr2d (nt, nL);
  tt_0 = tmin * 1e-6;
  tt_1 = tmax * 1e-6;

  // Use instance name for monitor output if no input was given
  if (!strcmp (filename, "\0"))
    sprintf (filename, "%s", NAME_CURRENT_COMP);
  #undef nowritefile
  #undef nL
  #undef nt
  #undef tmin
  #undef tmax
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef Lmin
  #undef Lmax
  #undef restore_neutron
  #undef TOFL_N
  #undef TOFL_p
  #undef TOFL_p2
  #undef tt_0
  #undef tt_1
  return(_comp);
} /* class_TOFLambda_monitor_init */

_class_E_monitor *class_E_monitor_init(_class_E_monitor *_comp
) {
  #define nE (_comp->_parameters.nE)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Emin (_comp->_parameters.Emin)
  #define Emax (_comp->_parameters.Emax)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define E_N (_comp->_parameters.E_N)
  #define E_p (_comp->_parameters.E_p)
  #define E_p2 (_comp->_parameters.E_p2)
  #define S_p (_comp->_parameters.S_p)
  #define S_pE (_comp->_parameters.S_pE)
  #define S_pE2 (_comp->_parameters.S_pE2)
  SIG_MESSAGE("[_Esample_init] component Esample=E_monitor() INITIALISE [E_monitor:0]");

  if (xwidth > 0) {
    xmax = xwidth / 2;
    xmin = -xmax;
  }
  if (yheight > 0) {
    ymax = yheight / 2;
    ymin = -ymax;
  }

  if ((xmin >= xmax) || (ymin >= ymax)) {
    printf ("E_monitor: %s: Null detection area !\n"
            "ERROR      (xwidth,yheight,xmin,xmax,ymin,ymax). Exiting",
            NAME_CURRENT_COMP);
    exit (0);
  }

  E_N = create_darr1d (nE);
  E_p = create_darr1d (nE);
  E_p2 = create_darr1d (nE);

  S_p = S_pE = S_pE2 = 0;

  // Use instance name for monitor output if no input was given
  if (!strcmp (filename, "\0"))
    sprintf (filename, "%s", NAME_CURRENT_COMP);
  #undef nE
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef nowritefile
  #undef xwidth
  #undef yheight
  #undef Emin
  #undef Emax
  #undef restore_neutron
  #undef E_N
  #undef E_p
  #undef E_p2
  #undef S_p
  #undef S_pE
  #undef S_pE2
  return(_comp);
} /* class_E_monitor_init */

_class_Tunneling_sample *class_Tunneling_sample_init(_class_Tunneling_sample *_comp
) {
  #define thickness (_comp->_parameters.thickness)
  #define radius (_comp->_parameters.radius)
  #define focus_r (_comp->_parameters.focus_r)
  #define p_interact (_comp->_parameters.p_interact)
  #define f_QE (_comp->_parameters.f_QE)
  #define f_tun (_comp->_parameters.f_tun)
  #define gamma (_comp->_parameters.gamma)
  #define E_tun (_comp->_parameters.E_tun)
  #define target_x (_comp->_parameters.target_x)
  #define target_y (_comp->_parameters.target_y)
  #define target_z (_comp->_parameters.target_z)
  #define focus_xw (_comp->_parameters.focus_xw)
  #define focus_yh (_comp->_parameters.focus_yh)
  #define focus_aw (_comp->_parameters.focus_aw)
  #define focus_ah (_comp->_parameters.focus_ah)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define zdepth (_comp->_parameters.zdepth)
  #define sigma_abs (_comp->_parameters.sigma_abs)
  #define sigma_inc (_comp->_parameters.sigma_inc)
  #define Vc (_comp->_parameters.Vc)
  #define target_index (_comp->_parameters.target_index)
  #define VarsV (_comp->_parameters.VarsV)
  #define ftun (_comp->_parameters.ftun)
  #define fQE (_comp->_parameters.fQE)
  SIG_MESSAGE("[_sample_init] component sample=Tunneling_sample() INITIALISE [Tunneling_sample:0]");

  if (!xwidth || !yheight || !zdepth) /* Cannot define a rectangle */
    if (!radius || !yheight)          /* Cannot define a cylinder either */
      exit (fprintf (stderr, "V_sample: %s: sample has no volume (zero dimensions)\n", NAME_CURRENT_COMP));
    else /* It is a cylinder */
      VarsV.isrect = 0;
  else /* It is a rectangle */
    VarsV.isrect = 1;

  VarsV.sigma_a = sigma_abs;
  VarsV.sigma_i = sigma_inc;
  VarsV.rho = (1 / Vc);
  VarsV.my_s = (VarsV.rho * 100 * VarsV.sigma_i);
  VarsV.my_a_v = (VarsV.rho * 100 * VarsV.sigma_a);

  /* now compute target coords if a component index is supplied */
  VarsV.tx = VarsV.ty = VarsV.tz = 0;
  if (target_index) {
    Coords ToTarget;
    ToTarget = coords_sub (POS_A_COMP_INDEX (INDEX_CURRENT_COMP + target_index), POS_A_CURRENT_COMP);
    ToTarget = rot_apply (ROT_A_CURRENT_COMP, ToTarget);
    coords_get (ToTarget, &VarsV.tx, &VarsV.ty, &VarsV.tz);
  } else {
    VarsV.tx = target_x;
    VarsV.ty = target_y;
    VarsV.tz = target_z;
  }

  if (!(VarsV.tx || VarsV.ty || VarsV.tz))
    printf ("Tunneling_sample: %s: The target is not defined. Using direct beam (Z-axis).\n", NAME_CURRENT_COMP);

  VarsV.distance = sqrt (VarsV.tx * VarsV.tx + VarsV.ty * VarsV.ty + VarsV.tz * VarsV.tz);

  /* different ways of setting rectangular area */
  VarsV.aw = VarsV.ah = 0;
  if (focus_xw) {
    VarsV.xw = focus_xw;
  }
  if (focus_yh) {
    VarsV.yh = focus_yh;
  }
  if (focus_aw) {
    VarsV.aw = DEG2RAD * focus_aw;
  }
  if (focus_ah) {
    VarsV.ah = DEG2RAD * focus_ah;
  }

  /* Check that probabilities are positive and do not exceed unity */
  if (f_tun < 0)
    ftun = 0;
  else
    ftun = f_tun;
  if (f_QE < 0)
    fQE = 0;
  else
    fQE = f_QE;
  if ((ftun + fQE) > 1) {
    ftun = 0;
    printf ("Tunneling_sample: Sum of inelastic probabilities > 1. Setting f_tun=0");
    if (fQE > 1) {
      fQE = 0;
      printf ("Tunneling_sample: Probability fQE > 1. Setting fQE=0.");
    }
  }
  #undef thickness
  #undef radius
  #undef focus_r
  #undef p_interact
  #undef f_QE
  #undef f_tun
  #undef gamma
  #undef E_tun
  #undef target_x
  #undef target_y
  #undef target_z
  #undef focus_xw
  #undef focus_yh
  #undef focus_aw
  #undef focus_ah
  #undef xwidth
  #undef yheight
  #undef zdepth
  #undef sigma_abs
  #undef sigma_inc
  #undef Vc
  #undef target_index
  #undef VarsV
  #undef ftun
  #undef fQE
  return(_comp);
} /* class_Tunneling_sample_init */

_class_TOF2E_monitor *class_TOF2E_monitor_init(_class_TOF2E_monitor *_comp
) {
  #define nE (_comp->_parameters.nE)
  #define filename (_comp->_parameters.filename)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Emin (_comp->_parameters.Emin)
  #define Emax (_comp->_parameters.Emax)
  #define T_zero (_comp->_parameters.T_zero)
  #define L_flight (_comp->_parameters.L_flight)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define E_N (_comp->_parameters.E_N)
  #define E_p (_comp->_parameters.E_p)
  #define E_p2 (_comp->_parameters.E_p2)
  #define S_p (_comp->_parameters.S_p)
  #define S_pE (_comp->_parameters.S_pE)
  #define S_pE2 (_comp->_parameters.S_pE2)
  SIG_MESSAGE("[_TOF2Edetector_init] component TOF2Edetector=TOF2E_monitor() INITIALISE [TOF2E_monitor:0]");

  if (xwidth > 0) {
    xmax = xwidth / 2;
    xmin = -xmax;
  }
  if (yheight > 0) {
    ymax = yheight / 2;
    ymin = -ymax;
  }

  if ((xmin >= xmax) || (ymin >= ymax)) {
    printf ("E_monitor: %s: Null detection area !\n"
            "ERROR      (xwidth,yheight,xmin,xmax,ymin,ymax). Exiting",
            NAME_CURRENT_COMP);
    exit (0);
  }

  E_N = create_darr1d (nE);
  E_p = create_darr1d (nE);
  E_p2 = create_darr1d (nE);

  S_p = S_pE = S_pE2 = 0;

  // Use instance name for monitor output if no input was given
  if (!strcmp (filename, "\0"))
    sprintf (filename, "%s", NAME_CURRENT_COMP);
  #undef nE
  #undef filename
  #undef nowritefile
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef Emin
  #undef Emax
  #undef T_zero
  #undef L_flight
  #undef restore_neutron
  #undef E_N
  #undef E_p
  #undef E_p2
  #undef S_p
  #undef S_pE
  #undef S_pE2
  return(_comp);
} /* class_TOF2E_monitor_init */



int init(void) { /* called by mccode_main for ESS_IN5_reprate:INITIALISE */
  DEBUG_INSTR();
  // Initialise rng
  srandom(_hash(mcseed-1));

  /* code_main/parseoptions/readparams sets instrument parameters value */
  stracpy(instrument->_name, "ESS_IN5_reprate", 256);

  /* Instrument 'ESS_IN5_reprate' INITIALISE */
  SIG_MESSAGE("[ESS_IN5_reprate] INITIALISE [(null):-1]");
  #define Lmin (instrument->_parameters.Lmin)
  #define Lmax (instrument->_parameters.Lmax)
  #define lambda0 (instrument->_parameters.lambda0)
  #define Pulse_width (instrument->_parameters.Pulse_width)
  #define Num_pulses (instrument->_parameters.Num_pulses)
  #define GUI_start (instrument->_parameters.GUI_start)
  #define FO1_DIST (instrument->_parameters.FO1_DIST)
  #define L_ballistic_begin (instrument->_parameters.L_ballistic_begin)
  #define L_ballistic_end (instrument->_parameters.L_ballistic_end)
  #define Length (instrument->_parameters.Length)
  #define SAMPLE_DIST (instrument->_parameters.SAMPLE_DIST)
  #define DETECTOR_DIST (instrument->_parameters.DETECTOR_DIST)
  #define GUI_h (instrument->_parameters.GUI_h)
  #define GUI_w (instrument->_parameters.GUI_w)
  #define GUI_GAP (instrument->_parameters.GUI_GAP)
  #define H1 (instrument->_parameters.H1)
  #define W1 (instrument->_parameters.W1)
  #define H2 (instrument->_parameters.H2)
  #define W2 (instrument->_parameters.W2)
  #define H3 (instrument->_parameters.H3)
  #define W3 (instrument->_parameters.W3)
  #define H4 (instrument->_parameters.H4)
  #define W4 (instrument->_parameters.W4)
  #define H_chop (instrument->_parameters.H_chop)
  #define W_chop (instrument->_parameters.W_chop)
  #define H_end (instrument->_parameters.H_end)
  #define W_end (instrument->_parameters.W_end)
  #define ALPHA (instrument->_parameters.ALPHA)
  #define M (instrument->_parameters.M)
  #define F_slow1 (instrument->_parameters.F_slow1)
  #define F_slow2 (instrument->_parameters.F_slow2)
  #define F_fast1 (instrument->_parameters.F_fast1)
  #define F_fast2 (instrument->_parameters.F_fast2)
  #define N_fast (instrument->_parameters.N_fast)
  #define SLOW1_THETA (instrument->_parameters.SLOW1_THETA)
  #define FO3 (instrument->_parameters.FO3)
  #define THETA_fast1 (instrument->_parameters.THETA_fast1)
  #define FAST_THETA (instrument->_parameters.FAST_THETA)
  #define Gamma (instrument->_parameters.Gamma)
  #define Etun (instrument->_parameters.Etun)
  #define V_HOLE (instrument->_parameters.V_HOLE)
  #define FRAC_QUASIEL (instrument->_parameters.FRAC_QUASIEL)
  #define FRAC_TUNNEL (instrument->_parameters.FRAC_TUNNEL)
  #define TT (instrument->_parameters.TT)
  #define RES_DE (instrument->_parameters.RES_DE)
  #define cold (instrument->_parameters.cold)
{
        FREQ = ESS_SOURCE_FREQUENCY;
        t_offset=Pulse_width/2.0+170e-6;
        t_FO1 = FO1_DIST*lambda0/(2*PI*K2V)+t_offset;
        t_FO2 = Length/2*lambda0/(2*PI*K2V)+t_offset;
        t_fast1 = (Length/2+0.1)*lambda0/(2*PI*K2V)+t_offset;
        t_fast2 = Length*lambda0/(2*PI*K2V)+t_offset;
        t_fast2a = (Length+GUI_GAP)*lambda0/(2*PI*K2V)+t_offset;
        t_fast3 = (Length+2*GUI_GAP)*lambda0/(2*PI*K2V)+t_offset;
        t_sample = (Length+SAMPLE_DIST)*lambda0/(2*PI*K2V)+t_offset;
        t_detector = (Length+SAMPLE_DIST+DETECTOR_DIST)*lambda0/(2*PI*K2V)+t_offset;
        tmin_zoom = t_fast2*1e6-1e3;
        tmax_zoom = t_fast2*1e6+1e3;
        E_target = VS2E*(K2V*2*PI/lambda0)*(K2V*2*PI/lambda0);
}
  #undef Lmin
  #undef Lmax
  #undef lambda0
  #undef Pulse_width
  #undef Num_pulses
  #undef GUI_start
  #undef FO1_DIST
  #undef L_ballistic_begin
  #undef L_ballistic_end
  #undef Length
  #undef SAMPLE_DIST
  #undef DETECTOR_DIST
  #undef GUI_h
  #undef GUI_w
  #undef GUI_GAP
  #undef H1
  #undef W1
  #undef H2
  #undef W2
  #undef H3
  #undef W3
  #undef H4
  #undef W4
  #undef H_chop
  #undef W_chop
  #undef H_end
  #undef W_end
  #undef ALPHA
  #undef M
  #undef F_slow1
  #undef F_slow2
  #undef F_fast1
  #undef F_fast2
  #undef N_fast
  #undef SLOW1_THETA
  #undef FO3
  #undef THETA_fast1
  #undef FAST_THETA
  #undef Gamma
  #undef Etun
  #undef V_HOLE
  #undef FRAC_QUASIEL
  #undef FRAC_TUNNEL
  #undef TT
  #undef RES_DE
  #undef cold
  _source_setpos(); /* type ESS_butterfly */
  _Origin_setpos(); /* type Progress_bar */
  _TOFmoderator_zoom_setpos(); /* type TOF_monitor */
  _TOFmoderator_setpos(); /* type TOF_monitor */
  _Lmon_guistart_setpos(); /* type L_monitor */
  _Lmon_normalize_setpos(); /* type L_monitor */
  _Guide1_setpos(); /* type Guide */
  _Lmonslow1_setpos(); /* type L_monitor */
  _PSDslow1_setpos(); /* type PSD_monitor */
  _FOchop1_setpos(); /* type DiskChopper */
  _TOFLmon1_setpos(); /* type TOFLambda_monitor */
  _Lmon_afterslow1_setpos(); /* type L_monitor */
  _PSD_afterslow1_setpos(); /* type PSD_monitor */
  _Guidelong1_setpos(); /* type Guide */
  _Guidelong1b_setpos(); /* type Guide */
  _Lmon_slow2_setpos(); /* type L_monitor */
  _FOchop2_setpos(); /* type DiskChopper */
  _Fastchop1_setpos(); /* type DiskChopper */
  _PSD_afterslow2_setpos(); /* type PSD_monitor */
  _Lmon_afterslow2_setpos(); /* type L_monitor */
  _TOFL_afterslow2_setpos(); /* type TOFLambda_monitor */
  _Guidelong2_setpos(); /* type Guide */
  _Lmon_beforeballistic_setpos(); /* type L_monitor */
  _PSD_beforeballistic_setpos(); /* type PSD_monitor */
  _Guidelong2a_setpos(); /* type Guide */
  _Lmonfast2_setpos(); /* type L_monitor */
  _Lmonfast2_zoom_setpos(); /* type L_monitor */
  _TOFLfast2_setpos(); /* type TOFLambda_monitor */
  _TOFLfast2zoom_setpos(); /* type TOFLambda_monitor */
  _PSDfast2_setpos(); /* type PSD_monitor */
  _Fastchop2_setpos(); /* type DiskChopper */
  _Fastchop2counter_setpos(); /* type DiskChopper */
  _FOchop3_setpos(); /* type DiskChopper */
  _TOFfast2_zoom_setpos(); /* type TOF_monitor */
  _Lmon_afterfast2_setpos(); /* type L_monitor */
  _TOFL_afterfast2_setpos(); /* type TOFLambda_monitor */
  _TOFL_afterfast2_zoom_setpos(); /* type TOFLambda_monitor */
  _PSD_afterfast2_setpos(); /* type PSD_monitor */
  _Guidesample_setpos(); /* type Guide */
  _Lmon_guideend_setpos(); /* type L_monitor */
  _PSDsample_setpos(); /* type PSD_monitor */
  _TOFsample_zoom_setpos(); /* type TOF_monitor */
  _Esample_setpos(); /* type E_monitor */
  _Lmon_sample_zoom_setpos(); /* type L_monitor */
  _sample_setpos(); /* type Tunneling_sample */
  _detectorarm_setpos(); /* type Arm */
  _TOFdetector_setpos(); /* type TOF_monitor */
  _TOFdetector_zoom_setpos(); /* type TOF_monitor */
  _Edetector_setpos(); /* type E_monitor */
  _TOF2Edetector_setpos(); /* type TOF2E_monitor */

  /* call iteratively all components INITIALISE */
  class_ESS_butterfly_init(&_source_var);

  class_Progress_bar_init(&_Origin_var);

  class_TOF_monitor_init(&_TOFmoderator_zoom_var);

  class_TOF_monitor_init(&_TOFmoderator_var);

  class_L_monitor_init(&_Lmon_guistart_var);

  class_L_monitor_init(&_Lmon_normalize_var);

  class_Guide_init(&_Guide1_var);

  class_L_monitor_init(&_Lmonslow1_var);

  class_PSD_monitor_init(&_PSDslow1_var);

  class_DiskChopper_init(&_FOchop1_var);

  class_TOFLambda_monitor_init(&_TOFLmon1_var);

  class_L_monitor_init(&_Lmon_afterslow1_var);

  class_PSD_monitor_init(&_PSD_afterslow1_var);

  class_Guide_init(&_Guidelong1_var);

  class_Guide_init(&_Guidelong1b_var);

  class_L_monitor_init(&_Lmon_slow2_var);

  class_DiskChopper_init(&_FOchop2_var);

  class_DiskChopper_init(&_Fastchop1_var);

  class_PSD_monitor_init(&_PSD_afterslow2_var);

  class_L_monitor_init(&_Lmon_afterslow2_var);

  class_TOFLambda_monitor_init(&_TOFL_afterslow2_var);

  class_Guide_init(&_Guidelong2_var);

  class_L_monitor_init(&_Lmon_beforeballistic_var);

  class_PSD_monitor_init(&_PSD_beforeballistic_var);

  class_Guide_init(&_Guidelong2a_var);

  class_L_monitor_init(&_Lmonfast2_var);

  class_L_monitor_init(&_Lmonfast2_zoom_var);

  class_TOFLambda_monitor_init(&_TOFLfast2_var);

  class_TOFLambda_monitor_init(&_TOFLfast2zoom_var);

  class_PSD_monitor_init(&_PSDfast2_var);

  class_DiskChopper_init(&_Fastchop2_var);

  class_DiskChopper_init(&_Fastchop2counter_var);

  class_DiskChopper_init(&_FOchop3_var);

  class_TOF_monitor_init(&_TOFfast2_zoom_var);

  class_L_monitor_init(&_Lmon_afterfast2_var);

  class_TOFLambda_monitor_init(&_TOFL_afterfast2_var);

  class_TOFLambda_monitor_init(&_TOFL_afterfast2_zoom_var);

  class_PSD_monitor_init(&_PSD_afterfast2_var);

  class_Guide_init(&_Guidesample_var);

  class_L_monitor_init(&_Lmon_guideend_var);

  class_PSD_monitor_init(&_PSDsample_var);

  class_TOF_monitor_init(&_TOFsample_zoom_var);

  class_E_monitor_init(&_Esample_var);

  class_L_monitor_init(&_Lmon_sample_zoom_var);

  class_Tunneling_sample_init(&_sample_var);


  class_TOF_monitor_init(&_TOFdetector_var);

  class_TOF_monitor_init(&_TOFdetector_zoom_var);

  class_E_monitor_init(&_Edetector_var);

  class_TOF2E_monitor_init(&_TOF2Edetector_var);

  if (mcdotrace) display();
  DEBUG_INSTR_END();

#ifdef OPENACC
#include <openacc.h>
#pragma acc update device(_source_var)
#pragma acc update device(_Origin_var)
#pragma acc update device(_TOFmoderator_zoom_var)
#pragma acc update device(_TOFmoderator_var)
#pragma acc update device(_Lmon_guistart_var)
#pragma acc update device(_Lmon_normalize_var)
#pragma acc update device(_Guide1_var)
#pragma acc update device(_Lmonslow1_var)
#pragma acc update device(_PSDslow1_var)
#pragma acc update device(_FOchop1_var)
#pragma acc update device(_TOFLmon1_var)
#pragma acc update device(_Lmon_afterslow1_var)
#pragma acc update device(_PSD_afterslow1_var)
#pragma acc update device(_Guidelong1_var)
#pragma acc update device(_Guidelong1b_var)
#pragma acc update device(_Lmon_slow2_var)
#pragma acc update device(_FOchop2_var)
#pragma acc update device(_Fastchop1_var)
#pragma acc update device(_PSD_afterslow2_var)
#pragma acc update device(_Lmon_afterslow2_var)
#pragma acc update device(_TOFL_afterslow2_var)
#pragma acc update device(_Guidelong2_var)
#pragma acc update device(_Lmon_beforeballistic_var)
#pragma acc update device(_PSD_beforeballistic_var)
#pragma acc update device(_Guidelong2a_var)
#pragma acc update device(_Lmonfast2_var)
#pragma acc update device(_Lmonfast2_zoom_var)
#pragma acc update device(_TOFLfast2_var)
#pragma acc update device(_TOFLfast2zoom_var)
#pragma acc update device(_PSDfast2_var)
#pragma acc update device(_Fastchop2_var)
#pragma acc update device(_Fastchop2counter_var)
#pragma acc update device(_FOchop3_var)
#pragma acc update device(_TOFfast2_zoom_var)
#pragma acc update device(_Lmon_afterfast2_var)
#pragma acc update device(_TOFL_afterfast2_var)
#pragma acc update device(_TOFL_afterfast2_zoom_var)
#pragma acc update device(_PSD_afterfast2_var)
#pragma acc update device(_Guidesample_var)
#pragma acc update device(_Lmon_guideend_var)
#pragma acc update device(_PSDsample_var)
#pragma acc update device(_TOFsample_zoom_var)
#pragma acc update device(_Esample_var)
#pragma acc update device(_Lmon_sample_zoom_var)
#pragma acc update device(_sample_var)
#pragma acc update device(_detectorarm_var)
#pragma acc update device(_TOFdetector_var)
#pragma acc update device(_TOFdetector_zoom_var)
#pragma acc update device(_Edetector_var)
#pragma acc update device(_TOF2Edetector_var)
#pragma acc update device(_instrument_var)
#endif

  return(0);
} /* init */

/*******************************************************************************
* components TRACE
*******************************************************************************/

#define x (_particle->x)
#define y (_particle->y)
#define z (_particle->z)
#define vx (_particle->vx)
#define vy (_particle->vy)
#define vz (_particle->vz)
#define t (_particle->t)
#define sx (_particle->sx)
#define sy (_particle->sy)
#define sz (_particle->sz)
#define p (_particle->p)
#define mcgravitation (_particle->mcgravitation)
#define mcMagnet (_particle->mcMagnet)
#define allow_backprop (_particle->allow_backprop)
#define _mctmp_a (_particle->_mctmp_a)
#define _mctmp_b (_particle->_mctmp_b)
#define _mctmp_c (_particle->_mctmp_c)
/* if on GPU, globally nullify sprintf,fprintf,printfs   */
/* (Similar defines are available in each comp trace but */
/*  those are not enough to handle external libs etc. )  */
#ifdef OPENACC
#define fprintf(stderr,...) printf(__VA_ARGS__)
#define sprintf(string,...) printf(__VA_ARGS__)
#define exit(...) noprintf()
#define strcmp(a,b) str_comp(a,b)
#define strlen(a) str_len(a)
#endif
#define SCATTERED (_particle->_scattered)
#define RESTORE (_particle->_restore)
#define RESTORE_NEUTRON(_index, ...) _particle->_restore = _index;
#define ABSORB0 do { DEBUG_STATE(); DEBUG_ABSORB(); MAGNET_OFF; ABSORBED++; return; } while(0)
#define ABSORBED (_particle->_absorbed)
#define mcget_run_num() _particle->_uid
#define ABSORB ABSORB0
#pragma acc routine
void class_ESS_butterfly_trace(_class_ESS_butterfly *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define sector (_comp->_parameters.sector)
  #define beamline (_comp->_parameters.beamline)
  #define yheight (_comp->_parameters.yheight)
  #define cold_frac (_comp->_parameters.cold_frac)
  #define target_index (_comp->_parameters.target_index)
  #define dist (_comp->_parameters.dist)
  #define focus_xw (_comp->_parameters.focus_xw)
  #define focus_yh (_comp->_parameters.focus_yh)
  #define c_performance (_comp->_parameters.c_performance)
  #define t_performance (_comp->_parameters.t_performance)
  #define Lmin (_comp->_parameters.Lmin)
  #define Lmax (_comp->_parameters.Lmax)
  #define tmax_multiplier (_comp->_parameters.tmax_multiplier)
  #define n_pulses (_comp->_parameters.n_pulses)
  #define acc_power (_comp->_parameters.acc_power)
  #define tfocus_dist (_comp->_parameters.tfocus_dist)
  #define tfocus_time (_comp->_parameters.tfocus_time)
  #define tfocus_width (_comp->_parameters.tfocus_width)
  #define ColdWidths (_comp->_parameters.ColdWidths)
  #define ThermalWidths (_comp->_parameters.ThermalWidths)
  #define ColdScalars (_comp->_parameters.ColdScalars)
  #define ThermalScalars (_comp->_parameters.ThermalScalars)
  #define Beamlines (_comp->_parameters.Beamlines)
  #define wfrac_cold (_comp->_parameters.wfrac_cold)
  #define wfrac_thermal (_comp->_parameters.wfrac_thermal)
  #define C1_x (_comp->_parameters.C1_x)
  #define C1_z (_comp->_parameters.C1_z)
  #define C2_x (_comp->_parameters.C2_x)
  #define C2_z (_comp->_parameters.C2_z)
  #define C3_x (_comp->_parameters.C3_x)
  #define C3_z (_comp->_parameters.C3_z)
  #define T1_x (_comp->_parameters.T1_x)
  #define T1_z (_comp->_parameters.T1_z)
  #define T2_x (_comp->_parameters.T2_x)
  #define T2_z (_comp->_parameters.T2_z)
  #define T3_x (_comp->_parameters.T3_x)
  #define T3_z (_comp->_parameters.T3_z)
  #define rC1_x (_comp->_parameters.rC1_x)
  #define rC1_z (_comp->_parameters.rC1_z)
  #define rC2_x (_comp->_parameters.rC2_x)
  #define rC2_z (_comp->_parameters.rC2_z)
  #define rC3_x (_comp->_parameters.rC3_x)
  #define rC3_z (_comp->_parameters.rC3_z)
  #define rT1_x (_comp->_parameters.rT1_x)
  #define rT1_z (_comp->_parameters.rT1_z)
  #define rT2_x (_comp->_parameters.rT2_x)
  #define rT2_z (_comp->_parameters.rT2_z)
  #define rT3_x (_comp->_parameters.rT3_x)
  #define rT3_z (_comp->_parameters.rT3_z)
  #define tx (_comp->_parameters.tx)
  #define ty (_comp->_parameters.ty)
  #define tz (_comp->_parameters.tz)
  #define r11 (_comp->_parameters.r11)
  #define r12 (_comp->_parameters.r12)
  #define r21 (_comp->_parameters.r21)
  #define r22 (_comp->_parameters.r22)
  #define delta_y (_comp->_parameters.delta_y)
  #define Mwidth_c (_comp->_parameters.Mwidth_c)
  #define Mwidth_t (_comp->_parameters.Mwidth_t)
  #define beamportangle (_comp->_parameters.beamportangle)
  #define w_mult (_comp->_parameters.w_mult)
  #define w_stat (_comp->_parameters.w_stat)
  #define w_focus (_comp->_parameters.w_focus)
  #define w_tfocus (_comp->_parameters.w_tfocus)
  #define w_geom_c (_comp->_parameters.w_geom_c)
  #define w_geom_t (_comp->_parameters.w_geom_t)
  #define isleft (_comp->_parameters.isleft)
  #define l_range (_comp->_parameters.l_range)
  #define cos_thermal (_comp->_parameters.cos_thermal)
  #define cos_cold (_comp->_parameters.cos_cold)
  #define orientation_angle (_comp->_parameters.orientation_angle)
  #define cx (_comp->_parameters.cx)
  #define cz (_comp->_parameters.cz)
  #define jmax (_comp->_parameters.jmax)
  #define dxC (_comp->_parameters.dxC)
  #define dxT (_comp->_parameters.dxT)
  SIG_MESSAGE("[_source_trace] component source=ESS_butterfly() TRACE [ESS_butterfly:0]");

  double xtmp;
  int iscold;
  double x0, z0;
  int surf_sign;
  double cos_factor;
  double w_geom;
  double xf, yf, zf;
  double dx, dy, dz;
  double k, v, r, lambda;
  double dt = 0;
  double modX, modY;
  #ifdef ADJUSTABLE_ESS_SOURCE
  #define ESS_SOURCE_DURATION _instrument_var._parameters.pulse_duration
  #endif

  /* Cold or thermal event? */
  p = 1;
  xtmp = rand01 ();
  y = randpm1 () * delta_y;
  modY = y;
  if (rand01 () < cold_frac) {
    iscold = 1;
    if (rand01 () < wfrac_cold) { // "Broad face"
      x = rC1_x + (rC2_x - rC1_x) * xtmp;
      z = rC1_z + (rC2_z - rC1_z) * xtmp;
      x0 = C1_x + (C2_x - C1_x) * xtmp;
      z0 = C1_z + (C2_z - C1_z) * xtmp;
      surf_sign = -1;
      cos_factor = cos_cold;
    } else {
      x = rC1_x + (rC3_x - rC1_x) * xtmp;
      z = rC1_z + (rC3_z - rC1_z) * xtmp;
      x0 = C1_x + (C3_x - C1_x) * xtmp;
      z0 = C1_z + (C3_z - C1_z) * xtmp;
      surf_sign = 1;
      cos_factor = cos_thermal;
    }
    modX = ((-1.0 * isleft * x0) - dxC);
    w_geom = w_geom_c;
  } else {
    iscold = 0;
    if (rand01 () < wfrac_thermal) { // "Broad face"
      x = rT1_x + (rT2_x - rT1_x) * xtmp;
      z = rT1_z + (rT2_z - rT1_z) * xtmp;
      x0 = T1_x + (T2_x - T1_x) * xtmp;
      z0 = T1_z + (T2_z - T1_z) * xtmp;
      surf_sign = 1;
      cos_factor = cos_thermal;
    } else {
      x = rT1_x + (rT3_x - rT1_x) * xtmp;
      z = rT1_z + (rT3_z - rT1_z) * xtmp;
      x0 = T1_x + (T3_x - T1_x) * xtmp;
      z0 = T1_z + (T3_z - T1_z) * xtmp;
      surf_sign = -1;
      cos_factor = cos_thermal;
    }
    modX = ((-1.0 * isleft * x0) + dxT);
    w_geom = w_geom_t;
  }

  SCATTER;
  /* Where are we going? */
  randvec_target_rect_real (&xf, &yf, &zf, NULL, tx, ty, tz, focus_xw, focus_yh, ROT_A_CURRENT_COMP, x, y, z, 0);

  w_focus = focus_xw * focus_yh / (tx * tx + ty * ty + tz * tz);

  dx = xf - x;
  dy = yf - y;
  dz = zf - z;
  r = sqrt (dx * dx + dy * dy + dz * dz);

  lambda = Lmin + l_range * rand01 (); /* Choose from uniform distribution */

  k = 2 * PI / lambda;
  v = K2V * k;

  vz = v * dz / r;
  vy = v * dy / r;
  vx = v * dx / r;

  /* Are we using time focusing? */
  if (tfocus_width > 0) {
    dt = tfocus_dist / vz;
    t = tfocus_time - dt; /* Set time to hit time window center */
    t += randpm1 () * tfocus_width / 2.0;
    if (t < 0)
      ABSORB; /* Kill neutron if outside pulse duration */
    if (t > tmax_multiplier * ESS_SOURCE_DURATION)
      ABSORB;
    w_tfocus = tfocus_width / (tmax_multiplier * ESS_SOURCE_DURATION);
  } else {
    /* Simple, random wavelength @ random time */
    t = rand01 () * tmax_multiplier * ESS_SOURCE_DURATION;
    w_tfocus = 1;
  }

  if (iscold) { // case: cold moderator
    /* Apply simple engineering reality correction */
    ESS_2015_Schoenfeldt_cold (&t, &p, lambda, tfocus_width, tfocus_time, dt, yheight, Mwidth_t, yheight, Mwidth_c, tmax_multiplier, beamportangle, modX, modY);
    p *= c_performance;
    p *= ColdScalars[beamline - 1];
  } else { // case: thermal moderator
    ESS_2015_Schoenfeldt_thermal (&t, &p, lambda, tfocus_width, tfocus_time, dt, yheight, Mwidth_t, yheight, Mwidth_c, tmax_multiplier, beamportangle, modX,
                                  modY);
    p *= t_performance;
    p *= ThermalScalars[beamline - 1];
  }
  p *= w_stat * w_focus * w_geom * w_mult * w_tfocus;
  t += (double)floor ((n_pulses)*rand01 ()) / ESS_SOURCE_FREQUENCY; /* Select a random pulse */
  p *= cos_factor;
  /* Correct weight for sampling of cold vs. thermal events. */
  if (iscold) {
    p /= cold_frac;
  } else {
    p /= (1 - cold_frac);
  }
  SCATTER;
#ifndef NOABSORB_INF_NAN
  /* Check for nan or inf particle parms */ 
  if(isnan(p + t + vx + vy + vz + x + y + z)) ABSORB;
  if(isinf(fabs(p) + fabs(t) + fabs(vx) + fabs(vy) + fabs(vz) + fabs(x) + fabs(y) + fabs(z))) ABSORB;
#else
  if(isnan(p)  ||  isinf(p)) printf("NAN or INF found in p,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(t)  ||  isinf(t)) printf("NAN or INF found in t,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vx) || isinf(vx)) printf("NAN or INF found in vx, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vy) || isinf(vy)) printf("NAN or INF found in vy, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vz) || isinf(vz)) printf("NAN or INF found in vz, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(x)  ||  isinf(x)) printf("NAN or INF found in x,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(y)  ||  isinf(y)) printf("NAN or INF found in y,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(z)  ||  isinf(z)) printf("NAN or INF found in z,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
#endif
  #undef sector
  #undef beamline
  #undef yheight
  #undef cold_frac
  #undef target_index
  #undef dist
  #undef focus_xw
  #undef focus_yh
  #undef c_performance
  #undef t_performance
  #undef Lmin
  #undef Lmax
  #undef tmax_multiplier
  #undef n_pulses
  #undef acc_power
  #undef tfocus_dist
  #undef tfocus_time
  #undef tfocus_width
  #undef ColdWidths
  #undef ThermalWidths
  #undef ColdScalars
  #undef ThermalScalars
  #undef Beamlines
  #undef wfrac_cold
  #undef wfrac_thermal
  #undef C1_x
  #undef C1_z
  #undef C2_x
  #undef C2_z
  #undef C3_x
  #undef C3_z
  #undef T1_x
  #undef T1_z
  #undef T2_x
  #undef T2_z
  #undef T3_x
  #undef T3_z
  #undef rC1_x
  #undef rC1_z
  #undef rC2_x
  #undef rC2_z
  #undef rC3_x
  #undef rC3_z
  #undef rT1_x
  #undef rT1_z
  #undef rT2_x
  #undef rT2_z
  #undef rT3_x
  #undef rT3_z
  #undef tx
  #undef ty
  #undef tz
  #undef r11
  #undef r12
  #undef r21
  #undef r22
  #undef delta_y
  #undef Mwidth_c
  #undef Mwidth_t
  #undef beamportangle
  #undef w_mult
  #undef w_stat
  #undef w_focus
  #undef w_tfocus
  #undef w_geom_c
  #undef w_geom_t
  #undef isleft
  #undef l_range
  #undef cos_thermal
  #undef cos_cold
  #undef orientation_angle
  #undef cx
  #undef cz
  #undef jmax
  #undef dxC
  #undef dxT
  return;
} /* class_ESS_butterfly_trace */

#pragma acc routine
void class_Progress_bar_trace(_class_Progress_bar *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define profile (_comp->_parameters.profile)
  #define percent (_comp->_parameters.percent)
  #define flag_save (_comp->_parameters.flag_save)
  #define minutes (_comp->_parameters.minutes)
  #define IntermediateCnts (_comp->_parameters.IntermediateCnts)
  #define StartTime (_comp->_parameters.StartTime)
  #define EndTime (_comp->_parameters.EndTime)
  #define CurrentTime (_comp->_parameters.CurrentTime)
  #define infostring (_comp->_parameters.infostring)
  SIG_MESSAGE("[_Origin_trace] component Origin=Progress_bar() TRACE [Progress_bar:0]");

  #ifndef OPENACC
  double ncount;
  ncount = mcget_run_num ();
  if (!StartTime) {
    time (&StartTime); /* compute starting time */
    IntermediateCnts = 1e3;
  }
  time_t NowTime;
  time (&NowTime);
  /* compute initial estimate of computation duration */
  if (!EndTime && ncount >= IntermediateCnts) {
    CurrentTime = NowTime;
    if (difftime (NowTime, StartTime) > 10 && ncount) { /* wait 10 sec before writing ETA */
      EndTime = StartTime + (time_t)(difftime (NowTime, StartTime) * (double)mcget_ncount () / ncount);
      IntermediateCnts = 0;
      MPI_MASTER (fprintf (stdout, "\nTrace ETA "); fprintf (stdout, "%s", infostring);
                  if (difftime (EndTime, StartTime) < 60.0) fprintf (stdout, "%g [s] ", difftime (EndTime, StartTime));
                  else if (difftime (EndTime, StartTime) > 3600.0) fprintf (stdout, "%g [h] ", difftime (EndTime, StartTime) / 3600.0);
                  else fprintf (stdout, "%g [min] ", difftime (EndTime, StartTime) / 60.0); fprintf (stdout, "\n"););
    } else
      IntermediateCnts += 1e3;
    fflush (stdout);
  }

  /* display percentage when percent or minutes have reached step */
  if (EndTime && mcget_ncount () && ((minutes && difftime (NowTime, CurrentTime) > minutes * 60) || (percent && !minutes && ncount >= IntermediateCnts))) {
    MPI_MASTER (fprintf (stdout, "%llu %%\n", (unsigned long long)(ncount * 100.0 / mcget_ncount ())); fflush (stdout););
    CurrentTime = NowTime;

    IntermediateCnts = ncount + percent * mcget_ncount () / 100;
    /* check that next intermediate ncount check is a multiple of the desired percentage */
    IntermediateCnts = floor (IntermediateCnts * 100 / percent / mcget_ncount ()) * percent * mcget_ncount () / 100;
    /* raise flag to indicate that we did something */
    SCATTER;
    if (flag_save)
      save (NULL);
  }
  #endif
#ifndef NOABSORB_INF_NAN
  /* Check for nan or inf particle parms */ 
  if(isnan(p + t + vx + vy + vz + x + y + z)) ABSORB;
  if(isinf(fabs(p) + fabs(t) + fabs(vx) + fabs(vy) + fabs(vz) + fabs(x) + fabs(y) + fabs(z))) ABSORB;
#else
  if(isnan(p)  ||  isinf(p)) printf("NAN or INF found in p,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(t)  ||  isinf(t)) printf("NAN or INF found in t,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vx) || isinf(vx)) printf("NAN or INF found in vx, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vy) || isinf(vy)) printf("NAN or INF found in vy, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vz) || isinf(vz)) printf("NAN or INF found in vz, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(x)  ||  isinf(x)) printf("NAN or INF found in x,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(y)  ||  isinf(y)) printf("NAN or INF found in y,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(z)  ||  isinf(z)) printf("NAN or INF found in z,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
#endif
  #undef profile
  #undef percent
  #undef flag_save
  #undef minutes
  #undef IntermediateCnts
  #undef StartTime
  #undef EndTime
  #undef CurrentTime
  #undef infostring
  return;
} /* class_Progress_bar_trace */

#pragma acc routine
void class_TOF_monitor_trace(_class_TOF_monitor *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define nt (_comp->_parameters.nt)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define tmin (_comp->_parameters.tmin)
  #define tmax (_comp->_parameters.tmax)
  #define dt (_comp->_parameters.dt)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define TOF_N (_comp->_parameters.TOF_N)
  #define TOF_p (_comp->_parameters.TOF_p)
  #define TOF_p2 (_comp->_parameters.TOF_p2)
  #define t_min (_comp->_parameters.t_min)
  #define t_max (_comp->_parameters.t_max)
  #define delta_t (_comp->_parameters.delta_t)
  SIG_MESSAGE("[_TOFmoderator_zoom_trace] component TOFmoderator_zoom=TOF_monitor() TRACE [TOF_monitor:0]");

  int i;

  PROP_Z0;
  if (x > xmin && x < xmax && y > ymin && y < ymax) {
    i = floor ((1E6 * t - t_min) / delta_t); /* Bin number */
    if (i >= 0 && i < nt) {
      double p2 = p * p;
      #pragma acc atomic
      TOF_N[i] = TOF_N[i] + 1;
      #pragma acc atomic
      TOF_p[i] = TOF_p[i] + p;
      #pragma acc atomic
      TOF_p2[i] = TOF_p2[i] + p2;
      SCATTER;
    }
  }
  if (restore_neutron) {
    RESTORE_NEUTRON (INDEX_CURRENT_COMP, x, y, z, vx, vy, vz, t, sx, sy, sz, p);
  }
#ifndef NOABSORB_INF_NAN
  /* Check for nan or inf particle parms */ 
  if(isnan(p + t + vx + vy + vz + x + y + z)) ABSORB;
  if(isinf(fabs(p) + fabs(t) + fabs(vx) + fabs(vy) + fabs(vz) + fabs(x) + fabs(y) + fabs(z))) ABSORB;
#else
  if(isnan(p)  ||  isinf(p)) printf("NAN or INF found in p,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(t)  ||  isinf(t)) printf("NAN or INF found in t,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vx) || isinf(vx)) printf("NAN or INF found in vx, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vy) || isinf(vy)) printf("NAN or INF found in vy, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vz) || isinf(vz)) printf("NAN or INF found in vz, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(x)  ||  isinf(x)) printf("NAN or INF found in x,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(y)  ||  isinf(y)) printf("NAN or INF found in y,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(z)  ||  isinf(z)) printf("NAN or INF found in z,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
#endif
  #undef nt
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef tmin
  #undef tmax
  #undef dt
  #undef restore_neutron
  #undef nowritefile
  #undef TOF_N
  #undef TOF_p
  #undef TOF_p2
  #undef t_min
  #undef t_max
  #undef delta_t
  return;
} /* class_TOF_monitor_trace */

#pragma acc routine
void class_L_monitor_trace(_class_L_monitor *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define nL (_comp->_parameters.nL)
  #define filename (_comp->_parameters.filename)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Lmin (_comp->_parameters.Lmin)
  #define Lmax (_comp->_parameters.Lmax)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define L_N (_comp->_parameters.L_N)
  #define L_p (_comp->_parameters.L_p)
  #define L_p2 (_comp->_parameters.L_p2)
  SIG_MESSAGE("[_Lmon_guistart_trace] component Lmon_guistart=L_monitor() TRACE [L_monitor:0]");

  PROP_Z0;
  if (x > xmin && x < xmax && y > ymin && y < ymax) {
    double L = (2 * PI / V2K) / sqrt (vx * vx + vy * vy + vz * vz);
    int i = floor ((L - Lmin) * nL / (Lmax - Lmin));
    if (i >= 0 && i < nL) {
      double p2 = p * p;
      #pragma acc atomic
      L_N[i] = L_N[i] + 1;
      #pragma acc atomic
      L_p[i] = L_p[i] + p;
      #pragma acc atomic
      L_p2[i] = L_p2[i] + p2;
      SCATTER;
    }
  }
  if (restore_neutron) {
    RESTORE_NEUTRON (INDEX_CURRENT_COMP, x, y, z, vx, vy, vz, t, sx, sy, sz, p);
  }
#ifndef NOABSORB_INF_NAN
  /* Check for nan or inf particle parms */ 
  if(isnan(p + t + vx + vy + vz + x + y + z)) ABSORB;
  if(isinf(fabs(p) + fabs(t) + fabs(vx) + fabs(vy) + fabs(vz) + fabs(x) + fabs(y) + fabs(z))) ABSORB;
#else
  if(isnan(p)  ||  isinf(p)) printf("NAN or INF found in p,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(t)  ||  isinf(t)) printf("NAN or INF found in t,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vx) || isinf(vx)) printf("NAN or INF found in vx, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vy) || isinf(vy)) printf("NAN or INF found in vy, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vz) || isinf(vz)) printf("NAN or INF found in vz, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(x)  ||  isinf(x)) printf("NAN or INF found in x,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(y)  ||  isinf(y)) printf("NAN or INF found in y,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(z)  ||  isinf(z)) printf("NAN or INF found in z,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
#endif
  #undef nL
  #undef filename
  #undef nowritefile
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef Lmin
  #undef Lmax
  #undef restore_neutron
  #undef L_N
  #undef L_p
  #undef L_p2
  return;
} /* class_L_monitor_trace */

#pragma acc routine
void class_Guide_trace(_class_Guide *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define reflect (_comp->_parameters.reflect)
  #define w1 (_comp->_parameters.w1)
  #define h1 (_comp->_parameters.h1)
  #define w2 (_comp->_parameters.w2)
  #define h2 (_comp->_parameters.h2)
  #define l (_comp->_parameters.l)
  #define R0 (_comp->_parameters.R0)
  #define Qc (_comp->_parameters.Qc)
  #define alpha (_comp->_parameters.alpha)
  #define m (_comp->_parameters.m)
  #define W (_comp->_parameters.W)
  #define pTable (_comp->_parameters.pTable)
  #define table_present (_comp->_parameters.table_present)
  SIG_MESSAGE("[_Guide1_trace] component Guide1=Guide() TRACE [Guide:0]");

  double t1, t2;                                 /* Intersection times. */
  double av, ah, bv, bh, cv1, cv2, ch1, ch2, d;  /* Intermediate values */
  double weight;                                 /* Internal probability weight */
  double vdotn_v1, vdotn_v2, vdotn_h1, vdotn_h2; /* Dot products. */
  int i;                                         /* Which mirror hit? */
  double q;                                      /* Q [1/AA] of reflection */
  double nlen2;                                  /* Vector lengths squared */
  double par[5] = { R0, Qc, alpha, m, W };

  /* ToDo: These could be precalculated. */
  double ww = .5 * (w2 - w1), hh = .5 * (h2 - h1);
  double whalf = .5 * w1, hhalf = .5 * h1;

  /* Propagate neutron to guide entrance. */
  PROP_Z0;
  /* Scatter here to ensure that fully transmitted neutrons will not be
     absorbed in a GROUP construction, e.g. all neutrons - even the
     later absorbed ones are scattered at the guide entry. */
  SCATTER;
  if (x <= -whalf || x >= whalf || y <= -hhalf || y >= hhalf)
    ABSORB;
  for (;;) {
    /* Compute the dot products of v and n for the four mirrors. */
    av = l * vx;
    bv = ww * vz;
    ah = l * vy;
    bh = hh * vz;
    vdotn_v1 = bv + av; /* Left vertical */
    vdotn_v2 = bv - av; /* Right vertical */
    vdotn_h1 = bh + ah; /* Lower horizontal */
    vdotn_h2 = bh - ah; /* Upper horizontal */
    /* Compute the dot products of (O - r) and n as c1+c2 and c1-c2 */
    cv1 = -whalf * l - z * ww;
    cv2 = x * l;
    ch1 = -hhalf * l - z * hh;
    ch2 = y * l;
    /* Compute intersection times. */
    t1 = (l - z) / vz;
    i = 0;
    if (vdotn_v1 < 0 && (t2 = (cv1 - cv2) / vdotn_v1) < t1) {
      t1 = t2;
      i = 1;
    }
    if (vdotn_v2 < 0 && (t2 = (cv1 + cv2) / vdotn_v2) < t1) {
      t1 = t2;
      i = 2;
    }
    if (vdotn_h1 < 0 && (t2 = (ch1 - ch2) / vdotn_h1) < t1) {
      t1 = t2;
      i = 3;
    }
    if (vdotn_h2 < 0 && (t2 = (ch1 + ch2) / vdotn_h2) < t1) {
      t1 = t2;
      i = 4;
    }
    if (i == 0)
      break; /* Neutron left guide. */
    PROP_DT (t1);
    switch (i) {
    case 1: /* Left vertical mirror */
      nlen2 = l * l + ww * ww;
      q = V2Q * (-2) * vdotn_v1 / sqrt (nlen2);
      d = 2 * vdotn_v1 / nlen2;
      vx = vx - d * l;
      vz = vz - d * ww;
      break;
    case 2: /* Right vertical mirror */
      nlen2 = l * l + ww * ww;
      q = V2Q * (-2) * vdotn_v2 / sqrt (nlen2);
      d = 2 * vdotn_v2 / nlen2;
      vx = vx + d * l;
      vz = vz - d * ww;
      break;
    case 3: /* Lower horizontal mirror */
      nlen2 = l * l + hh * hh;
      q = V2Q * (-2) * vdotn_h1 / sqrt (nlen2);
      d = 2 * vdotn_h1 / nlen2;
      vy = vy - d * l;
      vz = vz - d * hh;
      break;
    case 4: /* Upper horizontal mirror */
      nlen2 = l * l + hh * hh;
      q = V2Q * (-2) * vdotn_h2 / sqrt (nlen2);
      d = 2 * vdotn_h2 / nlen2;
      vy = vy + d * l;
      vz = vz - d * hh;
      break;
    }
    /* Now compute reflectivity. */
    weight = 1.0; /* Initial internal weight factor */
    if (m == 0)
      ABSORB;
    if (reflect && table_present == 1)
      TableReflecFunc (q, &pTable, &weight);
    else {
      StdReflecFunc (q, par, &weight);
    }
    if (weight > 0)
      p *= weight;
    else
      ABSORB;
    SCATTER;
  }
#ifndef NOABSORB_INF_NAN
  /* Check for nan or inf particle parms */ 
  if(isnan(p + t + vx + vy + vz + x + y + z)) ABSORB;
  if(isinf(fabs(p) + fabs(t) + fabs(vx) + fabs(vy) + fabs(vz) + fabs(x) + fabs(y) + fabs(z))) ABSORB;
#else
  if(isnan(p)  ||  isinf(p)) printf("NAN or INF found in p,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(t)  ||  isinf(t)) printf("NAN or INF found in t,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vx) || isinf(vx)) printf("NAN or INF found in vx, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vy) || isinf(vy)) printf("NAN or INF found in vy, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vz) || isinf(vz)) printf("NAN or INF found in vz, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(x)  ||  isinf(x)) printf("NAN or INF found in x,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(y)  ||  isinf(y)) printf("NAN or INF found in y,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(z)  ||  isinf(z)) printf("NAN or INF found in z,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
#endif
  #undef reflect
  #undef w1
  #undef h1
  #undef w2
  #undef h2
  #undef l
  #undef R0
  #undef Qc
  #undef alpha
  #undef m
  #undef W
  #undef pTable
  #undef table_present
  return;
} /* class_Guide_trace */

#pragma acc routine
void class_PSD_monitor_trace(_class_PSD_monitor *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define nx (_comp->_parameters.nx)
  #define ny (_comp->_parameters.ny)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define PSD_N (_comp->_parameters.PSD_N)
  #define PSD_p (_comp->_parameters.PSD_p)
  #define PSD_p2 (_comp->_parameters.PSD_p2)
  SIG_MESSAGE("[_PSDslow1_trace] component PSDslow1=PSD_monitor() TRACE [PSD_monitor:0]");

  PROP_Z0;
  if (x > xmin && x < xmax && y > ymin && y < ymax) {
    int i = floor ((x - xmin) * nx / (xmax - xmin));
    int j = floor ((y - ymin) * ny / (ymax - ymin));

    double p2 = p * p;
    #pragma acc atomic
    PSD_N[i][j] = PSD_N[i][j] + 1;

    #pragma acc atomic
    PSD_p[i][j] = PSD_p[i][j] + p;

    #pragma acc atomic
    PSD_p2[i][j] = PSD_p2[i][j] + p2;

    SCATTER;
  }
  if (restore_neutron) {
    RESTORE_NEUTRON (INDEX_CURRENT_COMP, x, y, z, vx, vy, vz, t, sx, sy, sz, p);
  }
#ifndef NOABSORB_INF_NAN
  /* Check for nan or inf particle parms */ 
  if(isnan(p + t + vx + vy + vz + x + y + z)) ABSORB;
  if(isinf(fabs(p) + fabs(t) + fabs(vx) + fabs(vy) + fabs(vz) + fabs(x) + fabs(y) + fabs(z))) ABSORB;
#else
  if(isnan(p)  ||  isinf(p)) printf("NAN or INF found in p,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(t)  ||  isinf(t)) printf("NAN or INF found in t,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vx) || isinf(vx)) printf("NAN or INF found in vx, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vy) || isinf(vy)) printf("NAN or INF found in vy, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vz) || isinf(vz)) printf("NAN or INF found in vz, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(x)  ||  isinf(x)) printf("NAN or INF found in x,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(y)  ||  isinf(y)) printf("NAN or INF found in y,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(z)  ||  isinf(z)) printf("NAN or INF found in z,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
#endif
  #undef nx
  #undef ny
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef restore_neutron
  #undef nowritefile
  #undef PSD_N
  #undef PSD_p
  #undef PSD_p2
  return;
} /* class_PSD_monitor_trace */

#pragma acc routine
void class_DiskChopper_trace(_class_DiskChopper *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define theta_0 (_comp->_parameters.theta_0)
  #define radius (_comp->_parameters.radius)
  #define yheight (_comp->_parameters.yheight)
  #define nu (_comp->_parameters.nu)
  #define nslit (_comp->_parameters.nslit)
  #define jitter (_comp->_parameters.jitter)
  #define delay (_comp->_parameters.delay)
  #define isfirst (_comp->_parameters.isfirst)
  #define n_pulse (_comp->_parameters.n_pulse)
  #define abs_out (_comp->_parameters.abs_out)
  #define phase (_comp->_parameters.phase)
  #define xwidth (_comp->_parameters.xwidth)
  #define verbose (_comp->_parameters.verbose)
  #define Tg (_comp->_parameters.Tg)
  #define To (_comp->_parameters.To)
  #define delta_y (_comp->_parameters.delta_y)
  #define height (_comp->_parameters.height)
  #define omega (_comp->_parameters.omega)
  SIG_MESSAGE("[_FOchop1_trace] component FOchop1=DiskChopper() TRACE [DiskChopper:0]");

  double toff;
  double yprime;
  PROP_Z0;
  yprime = y + delta_y;

  /* Is neutron outside the vertical slit range and should we absorb? */
  if (abs_out && (x * x + yprime * yprime) > radius * radius) {
    ABSORB;
  }
  /* Does neutron hit inner solid part of chopper in case of yheight!=radius? */
  if ((x * x + yprime * yprime) < (radius - height) * (radius - height)) {
    ABSORB;
  }

  if (isfirst) {
    /* all events are put in the transmitted time frame */
    t = atan2 (x, yprime) / omega + To * randpm1 () / 2.0 + delay + (jitter ? jitter * randnorm () : 0) + (n_pulse > 1 ? floor (n_pulse * rand01 ()) * Tg : 0);
    /* correction: chopper slits transmission opening/full disk */
    p *= nslit * theta_0 / 2.0 / PI;
  } else {
    toff = fabs (t - atan2 (x, yprime) / omega - delay - (jitter ? jitter * randnorm () : 0));

    /* does neutron hit outside slit? */
    if (fmod (toff + To / 2.0, Tg) > To)
      ABSORB;
  }
  SCATTER;
#ifndef NOABSORB_INF_NAN
  /* Check for nan or inf particle parms */ 
  if(isnan(p + t + vx + vy + vz + x + y + z)) ABSORB;
  if(isinf(fabs(p) + fabs(t) + fabs(vx) + fabs(vy) + fabs(vz) + fabs(x) + fabs(y) + fabs(z))) ABSORB;
#else
  if(isnan(p)  ||  isinf(p)) printf("NAN or INF found in p,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(t)  ||  isinf(t)) printf("NAN or INF found in t,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vx) || isinf(vx)) printf("NAN or INF found in vx, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vy) || isinf(vy)) printf("NAN or INF found in vy, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vz) || isinf(vz)) printf("NAN or INF found in vz, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(x)  ||  isinf(x)) printf("NAN or INF found in x,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(y)  ||  isinf(y)) printf("NAN or INF found in y,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(z)  ||  isinf(z)) printf("NAN or INF found in z,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
#endif
  #undef theta_0
  #undef radius
  #undef yheight
  #undef nu
  #undef nslit
  #undef jitter
  #undef delay
  #undef isfirst
  #undef n_pulse
  #undef abs_out
  #undef phase
  #undef xwidth
  #undef verbose
  #undef Tg
  #undef To
  #undef delta_y
  #undef height
  #undef omega
  return;
} /* class_DiskChopper_trace */

#pragma acc routine
void class_TOFLambda_monitor_trace(_class_TOFLambda_monitor *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define nowritefile (_comp->_parameters.nowritefile)
  #define nL (_comp->_parameters.nL)
  #define nt (_comp->_parameters.nt)
  #define tmin (_comp->_parameters.tmin)
  #define tmax (_comp->_parameters.tmax)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Lmin (_comp->_parameters.Lmin)
  #define Lmax (_comp->_parameters.Lmax)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define TOFL_N (_comp->_parameters.TOFL_N)
  #define TOFL_p (_comp->_parameters.TOFL_p)
  #define TOFL_p2 (_comp->_parameters.TOFL_p2)
  #define tt_0 (_comp->_parameters.tt_0)
  #define tt_1 (_comp->_parameters.tt_1)
  SIG_MESSAGE("[_TOFLmon1_trace] component TOFLmon1=TOFLambda_monitor() TRACE [TOFLambda_monitor:0]");

  int i, j;
  double div;
  double lambda;

  PROP_Z0;
  lambda = (2 * PI / V2K) / sqrt (vx * vx + vy * vy + vz * vz);
  if (x > xmin && x < xmax && y > ymin && y < ymax && lambda > Lmin && lambda < Lmax) {
    if (t < tt_1 && t > tt_0) {
      i = floor ((lambda - Lmin) * nL / (Lmax - Lmin));
      j = floor ((t - tt_0) * nt / (tt_1 - tt_0));
      /*  printf("tt_0, tt_1, nt %g %g %i t j %g %i \n",tt_0,tt_1,nt,t,j);
       */
      double p2 = p * p;
      #pragma acc atomic
      TOFL_N[j][i] = TOFL_N[j][i] + 1;
      #pragma acc atomic
      TOFL_p[j][i] = TOFL_p[j][i] + p;
      #pragma acc atomic
      TOFL_p2[j][i] = TOFL_p2[j][i] + p2;
    }
  }
  if (restore_neutron) {
    RESTORE_NEUTRON (INDEX_CURRENT_COMP, x, y, z, vx, vy, vz, t, sx, sy, sz, p);
  }
#ifndef NOABSORB_INF_NAN
  /* Check for nan or inf particle parms */ 
  if(isnan(p + t + vx + vy + vz + x + y + z)) ABSORB;
  if(isinf(fabs(p) + fabs(t) + fabs(vx) + fabs(vy) + fabs(vz) + fabs(x) + fabs(y) + fabs(z))) ABSORB;
#else
  if(isnan(p)  ||  isinf(p)) printf("NAN or INF found in p,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(t)  ||  isinf(t)) printf("NAN or INF found in t,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vx) || isinf(vx)) printf("NAN or INF found in vx, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vy) || isinf(vy)) printf("NAN or INF found in vy, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vz) || isinf(vz)) printf("NAN or INF found in vz, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(x)  ||  isinf(x)) printf("NAN or INF found in x,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(y)  ||  isinf(y)) printf("NAN or INF found in y,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(z)  ||  isinf(z)) printf("NAN or INF found in z,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
#endif
  #undef nowritefile
  #undef nL
  #undef nt
  #undef tmin
  #undef tmax
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef Lmin
  #undef Lmax
  #undef restore_neutron
  #undef TOFL_N
  #undef TOFL_p
  #undef TOFL_p2
  #undef tt_0
  #undef tt_1
  return;
} /* class_TOFLambda_monitor_trace */

#pragma acc routine
void class_E_monitor_trace(_class_E_monitor *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define nE (_comp->_parameters.nE)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Emin (_comp->_parameters.Emin)
  #define Emax (_comp->_parameters.Emax)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define E_N (_comp->_parameters.E_N)
  #define E_p (_comp->_parameters.E_p)
  #define E_p2 (_comp->_parameters.E_p2)
  #define S_p (_comp->_parameters.S_p)
  #define S_pE (_comp->_parameters.S_pE)
  #define S_pE2 (_comp->_parameters.S_pE2)
  SIG_MESSAGE("[_Esample_trace] component Esample=E_monitor() TRACE [E_monitor:0]");

  int i;
  double E;

  PROP_Z0;
  if (x > xmin && x < xmax && y > ymin && y < ymax) {
    E = VS2E * (vx * vx + vy * vy + vz * vz);

    S_p += p;
    S_pE += p * E;
    S_pE2 += p * E * E;

    i = floor ((E - Emin) * nE / (Emax - Emin));
    if (i >= 0 && i < nE) {
      double p2 = p * p;
      #pragma acc atomic
      E_N[i] = E_N[i] + 1;
      #pragma acc atomic
      E_p[i] = E_p[i] + p;
      #pragma acc atomic
      E_p2[i] = E_p2[i] + p2;
      SCATTER;
    }
  }
  if (restore_neutron) {
    RESTORE_NEUTRON (INDEX_CURRENT_COMP, x, y, z, vx, vy, vz, t, sx, sy, sz, p);
  }
#ifndef NOABSORB_INF_NAN
  /* Check for nan or inf particle parms */ 
  if(isnan(p + t + vx + vy + vz + x + y + z)) ABSORB;
  if(isinf(fabs(p) + fabs(t) + fabs(vx) + fabs(vy) + fabs(vz) + fabs(x) + fabs(y) + fabs(z))) ABSORB;
#else
  if(isnan(p)  ||  isinf(p)) printf("NAN or INF found in p,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(t)  ||  isinf(t)) printf("NAN or INF found in t,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vx) || isinf(vx)) printf("NAN or INF found in vx, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vy) || isinf(vy)) printf("NAN or INF found in vy, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vz) || isinf(vz)) printf("NAN or INF found in vz, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(x)  ||  isinf(x)) printf("NAN or INF found in x,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(y)  ||  isinf(y)) printf("NAN or INF found in y,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(z)  ||  isinf(z)) printf("NAN or INF found in z,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
#endif
  #undef nE
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef nowritefile
  #undef xwidth
  #undef yheight
  #undef Emin
  #undef Emax
  #undef restore_neutron
  #undef E_N
  #undef E_p
  #undef E_p2
  #undef S_p
  #undef S_pE
  #undef S_pE2
  return;
} /* class_E_monitor_trace */

#pragma acc routine
void class_Tunneling_sample_trace(_class_Tunneling_sample *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define thickness (_comp->_parameters.thickness)
  #define radius (_comp->_parameters.radius)
  #define focus_r (_comp->_parameters.focus_r)
  #define p_interact (_comp->_parameters.p_interact)
  #define f_QE (_comp->_parameters.f_QE)
  #define f_tun (_comp->_parameters.f_tun)
  #define gamma (_comp->_parameters.gamma)
  #define E_tun (_comp->_parameters.E_tun)
  #define target_x (_comp->_parameters.target_x)
  #define target_y (_comp->_parameters.target_y)
  #define target_z (_comp->_parameters.target_z)
  #define focus_xw (_comp->_parameters.focus_xw)
  #define focus_yh (_comp->_parameters.focus_yh)
  #define focus_aw (_comp->_parameters.focus_aw)
  #define focus_ah (_comp->_parameters.focus_ah)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define zdepth (_comp->_parameters.zdepth)
  #define sigma_abs (_comp->_parameters.sigma_abs)
  #define sigma_inc (_comp->_parameters.sigma_inc)
  #define Vc (_comp->_parameters.Vc)
  #define target_index (_comp->_parameters.target_index)
  #define VarsV (_comp->_parameters.VarsV)
  #define ftun (_comp->_parameters.ftun)
  #define fQE (_comp->_parameters.fQE)
  SIG_MESSAGE("[_sample_trace] component sample=Tunneling_sample() TRACE [Tunneling_sample:0]");

  double t0, t3;                          /* Entry/exit time for outer cylinder */
  double t1, t2;                          /* Entry/exit time for inner cylinder */
  double v;                               /* Neutron velocity */
  double dt0, dt1, dt2, dt;               /* Flight times through sample */
  double l_full;                          /* Flight path length for non-scattered neutron */
  double l_i, l_o = 0;                    /* Flight path lenght in/out for scattered neutron */
  double my_a = 0;                        /* Velocity-dependent attenuation factor */
  double solid_angle = 0;                 /* Solid angle of target as seen from scattering point */
  double aim_x = 0, aim_y = 0, aim_z = 1; /* Position of target relative to scattering point */
  double v_i, v_f, E_i, E_f;              /* initial and final energies and velocities */
  double dE;                              /* Energy transfer */
  double scatt_choice;                    /* Representing random choice of scattering type */
  int intersect = 0;

  if (VarsV.isrect)
    intersect = box_intersect (&t0, &t3, x, y, z, vx, vy, vz, xwidth, yheight, zdepth);
  else
    intersect = cylinder_intersect (&t0, &t3, x, y, z, vx, vy, vz, radius, yheight);
  if (intersect) {
    if (t0 < 0)
      ABSORB; /* we already passed the sample; this is illegal */
    /* Neutron enters at t=t0. */
    if (VarsV.isrect)
      t1 = t2 = t3;
    else if (!thickness || !cylinder_intersect (&t1, &t2, x, y, z, vx, vy, vz, radius - thickness, yheight))
      t1 = t2 = t3;

    dt0 = t1 - t0; /* Time in sample, ingoing */
    dt1 = t2 - t1; /* Time in hole */
    dt2 = t3 - t2; /* Time in sample, outgoing */
    v = sqrt (vx * vx + vy * vy + vz * vz);
    l_full = v * (dt0 + dt2); /* Length of full path through sample */
    if (v)
      my_a = VarsV.my_a_v * (2200 / v);

    if (p_interact >= 1 || rand01 () < p_interact) /* Scattering */
    {
      dt = rand01 () * (dt0 + dt2); /* Time of scattering (relative to t0) */
      l_i = v * dt;                 /* Penetration in sample: scattering+abs */
      if (dt > dt0)
        dt += dt1; /* jump to 2nd side of cylinder */

      PROP_DT (dt + t0); /* Point of scattering */

      if ((VarsV.tx || VarsV.ty || VarsV.tz)) {
        aim_x = VarsV.tx - x; /* Vector pointing at target (anal./det.) */
        aim_y = VarsV.ty - y;
        aim_z = VarsV.tz - z;
      }
      if (VarsV.aw && VarsV.ah) {
        randvec_target_rect_angular (&vx, &vy, &vz, &solid_angle, aim_x, aim_y, aim_z, VarsV.aw, VarsV.ah, ROT_A_CURRENT_COMP);
      } else if (VarsV.xw && VarsV.yh) {
        randvec_target_rect (&vx, &vy, &vz, &solid_angle, aim_x, aim_y, aim_z, VarsV.xw, VarsV.yh, ROT_A_CURRENT_COMP);
      } else {
        randvec_target_circle (&vx, &vy, &vz, &solid_angle, aim_x, aim_y, aim_z, focus_r);
      }
      NORM (vx, vy, vz);

      scatt_choice = rand01 (); /* chooses type of scattering */
      v_i = v;                  /* Store initial velocity in case of inel. */
      E_i = VS2E * v_i * v_i;
      if (scatt_choice < (fQE + ftun)) /* Inelastic choices */
      {
        if (scatt_choice < fQE) /* Quasielastic */
        {
          dE = gamma * tan (PI / 2 * randpm1 ());
        } else {
          if (randpm1 () > 0)
            dE = E_tun;
          else
            dE = -E_tun;
        }
        E_f = E_i + dE;
        if (E_f <= 0)
          ABSORB;
        v_f = SE2V * sqrt (E_f);
        v = v_f;
      }

      vx *= v;
      vy *= v;
      vz *= v;

      if (!VarsV.isrect) {
        if (!cylinder_intersect (&t0, &t3, x, y, z, vx, vy, vz, radius, yheight)) {
          /* ??? did not hit cylinder */
          printf ("FATAL ERROR: Did not hit cylinder from inside.\n");
          exit (1);
        }
        dt = t3; /* outgoing point */
        if (thickness && cylinder_intersect (&t1, &t2, x, y, z, vx, vy, vz, radius - thickness, yheight) && t2 > 0)
          dt -= (t2 - t1); /* Subtract hollow part */
      } else {
        if (!box_intersect (&t0, &t3, x, y, z, vx, vy, vz, xwidth, yheight, zdepth)) {
          /* ??? did not hit box */
          printf ("FATAL ERROR: Did not hit box from inside.\n");
          exit (1);
        }
        dt = t3;
      }
      l_o = v * dt; /* trajectory after scattering point: absorption only */

      p *= v / v_i * l_full * VarsV.my_s * exp (-my_a * (l_i + v_i / v * l_o) - VarsV.my_s * l_i);
      /* We do not consider scattering from 2nd part (outgoing) */
      p /= 4 * PI / solid_angle;
      p /= p_interact;

      /* Polarisation part (1/3 NSF, 2/3 SF) */
      sx *= -1.0 / 3.0;
      sy *= -1.0 / 3.0;
      sz *= -1.0 / 3.0;

      SCATTER;
    } else /* Transmitting; always elastic */
    {
      p *= exp (-(my_a + VarsV.my_s) * l_full);
      p /= (1 - p_interact);
    }
  }
#ifndef NOABSORB_INF_NAN
  /* Check for nan or inf particle parms */ 
  if(isnan(p + t + vx + vy + vz + x + y + z)) ABSORB;
  if(isinf(fabs(p) + fabs(t) + fabs(vx) + fabs(vy) + fabs(vz) + fabs(x) + fabs(y) + fabs(z))) ABSORB;
#else
  if(isnan(p)  ||  isinf(p)) printf("NAN or INF found in p,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(t)  ||  isinf(t)) printf("NAN or INF found in t,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vx) || isinf(vx)) printf("NAN or INF found in vx, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vy) || isinf(vy)) printf("NAN or INF found in vy, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vz) || isinf(vz)) printf("NAN or INF found in vz, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(x)  ||  isinf(x)) printf("NAN or INF found in x,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(y)  ||  isinf(y)) printf("NAN or INF found in y,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(z)  ||  isinf(z)) printf("NAN or INF found in z,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
#endif
  #undef thickness
  #undef radius
  #undef focus_r
  #undef p_interact
  #undef f_QE
  #undef f_tun
  #undef gamma
  #undef E_tun
  #undef target_x
  #undef target_y
  #undef target_z
  #undef focus_xw
  #undef focus_yh
  #undef focus_aw
  #undef focus_ah
  #undef xwidth
  #undef yheight
  #undef zdepth
  #undef sigma_abs
  #undef sigma_inc
  #undef Vc
  #undef target_index
  #undef VarsV
  #undef ftun
  #undef fQE
  return;
} /* class_Tunneling_sample_trace */

#pragma acc routine
void class_TOF2E_monitor_trace(_class_TOF2E_monitor *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define nE (_comp->_parameters.nE)
  #define filename (_comp->_parameters.filename)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Emin (_comp->_parameters.Emin)
  #define Emax (_comp->_parameters.Emax)
  #define T_zero (_comp->_parameters.T_zero)
  #define L_flight (_comp->_parameters.L_flight)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define E_N (_comp->_parameters.E_N)
  #define E_p (_comp->_parameters.E_p)
  #define E_p2 (_comp->_parameters.E_p2)
  #define S_p (_comp->_parameters.S_p)
  #define S_pE (_comp->_parameters.S_pE)
  #define S_pE2 (_comp->_parameters.S_pE2)
  SIG_MESSAGE("[_TOF2Edetector_trace] component TOF2Edetector=TOF2E_monitor() TRACE [TOF2E_monitor:0]");

  int i;
  double E;

  PROP_Z0;
  if (x > xmin && x < xmax && y > ymin && y < ymax) {
    E = VS2E * (L_flight / (t - T_zero)) * (L_flight / (t - T_zero));

    S_p += p;
    S_pE += p * E;
    S_pE2 += p * E * E;

    i = floor ((E - Emin) * nE / (Emax - Emin));
    if (i >= 0 && i < nE) {
      double p2 = p * p;
      #pragma acc atomic
      E_N[i] = E_N[i] + 1;
      #pragma acc atomic
      E_p[i] = E_p[i] + p;
      #pragma acc atomic
      E_p2[i] = E_p2[i] + p2;
      SCATTER;
    }
  }
  if (restore_neutron) {
    RESTORE_NEUTRON (INDEX_CURRENT_COMP, x, y, z, vx, vy, vz, t, sx, sy, sz, p);
  }
#ifndef NOABSORB_INF_NAN
  /* Check for nan or inf particle parms */ 
  if(isnan(p + t + vx + vy + vz + x + y + z)) ABSORB;
  if(isinf(fabs(p) + fabs(t) + fabs(vx) + fabs(vy) + fabs(vz) + fabs(x) + fabs(y) + fabs(z))) ABSORB;
#else
  if(isnan(p)  ||  isinf(p)) printf("NAN or INF found in p,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(t)  ||  isinf(t)) printf("NAN or INF found in t,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vx) || isinf(vx)) printf("NAN or INF found in vx, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vy) || isinf(vy)) printf("NAN or INF found in vy, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(vz) || isinf(vz)) printf("NAN or INF found in vz, %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(x)  ||  isinf(x)) printf("NAN or INF found in x,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(y)  ||  isinf(y)) printf("NAN or INF found in y,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
  if(isnan(z)  ||  isinf(z)) printf("NAN or INF found in z,  %s (particle %lld)\n",_comp->_name,_particle->_uid);
#endif
  #undef nE
  #undef filename
  #undef nowritefile
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef Emin
  #undef Emax
  #undef T_zero
  #undef L_flight
  #undef restore_neutron
  #undef E_N
  #undef E_p
  #undef E_p2
  #undef S_p
  #undef S_pE
  #undef S_pE2
  return;
} /* class_TOF2E_monitor_trace */

/* *****************************************************************************
* instrument 'ESS_IN5_reprate' TRACE
***************************************************************************** */

#ifndef FUNNEL
#pragma acc routine
int raytrace(_class_particle* _particle) { /* single event propagation, called by mccode_main for ESS_IN5_reprate:TRACE */

  /* init variables and counters for TRACE */
  #undef ABSORB0
  #undef ABSORB
  #define ABSORB0 do { DEBUG_ABSORB(); MAGNET_OFF; ABSORBED++;} while(0)
  #define ABSORB ABSORB0
  DEBUG_ENTER();
  DEBUG_STATE();
  _particle->flag_nocoordschange=0; /* Init */
  _class_particle _particle_save=*_particle;
  /* the main iteration loop for one incoming event */
  while (!ABSORBED) { /* iterate event until absorbed */
    /* send particle event to component instance, one after the other */
    /* begin component source=ESS_butterfly() [1] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_source_var._rotation_is_identity) {
        if(!_source_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _source_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_source_var._position_relative, _source_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 1) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_source_var._name);
      DEBUG_STATE();
      class_ESS_butterfly_trace(&_source_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component source [1] */
    /* begin component Origin=Progress_bar() [2] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Origin_var._rotation_is_identity) {
        if(!_Origin_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Origin_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Origin_var._position_relative, _Origin_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 2) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Origin_var._name);
      DEBUG_STATE();
      class_Progress_bar_trace(&_Origin_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Origin [2] */
    /* begin component TOFmoderator_zoom=TOF_monitor() [3] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_TOFmoderator_zoom_var._rotation_is_identity) {
        if(!_TOFmoderator_zoom_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _TOFmoderator_zoom_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_TOFmoderator_zoom_var._position_relative, _TOFmoderator_zoom_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 3) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_TOFmoderator_zoom_var._name);
      DEBUG_STATE();
      class_TOF_monitor_trace(&_TOFmoderator_zoom_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component TOFmoderator_zoom [3] */
    /* begin component TOFmoderator=TOF_monitor() [4] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_TOFmoderator_var._rotation_is_identity) {
        if(!_TOFmoderator_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _TOFmoderator_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_TOFmoderator_var._position_relative, _TOFmoderator_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 4) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_TOFmoderator_var._name);
      DEBUG_STATE();
      class_TOF_monitor_trace(&_TOFmoderator_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component TOFmoderator [4] */
    /* begin component Lmon_guistart=L_monitor() [5] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Lmon_guistart_var._rotation_is_identity) {
        if(!_Lmon_guistart_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Lmon_guistart_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Lmon_guistart_var._position_relative, _Lmon_guistart_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 5) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Lmon_guistart_var._name);
      DEBUG_STATE();
      class_L_monitor_trace(&_Lmon_guistart_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Lmon_guistart [5] */
    /* begin component Lmon_normalize=L_monitor() [6] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Lmon_normalize_var._rotation_is_identity) {
        if(!_Lmon_normalize_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Lmon_normalize_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Lmon_normalize_var._position_relative, _Lmon_normalize_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 6) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Lmon_normalize_var._name);
      DEBUG_STATE();
      class_L_monitor_trace(&_Lmon_normalize_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Lmon_normalize [6] */
    /* begin component Guide1=Guide() [7] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Guide1_var._rotation_is_identity) {
        if(!_Guide1_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Guide1_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Guide1_var._position_relative, _Guide1_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 7) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Guide1_var._name);
      DEBUG_STATE();
      class_Guide_trace(&_Guide1_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Guide1 [7] */
    /* begin component Lmonslow1=L_monitor() [8] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Lmonslow1_var._rotation_is_identity) {
        if(!_Lmonslow1_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Lmonslow1_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Lmonslow1_var._position_relative, _Lmonslow1_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 8) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Lmonslow1_var._name);
      DEBUG_STATE();
      class_L_monitor_trace(&_Lmonslow1_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Lmonslow1 [8] */
    /* begin component PSDslow1=PSD_monitor() [9] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_PSDslow1_var._rotation_is_identity) {
        if(!_PSDslow1_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _PSDslow1_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_PSDslow1_var._position_relative, _PSDslow1_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 9) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_PSDslow1_var._name);
      DEBUG_STATE();
      class_PSD_monitor_trace(&_PSDslow1_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component PSDslow1 [9] */
    /* begin component FOchop1=DiskChopper() [10] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_FOchop1_var._rotation_is_identity) {
        if(!_FOchop1_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _FOchop1_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_FOchop1_var._position_relative, _FOchop1_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 10) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_FOchop1_var._name);
      DEBUG_STATE();
      class_DiskChopper_trace(&_FOchop1_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component FOchop1 [10] */
    /* begin component TOFLmon1=TOFLambda_monitor() [11] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_TOFLmon1_var._rotation_is_identity) {
        if(!_TOFLmon1_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _TOFLmon1_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_TOFLmon1_var._position_relative, _TOFLmon1_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 11) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_TOFLmon1_var._name);
      DEBUG_STATE();
      class_TOFLambda_monitor_trace(&_TOFLmon1_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component TOFLmon1 [11] */
    /* begin component Lmon_afterslow1=L_monitor() [12] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Lmon_afterslow1_var._rotation_is_identity) {
        if(!_Lmon_afterslow1_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Lmon_afterslow1_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Lmon_afterslow1_var._position_relative, _Lmon_afterslow1_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 12) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Lmon_afterslow1_var._name);
      DEBUG_STATE();
      class_L_monitor_trace(&_Lmon_afterslow1_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Lmon_afterslow1 [12] */
    /* begin component PSD_afterslow1=PSD_monitor() [13] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_PSD_afterslow1_var._rotation_is_identity) {
        if(!_PSD_afterslow1_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _PSD_afterslow1_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_PSD_afterslow1_var._position_relative, _PSD_afterslow1_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 13) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_PSD_afterslow1_var._name);
      DEBUG_STATE();
      class_PSD_monitor_trace(&_PSD_afterslow1_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component PSD_afterslow1 [13] */
    /* begin component Guidelong1=Guide() [14] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Guidelong1_var._rotation_is_identity) {
        if(!_Guidelong1_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Guidelong1_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Guidelong1_var._position_relative, _Guidelong1_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 14) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Guidelong1_var._name);
      DEBUG_STATE();
      class_Guide_trace(&_Guidelong1_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Guidelong1 [14] */
    /* begin component Guidelong1b=Guide() [15] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Guidelong1b_var._rotation_is_identity) {
        if(!_Guidelong1b_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Guidelong1b_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Guidelong1b_var._position_relative, _Guidelong1b_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 15) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Guidelong1b_var._name);
      DEBUG_STATE();
      class_Guide_trace(&_Guidelong1b_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Guidelong1b [15] */
    /* begin component Lmon_slow2=L_monitor() [16] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Lmon_slow2_var._rotation_is_identity) {
        if(!_Lmon_slow2_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Lmon_slow2_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Lmon_slow2_var._position_relative, _Lmon_slow2_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 16) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Lmon_slow2_var._name);
      DEBUG_STATE();
      class_L_monitor_trace(&_Lmon_slow2_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Lmon_slow2 [16] */
    /* begin component FOchop2=DiskChopper() [17] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_FOchop2_var._rotation_is_identity) {
        if(!_FOchop2_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _FOchop2_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_FOchop2_var._position_relative, _FOchop2_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 17) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_FOchop2_var._name);
      DEBUG_STATE();
      class_DiskChopper_trace(&_FOchop2_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component FOchop2 [17] */
    /* begin component Fastchop1=DiskChopper() [18] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Fastchop1_var._rotation_is_identity) {
        if(!_Fastchop1_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Fastchop1_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Fastchop1_var._position_relative, _Fastchop1_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 18) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Fastchop1_var._name);
      DEBUG_STATE();
      class_DiskChopper_trace(&_Fastchop1_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Fastchop1 [18] */
    /* begin component PSD_afterslow2=PSD_monitor() [19] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_PSD_afterslow2_var._rotation_is_identity) {
        if(!_PSD_afterslow2_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _PSD_afterslow2_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_PSD_afterslow2_var._position_relative, _PSD_afterslow2_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 19) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_PSD_afterslow2_var._name);
      DEBUG_STATE();
      class_PSD_monitor_trace(&_PSD_afterslow2_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component PSD_afterslow2 [19] */
    /* begin component Lmon_afterslow2=L_monitor() [20] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Lmon_afterslow2_var._rotation_is_identity) {
        if(!_Lmon_afterslow2_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Lmon_afterslow2_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Lmon_afterslow2_var._position_relative, _Lmon_afterslow2_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 20) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Lmon_afterslow2_var._name);
      DEBUG_STATE();
      class_L_monitor_trace(&_Lmon_afterslow2_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Lmon_afterslow2 [20] */
    /* begin component TOFL_afterslow2=TOFLambda_monitor() [21] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_TOFL_afterslow2_var._rotation_is_identity) {
        if(!_TOFL_afterslow2_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _TOFL_afterslow2_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_TOFL_afterslow2_var._position_relative, _TOFL_afterslow2_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 21) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_TOFL_afterslow2_var._name);
      DEBUG_STATE();
      class_TOFLambda_monitor_trace(&_TOFL_afterslow2_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component TOFL_afterslow2 [21] */
    /* begin component Guidelong2=Guide() [22] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Guidelong2_var._rotation_is_identity) {
        if(!_Guidelong2_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Guidelong2_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Guidelong2_var._position_relative, _Guidelong2_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 22) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Guidelong2_var._name);
      DEBUG_STATE();
      class_Guide_trace(&_Guidelong2_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Guidelong2 [22] */
    /* begin component Lmon_beforeballistic=L_monitor() [23] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Lmon_beforeballistic_var._rotation_is_identity) {
        if(!_Lmon_beforeballistic_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Lmon_beforeballistic_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Lmon_beforeballistic_var._position_relative, _Lmon_beforeballistic_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 23) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Lmon_beforeballistic_var._name);
      DEBUG_STATE();
      class_L_monitor_trace(&_Lmon_beforeballistic_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Lmon_beforeballistic [23] */
    /* begin component PSD_beforeballistic=PSD_monitor() [24] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_PSD_beforeballistic_var._rotation_is_identity) {
        if(!_PSD_beforeballistic_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _PSD_beforeballistic_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_PSD_beforeballistic_var._position_relative, _PSD_beforeballistic_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 24) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_PSD_beforeballistic_var._name);
      DEBUG_STATE();
      class_PSD_monitor_trace(&_PSD_beforeballistic_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component PSD_beforeballistic [24] */
    /* begin component Guidelong2a=Guide() [25] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Guidelong2a_var._rotation_is_identity) {
        if(!_Guidelong2a_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Guidelong2a_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Guidelong2a_var._position_relative, _Guidelong2a_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 25) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Guidelong2a_var._name);
      DEBUG_STATE();
      class_Guide_trace(&_Guidelong2a_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Guidelong2a [25] */
    /* begin component Lmonfast2=L_monitor() [26] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Lmonfast2_var._rotation_is_identity) {
        if(!_Lmonfast2_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Lmonfast2_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Lmonfast2_var._position_relative, _Lmonfast2_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 26) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Lmonfast2_var._name);
      DEBUG_STATE();
      class_L_monitor_trace(&_Lmonfast2_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Lmonfast2 [26] */
    /* begin component Lmonfast2_zoom=L_monitor() [27] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Lmonfast2_zoom_var._rotation_is_identity) {
        if(!_Lmonfast2_zoom_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Lmonfast2_zoom_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Lmonfast2_zoom_var._position_relative, _Lmonfast2_zoom_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 27) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Lmonfast2_zoom_var._name);
      DEBUG_STATE();
      class_L_monitor_trace(&_Lmonfast2_zoom_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Lmonfast2_zoom [27] */
    /* begin component TOFLfast2=TOFLambda_monitor() [28] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_TOFLfast2_var._rotation_is_identity) {
        if(!_TOFLfast2_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _TOFLfast2_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_TOFLfast2_var._position_relative, _TOFLfast2_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 28) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_TOFLfast2_var._name);
      DEBUG_STATE();
      class_TOFLambda_monitor_trace(&_TOFLfast2_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component TOFLfast2 [28] */
    /* begin component TOFLfast2zoom=TOFLambda_monitor() [29] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_TOFLfast2zoom_var._rotation_is_identity) {
        if(!_TOFLfast2zoom_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _TOFLfast2zoom_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_TOFLfast2zoom_var._position_relative, _TOFLfast2zoom_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 29) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_TOFLfast2zoom_var._name);
      DEBUG_STATE();
      class_TOFLambda_monitor_trace(&_TOFLfast2zoom_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component TOFLfast2zoom [29] */
    /* begin component PSDfast2=PSD_monitor() [30] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_PSDfast2_var._rotation_is_identity) {
        if(!_PSDfast2_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _PSDfast2_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_PSDfast2_var._position_relative, _PSDfast2_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 30) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_PSDfast2_var._name);
      DEBUG_STATE();
      class_PSD_monitor_trace(&_PSDfast2_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component PSDfast2 [30] */
    /* begin component Fastchop2=DiskChopper() [31] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Fastchop2_var._rotation_is_identity) {
        if(!_Fastchop2_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Fastchop2_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Fastchop2_var._position_relative, _Fastchop2_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 31) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Fastchop2_var._name);
      DEBUG_STATE();
      class_DiskChopper_trace(&_Fastchop2_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Fastchop2 [31] */
    /* begin component Fastchop2counter=DiskChopper() [32] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Fastchop2counter_var._rotation_is_identity) {
        if(!_Fastchop2counter_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Fastchop2counter_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Fastchop2counter_var._position_relative, _Fastchop2counter_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 32) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Fastchop2counter_var._name);
      DEBUG_STATE();
      class_DiskChopper_trace(&_Fastchop2counter_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Fastchop2counter [32] */
    /* begin component FOchop3=DiskChopper() [33] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_FOchop3_var._rotation_is_identity) {
        if(!_FOchop3_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _FOchop3_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_FOchop3_var._position_relative, _FOchop3_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 33) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_FOchop3_var._name);
      DEBUG_STATE();
      class_DiskChopper_trace(&_FOchop3_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component FOchop3 [33] */
    /* begin component TOFfast2_zoom=TOF_monitor() [34] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_TOFfast2_zoom_var._rotation_is_identity) {
        if(!_TOFfast2_zoom_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _TOFfast2_zoom_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_TOFfast2_zoom_var._position_relative, _TOFfast2_zoom_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 34) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_TOFfast2_zoom_var._name);
      DEBUG_STATE();
      class_TOF_monitor_trace(&_TOFfast2_zoom_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component TOFfast2_zoom [34] */
    /* begin component Lmon_afterfast2=L_monitor() [35] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Lmon_afterfast2_var._rotation_is_identity) {
        if(!_Lmon_afterfast2_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Lmon_afterfast2_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Lmon_afterfast2_var._position_relative, _Lmon_afterfast2_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 35) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Lmon_afterfast2_var._name);
      DEBUG_STATE();
      class_L_monitor_trace(&_Lmon_afterfast2_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Lmon_afterfast2 [35] */
    /* begin component TOFL_afterfast2=TOFLambda_monitor() [36] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_TOFL_afterfast2_var._rotation_is_identity) {
        if(!_TOFL_afterfast2_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _TOFL_afterfast2_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_TOFL_afterfast2_var._position_relative, _TOFL_afterfast2_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 36) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_TOFL_afterfast2_var._name);
      DEBUG_STATE();
      class_TOFLambda_monitor_trace(&_TOFL_afterfast2_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component TOFL_afterfast2 [36] */
    /* begin component TOFL_afterfast2_zoom=TOFLambda_monitor() [37] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_TOFL_afterfast2_zoom_var._rotation_is_identity) {
        if(!_TOFL_afterfast2_zoom_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _TOFL_afterfast2_zoom_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_TOFL_afterfast2_zoom_var._position_relative, _TOFL_afterfast2_zoom_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 37) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_TOFL_afterfast2_zoom_var._name);
      DEBUG_STATE();
      class_TOFLambda_monitor_trace(&_TOFL_afterfast2_zoom_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component TOFL_afterfast2_zoom [37] */
    /* begin component PSD_afterfast2=PSD_monitor() [38] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_PSD_afterfast2_var._rotation_is_identity) {
        if(!_PSD_afterfast2_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _PSD_afterfast2_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_PSD_afterfast2_var._position_relative, _PSD_afterfast2_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 38) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_PSD_afterfast2_var._name);
      DEBUG_STATE();
      class_PSD_monitor_trace(&_PSD_afterfast2_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component PSD_afterfast2 [38] */
    /* begin component Guidesample=Guide() [39] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Guidesample_var._rotation_is_identity) {
        if(!_Guidesample_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Guidesample_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Guidesample_var._position_relative, _Guidesample_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 39) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Guidesample_var._name);
      DEBUG_STATE();
      class_Guide_trace(&_Guidesample_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Guidesample [39] */
    /* begin component Lmon_guideend=L_monitor() [40] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Lmon_guideend_var._rotation_is_identity) {
        if(!_Lmon_guideend_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Lmon_guideend_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Lmon_guideend_var._position_relative, _Lmon_guideend_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 40) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Lmon_guideend_var._name);
      DEBUG_STATE();
      class_L_monitor_trace(&_Lmon_guideend_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Lmon_guideend [40] */
    /* begin component PSDsample=PSD_monitor() [41] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_PSDsample_var._rotation_is_identity) {
        if(!_PSDsample_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _PSDsample_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_PSDsample_var._position_relative, _PSDsample_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 41) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_PSDsample_var._name);
      DEBUG_STATE();
      class_PSD_monitor_trace(&_PSDsample_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component PSDsample [41] */
    /* begin component TOFsample_zoom=TOF_monitor() [42] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_TOFsample_zoom_var._rotation_is_identity) {
        if(!_TOFsample_zoom_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _TOFsample_zoom_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_TOFsample_zoom_var._position_relative, _TOFsample_zoom_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 42) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_TOFsample_zoom_var._name);
      DEBUG_STATE();
      class_TOF_monitor_trace(&_TOFsample_zoom_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component TOFsample_zoom [42] */
    /* begin component Esample=E_monitor() [43] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Esample_var._rotation_is_identity) {
        if(!_Esample_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Esample_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Esample_var._position_relative, _Esample_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 43) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Esample_var._name);
      DEBUG_STATE();
      class_E_monitor_trace(&_Esample_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Esample [43] */
    /* begin component Lmon_sample_zoom=L_monitor() [44] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Lmon_sample_zoom_var._rotation_is_identity) {
        if(!_Lmon_sample_zoom_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Lmon_sample_zoom_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Lmon_sample_zoom_var._position_relative, _Lmon_sample_zoom_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 44) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Lmon_sample_zoom_var._name);
      DEBUG_STATE();
      class_L_monitor_trace(&_Lmon_sample_zoom_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Lmon_sample_zoom [44] */
    /* begin component sample=Tunneling_sample() [45] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_sample_var._rotation_is_identity) {
        if(!_sample_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _sample_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_sample_var._position_relative, _sample_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 45) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_sample_var._name);
      DEBUG_STATE();
      class_Tunneling_sample_trace(&_sample_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component sample [45] */
    /* begin component detectorarm=Arm() [46] */
    if (!ABSORBED && _particle->_index == 46) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle->_index++;
    } /* end component detectorarm [46] */
    /* begin component TOFdetector=TOF_monitor() [47] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_TOFdetector_var._rotation_is_identity) {
        if(!_TOFdetector_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _TOFdetector_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_TOFdetector_var._position_relative, _TOFdetector_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 47) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_TOFdetector_var._name);
      DEBUG_STATE();
      class_TOF_monitor_trace(&_TOFdetector_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component TOFdetector [47] */
    /* begin component TOFdetector_zoom=TOF_monitor() [48] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_TOFdetector_zoom_var._rotation_is_identity) {
        if(!_TOFdetector_zoom_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _TOFdetector_zoom_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_TOFdetector_zoom_var._position_relative, _TOFdetector_zoom_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 48) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_TOFdetector_zoom_var._name);
      DEBUG_STATE();
      class_TOF_monitor_trace(&_TOFdetector_zoom_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component TOFdetector_zoom [48] */
    /* begin component Edetector=E_monitor() [49] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Edetector_var._rotation_is_identity) {
        if(!_Edetector_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Edetector_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Edetector_var._position_relative, _Edetector_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 49) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Edetector_var._name);
      DEBUG_STATE();
      class_E_monitor_trace(&_Edetector_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Edetector [49] */
    /* begin component TOF2Edetector=TOF2E_monitor() [50] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_TOF2Edetector_var._rotation_is_identity) {
        if(!_TOF2Edetector_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _TOF2Edetector_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_TOF2Edetector_var._position_relative, _TOF2Edetector_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 50) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_TOF2Edetector_var._name);
      DEBUG_STATE();
      class_TOF2E_monitor_trace(&_TOF2Edetector_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component TOF2Edetector [50] */
    if (_particle->_index > 50)
      ABSORBED++; /* absorbed when passed all components */
  } /* while !ABSORBED */

  DEBUG_LEAVE()
  particle_restore(_particle, &_particle_save);
  DEBUG_STATE()

  return(_particle->_index);
} /* raytrace */

/* loop to generate events and call raytrace() propagate them */
void raytrace_all(unsigned long long ncount, unsigned long seed) {

  // if on GPU and mcdotrace just exit
  #ifdef OPENACC
  if (!mcdotrace) {
  #endif

  /* CPU-loop */
  unsigned long long loops;
  loops = ceil((double)ncount/gpu_innerloop);
  /* if on GPU, printf has been globally nullified, re-enable here */
  #ifdef OPENACC
  #undef strlen
  #undef strcmp
  #undef exit
  #undef printf
  #undef sprintf
  #undef fprintf
  #endif

  #ifdef OPENACC
  if (ncount>gpu_innerloop) {
    printf("Defining %llu CPU loops around GPU kernel and adjusting ncount\n",loops);
    mcset_ncount(loops*gpu_innerloop);
  } else {
    #endif
    loops=1;
    gpu_innerloop = ncount;
    #ifdef OPENACC
  }
    #endif

  for (unsigned long long cloop=0; cloop<loops; cloop++) {
    #ifdef OPENACC
    if (loops>1) fprintf(stdout, "%d..", (int)cloop); fflush(stdout);
    #endif

    /* if on GPU, re-nullify printf */
     #ifdef OPENACC
     #undef strlen
     #undef strcmp
     #undef exit
     #undef printf
     #undef sprintf
     #undef fprintf
     #endif

    #pragma acc parallel loop num_gangs(numgangs) vector_length(vecsize)
    for (unsigned long pidx=0 ; pidx < gpu_innerloop ; pidx++) {
      _class_particle particleN = mcgenstate(); // initial particle
      _class_particle* _particle = &particleN;
      particleN._uid = pidx;
      #ifdef USE_MPI
      particleN._uid += mpi_node_rank * ncount; 
      #endif

      srandom(_hash((pidx+1)*(seed+1)));

      raytrace(_particle);
    } /* inner for */
    seed = seed+gpu_innerloop;
  } /* CPU for */
  /* if on GPU, printf has been globally nullified, re-enable here */
     #ifdef OPENACC
     #undef strlen
     #undef strcmp
     #undef exit
     #undef printf
     #undef sprintf
     #undef fprintf
     #endif
  MPI_MASTER(
  printf("*** TRACE end *** \n");
  );

  // if on GPU and mcdotrace just exit
  #ifdef OPENACC
  }
  #endif

} /* raytrace_all */

#endif //no-FUNNEL

#ifdef FUNNEL
// Alternative raytrace algorithm which iterates all particles through
// one component at the time, can remove absorbs from the next loop and
// switch between cpu/gpu.
void raytrace_all_funnel(unsigned long long ncount, unsigned long seed) {

  // if on GPU and mcdotrace just exit
  #ifdef OPENACC
  if (!mcdotrace) {
  #endif
  // set up outer (CPU) loop / particle batches
  unsigned long long loops;

  /* if on GPU, printf has been globally nullified, re-enable here */
   #ifdef OPENACC
   #undef strlen
   #undef strcmp
   #undef exit
   #undef printf
   #undef sprintf
   #undef fprintf
   #endif
  #ifdef OPENACC
  loops = ceil((double)ncount/gpu_innerloop);
  if (ncount>gpu_innerloop) {
    printf("Defining %llu CPU loops around kernel and adjusting ncount\n",loops);
    mcset_ncount(loops*gpu_innerloop);
  } else {
  #endif
    loops=1;
    gpu_innerloop = ncount;
  #ifdef OPENACC
  }
  #endif

  // create particles struct and pointer arrays (same memory used by all batches)
  _class_particle* particles = malloc(gpu_innerloop*sizeof(_class_particle));
  _class_particle* pbuffer = malloc(gpu_innerloop*sizeof(_class_particle));
  long livebatchsize = gpu_innerloop;

  #undef ABSORB0
  #undef ABSORB
  #define ABSORB0 do { DEBUG_ABSORB(); MAGNET_OFF; ABSORBED++; } while(0)
  #define ABSORB ABSORB0
  // outer loop / particle batches
  for (unsigned long long cloop=0; cloop<loops; cloop++) {
    if (loops>1) fprintf(stdout, "%d..", (int)cloop); fflush(stdout);

    // init particles
    #pragma acc parallel loop present(particles[0:livebatchsize])
    for (unsigned long pidx=0 ; pidx < livebatchsize ; pidx++) {
      // generate particle state, set loop index and seed
      particles[pidx] = mcgenstate();
      _class_particle* _particle = particles + pidx;
      _particle->_uid = pidx;
      #ifdef USE_MPI
      _particle->_uid += mpi_node_rank * ncount; 
      #endif
      srandom(_hash((pidx+1)*(seed+1))); // _particle->state usage built into srandom macro
    }

    // iterate components

    #pragma acc parallel loop present(particles[0:livebatchsize])
    for (unsigned long pidx=0 ; pidx < livebatchsize ; pidx++) {
      _class_particle* _particle = &particles[pidx];
      _class_particle _particle_save;

      // source
    if (!ABSORBED && _particle->_index == 1) {
#ifndef MULTICORE
        if (_source_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _source_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_source_var._position_relative, _source_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_ESS_butterfly_trace(&_source_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Origin
    if (!ABSORBED && _particle->_index == 2) {
#ifndef MULTICORE
        if (_Origin_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Origin_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Origin_var._position_relative, _Origin_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Progress_bar_trace(&_Origin_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // TOFmoderator_zoom
    if (!ABSORBED && _particle->_index == 3) {
#ifndef MULTICORE
        if (_TOFmoderator_zoom_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _TOFmoderator_zoom_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_TOFmoderator_zoom_var._position_relative, _TOFmoderator_zoom_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_TOF_monitor_trace(&_TOFmoderator_zoom_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // TOFmoderator
    if (!ABSORBED && _particle->_index == 4) {
#ifndef MULTICORE
        if (_TOFmoderator_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _TOFmoderator_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_TOFmoderator_var._position_relative, _TOFmoderator_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_TOF_monitor_trace(&_TOFmoderator_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Lmon_guistart
    if (!ABSORBED && _particle->_index == 5) {
#ifndef MULTICORE
        if (_Lmon_guistart_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Lmon_guistart_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Lmon_guistart_var._position_relative, _Lmon_guistart_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_L_monitor_trace(&_Lmon_guistart_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Lmon_normalize
    if (!ABSORBED && _particle->_index == 6) {
#ifndef MULTICORE
        if (_Lmon_normalize_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Lmon_normalize_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Lmon_normalize_var._position_relative, _Lmon_normalize_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_L_monitor_trace(&_Lmon_normalize_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Guide1
    if (!ABSORBED && _particle->_index == 7) {
#ifndef MULTICORE
        if (_Guide1_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Guide1_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Guide1_var._position_relative, _Guide1_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Guide_trace(&_Guide1_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Lmonslow1
    if (!ABSORBED && _particle->_index == 8) {
#ifndef MULTICORE
        if (_Lmonslow1_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Lmonslow1_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Lmonslow1_var._position_relative, _Lmonslow1_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_L_monitor_trace(&_Lmonslow1_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // PSDslow1
    if (!ABSORBED && _particle->_index == 9) {
#ifndef MULTICORE
        if (_PSDslow1_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _PSDslow1_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_PSDslow1_var._position_relative, _PSDslow1_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_PSD_monitor_trace(&_PSDslow1_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // FOchop1
    if (!ABSORBED && _particle->_index == 10) {
#ifndef MULTICORE
        if (_FOchop1_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _FOchop1_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_FOchop1_var._position_relative, _FOchop1_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_DiskChopper_trace(&_FOchop1_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // TOFLmon1
    if (!ABSORBED && _particle->_index == 11) {
#ifndef MULTICORE
        if (_TOFLmon1_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _TOFLmon1_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_TOFLmon1_var._position_relative, _TOFLmon1_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_TOFLambda_monitor_trace(&_TOFLmon1_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Lmon_afterslow1
    if (!ABSORBED && _particle->_index == 12) {
#ifndef MULTICORE
        if (_Lmon_afterslow1_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Lmon_afterslow1_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Lmon_afterslow1_var._position_relative, _Lmon_afterslow1_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_L_monitor_trace(&_Lmon_afterslow1_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // PSD_afterslow1
    if (!ABSORBED && _particle->_index == 13) {
#ifndef MULTICORE
        if (_PSD_afterslow1_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _PSD_afterslow1_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_PSD_afterslow1_var._position_relative, _PSD_afterslow1_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_PSD_monitor_trace(&_PSD_afterslow1_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Guidelong1
    if (!ABSORBED && _particle->_index == 14) {
#ifndef MULTICORE
        if (_Guidelong1_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Guidelong1_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Guidelong1_var._position_relative, _Guidelong1_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Guide_trace(&_Guidelong1_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Guidelong1b
    if (!ABSORBED && _particle->_index == 15) {
#ifndef MULTICORE
        if (_Guidelong1b_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Guidelong1b_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Guidelong1b_var._position_relative, _Guidelong1b_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Guide_trace(&_Guidelong1b_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Lmon_slow2
    if (!ABSORBED && _particle->_index == 16) {
#ifndef MULTICORE
        if (_Lmon_slow2_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Lmon_slow2_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Lmon_slow2_var._position_relative, _Lmon_slow2_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_L_monitor_trace(&_Lmon_slow2_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // FOchop2
    if (!ABSORBED && _particle->_index == 17) {
#ifndef MULTICORE
        if (_FOchop2_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _FOchop2_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_FOchop2_var._position_relative, _FOchop2_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_DiskChopper_trace(&_FOchop2_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Fastchop1
    if (!ABSORBED && _particle->_index == 18) {
#ifndef MULTICORE
        if (_Fastchop1_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Fastchop1_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Fastchop1_var._position_relative, _Fastchop1_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_DiskChopper_trace(&_Fastchop1_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // PSD_afterslow2
    if (!ABSORBED && _particle->_index == 19) {
#ifndef MULTICORE
        if (_PSD_afterslow2_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _PSD_afterslow2_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_PSD_afterslow2_var._position_relative, _PSD_afterslow2_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_PSD_monitor_trace(&_PSD_afterslow2_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Lmon_afterslow2
    if (!ABSORBED && _particle->_index == 20) {
#ifndef MULTICORE
        if (_Lmon_afterslow2_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Lmon_afterslow2_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Lmon_afterslow2_var._position_relative, _Lmon_afterslow2_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_L_monitor_trace(&_Lmon_afterslow2_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // TOFL_afterslow2
    if (!ABSORBED && _particle->_index == 21) {
#ifndef MULTICORE
        if (_TOFL_afterslow2_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _TOFL_afterslow2_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_TOFL_afterslow2_var._position_relative, _TOFL_afterslow2_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_TOFLambda_monitor_trace(&_TOFL_afterslow2_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Guidelong2
    if (!ABSORBED && _particle->_index == 22) {
#ifndef MULTICORE
        if (_Guidelong2_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Guidelong2_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Guidelong2_var._position_relative, _Guidelong2_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Guide_trace(&_Guidelong2_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Lmon_beforeballistic
    if (!ABSORBED && _particle->_index == 23) {
#ifndef MULTICORE
        if (_Lmon_beforeballistic_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Lmon_beforeballistic_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Lmon_beforeballistic_var._position_relative, _Lmon_beforeballistic_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_L_monitor_trace(&_Lmon_beforeballistic_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // PSD_beforeballistic
    if (!ABSORBED && _particle->_index == 24) {
#ifndef MULTICORE
        if (_PSD_beforeballistic_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _PSD_beforeballistic_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_PSD_beforeballistic_var._position_relative, _PSD_beforeballistic_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_PSD_monitor_trace(&_PSD_beforeballistic_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Guidelong2a
    if (!ABSORBED && _particle->_index == 25) {
#ifndef MULTICORE
        if (_Guidelong2a_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Guidelong2a_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Guidelong2a_var._position_relative, _Guidelong2a_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Guide_trace(&_Guidelong2a_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Lmonfast2
    if (!ABSORBED && _particle->_index == 26) {
#ifndef MULTICORE
        if (_Lmonfast2_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Lmonfast2_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Lmonfast2_var._position_relative, _Lmonfast2_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_L_monitor_trace(&_Lmonfast2_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Lmonfast2_zoom
    if (!ABSORBED && _particle->_index == 27) {
#ifndef MULTICORE
        if (_Lmonfast2_zoom_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Lmonfast2_zoom_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Lmonfast2_zoom_var._position_relative, _Lmonfast2_zoom_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_L_monitor_trace(&_Lmonfast2_zoom_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // TOFLfast2
    if (!ABSORBED && _particle->_index == 28) {
#ifndef MULTICORE
        if (_TOFLfast2_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _TOFLfast2_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_TOFLfast2_var._position_relative, _TOFLfast2_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_TOFLambda_monitor_trace(&_TOFLfast2_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // TOFLfast2zoom
    if (!ABSORBED && _particle->_index == 29) {
#ifndef MULTICORE
        if (_TOFLfast2zoom_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _TOFLfast2zoom_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_TOFLfast2zoom_var._position_relative, _TOFLfast2zoom_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_TOFLambda_monitor_trace(&_TOFLfast2zoom_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // PSDfast2
    if (!ABSORBED && _particle->_index == 30) {
#ifndef MULTICORE
        if (_PSDfast2_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _PSDfast2_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_PSDfast2_var._position_relative, _PSDfast2_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_PSD_monitor_trace(&_PSDfast2_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Fastchop2
    if (!ABSORBED && _particle->_index == 31) {
#ifndef MULTICORE
        if (_Fastchop2_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Fastchop2_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Fastchop2_var._position_relative, _Fastchop2_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_DiskChopper_trace(&_Fastchop2_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Fastchop2counter
    if (!ABSORBED && _particle->_index == 32) {
#ifndef MULTICORE
        if (_Fastchop2counter_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Fastchop2counter_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Fastchop2counter_var._position_relative, _Fastchop2counter_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_DiskChopper_trace(&_Fastchop2counter_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // FOchop3
    if (!ABSORBED && _particle->_index == 33) {
#ifndef MULTICORE
        if (_FOchop3_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _FOchop3_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_FOchop3_var._position_relative, _FOchop3_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_DiskChopper_trace(&_FOchop3_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // TOFfast2_zoom
    if (!ABSORBED && _particle->_index == 34) {
#ifndef MULTICORE
        if (_TOFfast2_zoom_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _TOFfast2_zoom_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_TOFfast2_zoom_var._position_relative, _TOFfast2_zoom_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_TOF_monitor_trace(&_TOFfast2_zoom_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Lmon_afterfast2
    if (!ABSORBED && _particle->_index == 35) {
#ifndef MULTICORE
        if (_Lmon_afterfast2_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Lmon_afterfast2_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Lmon_afterfast2_var._position_relative, _Lmon_afterfast2_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_L_monitor_trace(&_Lmon_afterfast2_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // TOFL_afterfast2
    if (!ABSORBED && _particle->_index == 36) {
#ifndef MULTICORE
        if (_TOFL_afterfast2_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _TOFL_afterfast2_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_TOFL_afterfast2_var._position_relative, _TOFL_afterfast2_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_TOFLambda_monitor_trace(&_TOFL_afterfast2_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // TOFL_afterfast2_zoom
    if (!ABSORBED && _particle->_index == 37) {
#ifndef MULTICORE
        if (_TOFL_afterfast2_zoom_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _TOFL_afterfast2_zoom_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_TOFL_afterfast2_zoom_var._position_relative, _TOFL_afterfast2_zoom_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_TOFLambda_monitor_trace(&_TOFL_afterfast2_zoom_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // PSD_afterfast2
    if (!ABSORBED && _particle->_index == 38) {
#ifndef MULTICORE
        if (_PSD_afterfast2_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _PSD_afterfast2_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_PSD_afterfast2_var._position_relative, _PSD_afterfast2_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_PSD_monitor_trace(&_PSD_afterfast2_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Guidesample
    if (!ABSORBED && _particle->_index == 39) {
#ifndef MULTICORE
        if (_Guidesample_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Guidesample_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Guidesample_var._position_relative, _Guidesample_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Guide_trace(&_Guidesample_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Lmon_guideend
    if (!ABSORBED && _particle->_index == 40) {
#ifndef MULTICORE
        if (_Lmon_guideend_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Lmon_guideend_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Lmon_guideend_var._position_relative, _Lmon_guideend_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_L_monitor_trace(&_Lmon_guideend_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // PSDsample
    if (!ABSORBED && _particle->_index == 41) {
#ifndef MULTICORE
        if (_PSDsample_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _PSDsample_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_PSDsample_var._position_relative, _PSDsample_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_PSD_monitor_trace(&_PSDsample_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // TOFsample_zoom
    if (!ABSORBED && _particle->_index == 42) {
#ifndef MULTICORE
        if (_TOFsample_zoom_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _TOFsample_zoom_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_TOFsample_zoom_var._position_relative, _TOFsample_zoom_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_TOF_monitor_trace(&_TOFsample_zoom_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Esample
    if (!ABSORBED && _particle->_index == 43) {
#ifndef MULTICORE
        if (_Esample_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Esample_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Esample_var._position_relative, _Esample_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_E_monitor_trace(&_Esample_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Lmon_sample_zoom
    if (!ABSORBED && _particle->_index == 44) {
#ifndef MULTICORE
        if (_Lmon_sample_zoom_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Lmon_sample_zoom_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Lmon_sample_zoom_var._position_relative, _Lmon_sample_zoom_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_L_monitor_trace(&_Lmon_sample_zoom_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // sample
    if (!ABSORBED && _particle->_index == 45) {
#ifndef MULTICORE
        if (_sample_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _sample_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_sample_var._position_relative, _sample_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Tunneling_sample_trace(&_sample_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // detectorarm
    if (!ABSORBED && _particle->_index == 46) {
        _particle->_index++;
      }

      // TOFdetector
    if (!ABSORBED && _particle->_index == 47) {
#ifndef MULTICORE
        if (_TOFdetector_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _TOFdetector_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_TOFdetector_var._position_relative, _TOFdetector_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_TOF_monitor_trace(&_TOFdetector_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // TOFdetector_zoom
    if (!ABSORBED && _particle->_index == 48) {
#ifndef MULTICORE
        if (_TOFdetector_zoom_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _TOFdetector_zoom_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_TOFdetector_zoom_var._position_relative, _TOFdetector_zoom_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_TOF_monitor_trace(&_TOFdetector_zoom_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Edetector
    if (!ABSORBED && _particle->_index == 49) {
#ifndef MULTICORE
        if (_Edetector_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Edetector_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Edetector_var._position_relative, _Edetector_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_E_monitor_trace(&_Edetector_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // TOF2Edetector
    if (!ABSORBED && _particle->_index == 50) {
#ifndef MULTICORE
        if (_TOF2Edetector_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _TOF2Edetector_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_TOF2Edetector_var._position_relative, _TOF2Edetector_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_TOF2E_monitor_trace(&_TOF2Edetector_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

    }

    // jump to next viable seed
    seed = seed + gpu_innerloop;
  } // outer loop / particle batches

  free(particles);
  free(pbuffer);

  printf("\n");
  // if on GPU and mcdotrace just exit
  #ifdef OPENACC
  }
  #endif
} /* raytrace_all_funnel */
#endif // FUNNEL

#undef x
#undef y
#undef z
#undef vx
#undef vy
#undef vz
#undef t
#undef sx
#undef sy
#undef sz
#undef p
#undef mcgravitation
#undef mcMagnet
#undef allow_backprop
#undef _mctmp_a
#undef _mctmp_b
#undef _mctmp_c
#ifdef OPENACC
#undef strlen
#undef strcmp
#undef exit
#undef printf
#undef sprintf
#undef fprintf
#endif
#undef SCATTERED
#undef RESTORE
#undef RESTORE_NEUTRON
#undef STORE_NEUTRON
#undef ABSORBED
#undef ABSORB
#undef ABSORB0
/* *****************************************************************************
* instrument 'ESS_IN5_reprate' and components SAVE
***************************************************************************** */

_class_Progress_bar *class_Progress_bar_save(_class_Progress_bar *_comp
) {
  #define profile (_comp->_parameters.profile)
  #define percent (_comp->_parameters.percent)
  #define flag_save (_comp->_parameters.flag_save)
  #define minutes (_comp->_parameters.minutes)
  #define IntermediateCnts (_comp->_parameters.IntermediateCnts)
  #define StartTime (_comp->_parameters.StartTime)
  #define EndTime (_comp->_parameters.EndTime)
  #define CurrentTime (_comp->_parameters.CurrentTime)
  #define infostring (_comp->_parameters.infostring)
  SIG_MESSAGE("[_Origin_save] component Origin=Progress_bar() SAVE [Progress_bar:0]");

  MPI_MASTER (fprintf (stdout, "\nSave [%s]\n", instrument_name););
  if (profile && strlen (profile) && strcmp (profile, "NULL") && strcmp (profile, "0")) {
    char filename[256];
    if (!strlen (profile) || !strcmp (profile, "NULL") || !strcmp (profile, "0"))
      strcpy (filename, instrument_name);
    else
      strcpy (filename, profile);
    DETECTOR_OUT_1D ("Intensity profiler", "Component index [1]", "Intensity", "prof", 1, mcNUMCOMP, mcNUMCOMP - 1, &(instrument->counter_N[1]),
                     &(instrument->counter_P[1]), &(instrument->counter_P2[1]), filename);
  }
  #undef profile
  #undef percent
  #undef flag_save
  #undef minutes
  #undef IntermediateCnts
  #undef StartTime
  #undef EndTime
  #undef CurrentTime
  #undef infostring
  return(_comp);
} /* class_Progress_bar_save */

_class_TOF_monitor *class_TOF_monitor_save(_class_TOF_monitor *_comp
) {
  #define nt (_comp->_parameters.nt)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define tmin (_comp->_parameters.tmin)
  #define tmax (_comp->_parameters.tmax)
  #define dt (_comp->_parameters.dt)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define TOF_N (_comp->_parameters.TOF_N)
  #define TOF_p (_comp->_parameters.TOF_p)
  #define TOF_p2 (_comp->_parameters.TOF_p2)
  #define t_min (_comp->_parameters.t_min)
  #define t_max (_comp->_parameters.t_max)
  #define delta_t (_comp->_parameters.delta_t)
  SIG_MESSAGE("[_TOFmoderator_zoom_save] component TOFmoderator_zoom=TOF_monitor() SAVE [TOF_monitor:0]");

  if (!nowritefile) {
    DETECTOR_OUT_1D ("Time-of-flight monitor", "Time-of-flight [\\gms]", "Intensity", "t", t_min, t_max, nt, &TOF_N[0], &TOF_p[0], &TOF_p2[0], filename);
  }
  #undef nt
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef tmin
  #undef tmax
  #undef dt
  #undef restore_neutron
  #undef nowritefile
  #undef TOF_N
  #undef TOF_p
  #undef TOF_p2
  #undef t_min
  #undef t_max
  #undef delta_t
  return(_comp);
} /* class_TOF_monitor_save */

_class_L_monitor *class_L_monitor_save(_class_L_monitor *_comp
) {
  #define nL (_comp->_parameters.nL)
  #define filename (_comp->_parameters.filename)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Lmin (_comp->_parameters.Lmin)
  #define Lmax (_comp->_parameters.Lmax)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define L_N (_comp->_parameters.L_N)
  #define L_p (_comp->_parameters.L_p)
  #define L_p2 (_comp->_parameters.L_p2)
  SIG_MESSAGE("[_Lmon_guistart_save] component Lmon_guistart=L_monitor() SAVE [L_monitor:0]");

  if (!nowritefile) {
    DETECTOR_OUT_1D ("Wavelength monitor", "Wavelength [AA]", "Intensity", "L", Lmin, Lmax, nL, &L_N[0], &L_p[0], &L_p2[0], filename);
  }
  #undef nL
  #undef filename
  #undef nowritefile
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef Lmin
  #undef Lmax
  #undef restore_neutron
  #undef L_N
  #undef L_p
  #undef L_p2
  return(_comp);
} /* class_L_monitor_save */

_class_PSD_monitor *class_PSD_monitor_save(_class_PSD_monitor *_comp
) {
  #define nx (_comp->_parameters.nx)
  #define ny (_comp->_parameters.ny)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define PSD_N (_comp->_parameters.PSD_N)
  #define PSD_p (_comp->_parameters.PSD_p)
  #define PSD_p2 (_comp->_parameters.PSD_p2)
  SIG_MESSAGE("[_PSDslow1_save] component PSDslow1=PSD_monitor() SAVE [PSD_monitor:0]");

  if (!nowritefile) {
    DETECTOR_OUT_2D ("PSD monitor", "X position [cm]", "Y position [cm]", xmin * 100.0, xmax * 100.0, ymin * 100.0, ymax * 100.0, nx, ny, &PSD_N[0][0],
                     &PSD_p[0][0], &PSD_p2[0][0], filename);
  }
  #undef nx
  #undef ny
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef restore_neutron
  #undef nowritefile
  #undef PSD_N
  #undef PSD_p
  #undef PSD_p2
  return(_comp);
} /* class_PSD_monitor_save */

_class_TOFLambda_monitor *class_TOFLambda_monitor_save(_class_TOFLambda_monitor *_comp
) {
  #define nowritefile (_comp->_parameters.nowritefile)
  #define nL (_comp->_parameters.nL)
  #define nt (_comp->_parameters.nt)
  #define tmin (_comp->_parameters.tmin)
  #define tmax (_comp->_parameters.tmax)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Lmin (_comp->_parameters.Lmin)
  #define Lmax (_comp->_parameters.Lmax)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define TOFL_N (_comp->_parameters.TOFL_N)
  #define TOFL_p (_comp->_parameters.TOFL_p)
  #define TOFL_p2 (_comp->_parameters.TOFL_p2)
  #define tt_0 (_comp->_parameters.tt_0)
  #define tt_1 (_comp->_parameters.tt_1)
  SIG_MESSAGE("[_TOFLmon1_save] component TOFLmon1=TOFLambda_monitor() SAVE [TOFLambda_monitor:0]");

  if (!nowritefile) {
    DETECTOR_OUT_2D ("TOF-wavelength monitor", "Time-of-flight [\\gms]", "Wavelength [AA]", tmin, tmax, Lmin, Lmax, nt, nL, &TOFL_N[0][0], &TOFL_p[0][0],
                     &TOFL_p2[0][0], filename);
  }
  #undef nowritefile
  #undef nL
  #undef nt
  #undef tmin
  #undef tmax
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef Lmin
  #undef Lmax
  #undef restore_neutron
  #undef TOFL_N
  #undef TOFL_p
  #undef TOFL_p2
  #undef tt_0
  #undef tt_1
  return(_comp);
} /* class_TOFLambda_monitor_save */

_class_E_monitor *class_E_monitor_save(_class_E_monitor *_comp
) {
  #define nE (_comp->_parameters.nE)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Emin (_comp->_parameters.Emin)
  #define Emax (_comp->_parameters.Emax)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define E_N (_comp->_parameters.E_N)
  #define E_p (_comp->_parameters.E_p)
  #define E_p2 (_comp->_parameters.E_p2)
  #define S_p (_comp->_parameters.S_p)
  #define S_pE (_comp->_parameters.S_pE)
  #define S_pE2 (_comp->_parameters.S_pE2)
  SIG_MESSAGE("[_Esample_save] component Esample=E_monitor() SAVE [E_monitor:0]");

  if (!nowritefile) {
    DETECTOR_OUT_1D ("Energy monitor", "Energy [meV]", "Intensity", "E", Emin, Emax, nE, &E_N[0], &E_p[0], &E_p2[0], filename);
    if (S_p)
      printf ("<E> : %g meV , E-width : %g meV \n", S_pE / S_p, sqrt (S_pE2 / S_p - S_pE * S_pE / (S_p * S_p)));
  }
  #undef nE
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef nowritefile
  #undef xwidth
  #undef yheight
  #undef Emin
  #undef Emax
  #undef restore_neutron
  #undef E_N
  #undef E_p
  #undef E_p2
  #undef S_p
  #undef S_pE
  #undef S_pE2
  return(_comp);
} /* class_E_monitor_save */

_class_TOF2E_monitor *class_TOF2E_monitor_save(_class_TOF2E_monitor *_comp
) {
  #define nE (_comp->_parameters.nE)
  #define filename (_comp->_parameters.filename)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Emin (_comp->_parameters.Emin)
  #define Emax (_comp->_parameters.Emax)
  #define T_zero (_comp->_parameters.T_zero)
  #define L_flight (_comp->_parameters.L_flight)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define E_N (_comp->_parameters.E_N)
  #define E_p (_comp->_parameters.E_p)
  #define E_p2 (_comp->_parameters.E_p2)
  #define S_p (_comp->_parameters.S_p)
  #define S_pE (_comp->_parameters.S_pE)
  #define S_pE2 (_comp->_parameters.S_pE2)
  SIG_MESSAGE("[_TOF2Edetector_save] component TOF2Edetector=TOF2E_monitor() SAVE [TOF2E_monitor:0]");

  if (!nowritefile) {
    DETECTOR_OUT_1D ("TOF-to-Energy monitor", "Energy [meV]", "Intensity", "E", Emin, Emax, nE, &E_N[0], &E_p[0], &E_p2[0], filename);
    if (S_p)
      printf ("<E> : %g meV , E-width : %g meV \n", S_pE / S_p, sqrt (S_pE2 / S_p - S_pE * S_pE / (S_p * S_p)));
  }
  #undef nE
  #undef filename
  #undef nowritefile
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef Emin
  #undef Emax
  #undef T_zero
  #undef L_flight
  #undef restore_neutron
  #undef E_N
  #undef E_p
  #undef E_p2
  #undef S_p
  #undef S_pE
  #undef S_pE2
  return(_comp);
} /* class_TOF2E_monitor_save */



int save(FILE *handle) { /* called by mccode_main for ESS_IN5_reprate:SAVE */
  if (!handle) siminfo_init(NULL);

  /* call iteratively all components SAVE */

  class_Progress_bar_save(&_Origin_var);

  class_TOF_monitor_save(&_TOFmoderator_zoom_var);

  class_TOF_monitor_save(&_TOFmoderator_var);

  class_L_monitor_save(&_Lmon_guistart_var);

  class_L_monitor_save(&_Lmon_normalize_var);


  class_L_monitor_save(&_Lmonslow1_var);

  class_PSD_monitor_save(&_PSDslow1_var);


  class_TOFLambda_monitor_save(&_TOFLmon1_var);

  class_L_monitor_save(&_Lmon_afterslow1_var);

  class_PSD_monitor_save(&_PSD_afterslow1_var);



  class_L_monitor_save(&_Lmon_slow2_var);



  class_PSD_monitor_save(&_PSD_afterslow2_var);

  class_L_monitor_save(&_Lmon_afterslow2_var);

  class_TOFLambda_monitor_save(&_TOFL_afterslow2_var);


  class_L_monitor_save(&_Lmon_beforeballistic_var);

  class_PSD_monitor_save(&_PSD_beforeballistic_var);


  class_L_monitor_save(&_Lmonfast2_var);

  class_L_monitor_save(&_Lmonfast2_zoom_var);

  class_TOFLambda_monitor_save(&_TOFLfast2_var);

  class_TOFLambda_monitor_save(&_TOFLfast2zoom_var);

  class_PSD_monitor_save(&_PSDfast2_var);




  class_TOF_monitor_save(&_TOFfast2_zoom_var);

  class_L_monitor_save(&_Lmon_afterfast2_var);

  class_TOFLambda_monitor_save(&_TOFL_afterfast2_var);

  class_TOFLambda_monitor_save(&_TOFL_afterfast2_zoom_var);

  class_PSD_monitor_save(&_PSD_afterfast2_var);


  class_L_monitor_save(&_Lmon_guideend_var);

  class_PSD_monitor_save(&_PSDsample_var);

  class_TOF_monitor_save(&_TOFsample_zoom_var);

  class_E_monitor_save(&_Esample_var);

  class_L_monitor_save(&_Lmon_sample_zoom_var);



  class_TOF_monitor_save(&_TOFdetector_var);

  class_TOF_monitor_save(&_TOFdetector_zoom_var);

  class_E_monitor_save(&_Edetector_var);

  class_TOF2E_monitor_save(&_TOF2Edetector_var);

  if (!handle) siminfo_close(); 

  return(0);
} /* save */

/* *****************************************************************************
* instrument 'ESS_IN5_reprate' and components FINALLY
***************************************************************************** */

_class_Progress_bar *class_Progress_bar_finally(_class_Progress_bar *_comp
) {
  #define profile (_comp->_parameters.profile)
  #define percent (_comp->_parameters.percent)
  #define flag_save (_comp->_parameters.flag_save)
  #define minutes (_comp->_parameters.minutes)
  #define IntermediateCnts (_comp->_parameters.IntermediateCnts)
  #define StartTime (_comp->_parameters.StartTime)
  #define EndTime (_comp->_parameters.EndTime)
  #define CurrentTime (_comp->_parameters.CurrentTime)
  #define infostring (_comp->_parameters.infostring)
  SIG_MESSAGE("[_Origin_finally] component Origin=Progress_bar() FINALLY [Progress_bar:0]");

  time_t NowTime;
  time (&NowTime);
  fprintf (stdout, "\nFinally [%s: %s]. Time: ", instrument_name, dirname ? dirname : ".");
  if (difftime (NowTime, StartTime) < 60.0)
    fprintf (stdout, "%g [s] ", difftime (NowTime, StartTime));
  else if (difftime (NowTime, StartTime) > 3600.0)
    fprintf (stdout, "%g [h] ", difftime (NowTime, StartTime) / 3600.0);
  else
    fprintf (stdout, "%g [min] ", difftime (NowTime, StartTime) / 60.0);
  fprintf (stdout, "\n");
  #undef profile
  #undef percent
  #undef flag_save
  #undef minutes
  #undef IntermediateCnts
  #undef StartTime
  #undef EndTime
  #undef CurrentTime
  #undef infostring
  return(_comp);
} /* class_Progress_bar_finally */

_class_TOF_monitor *class_TOF_monitor_finally(_class_TOF_monitor *_comp
) {
  #define nt (_comp->_parameters.nt)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define tmin (_comp->_parameters.tmin)
  #define tmax (_comp->_parameters.tmax)
  #define dt (_comp->_parameters.dt)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define TOF_N (_comp->_parameters.TOF_N)
  #define TOF_p (_comp->_parameters.TOF_p)
  #define TOF_p2 (_comp->_parameters.TOF_p2)
  #define t_min (_comp->_parameters.t_min)
  #define t_max (_comp->_parameters.t_max)
  #define delta_t (_comp->_parameters.delta_t)
  SIG_MESSAGE("[_TOFmoderator_zoom_finally] component TOFmoderator_zoom=TOF_monitor() FINALLY [TOF_monitor:0]");

  destroy_darr1d (TOF_N);
  destroy_darr1d (TOF_p);
  destroy_darr1d (TOF_p2);
  #undef nt
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef tmin
  #undef tmax
  #undef dt
  #undef restore_neutron
  #undef nowritefile
  #undef TOF_N
  #undef TOF_p
  #undef TOF_p2
  #undef t_min
  #undef t_max
  #undef delta_t
  return(_comp);
} /* class_TOF_monitor_finally */

_class_L_monitor *class_L_monitor_finally(_class_L_monitor *_comp
) {
  #define nL (_comp->_parameters.nL)
  #define filename (_comp->_parameters.filename)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Lmin (_comp->_parameters.Lmin)
  #define Lmax (_comp->_parameters.Lmax)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define L_N (_comp->_parameters.L_N)
  #define L_p (_comp->_parameters.L_p)
  #define L_p2 (_comp->_parameters.L_p2)
  SIG_MESSAGE("[_Lmon_guistart_finally] component Lmon_guistart=L_monitor() FINALLY [L_monitor:0]");

  destroy_darr1d (L_N);
  destroy_darr1d (L_p);
  destroy_darr1d (L_p2);
  #undef nL
  #undef filename
  #undef nowritefile
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef Lmin
  #undef Lmax
  #undef restore_neutron
  #undef L_N
  #undef L_p
  #undef L_p2
  return(_comp);
} /* class_L_monitor_finally */

_class_PSD_monitor *class_PSD_monitor_finally(_class_PSD_monitor *_comp
) {
  #define nx (_comp->_parameters.nx)
  #define ny (_comp->_parameters.ny)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define PSD_N (_comp->_parameters.PSD_N)
  #define PSD_p (_comp->_parameters.PSD_p)
  #define PSD_p2 (_comp->_parameters.PSD_p2)
  SIG_MESSAGE("[_PSDslow1_finally] component PSDslow1=PSD_monitor() FINALLY [PSD_monitor:0]");

  destroy_darr2d(PSD_N);
  destroy_darr2d(PSD_p);
  destroy_darr2d(PSD_p2);
  #undef nx
  #undef ny
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef restore_neutron
  #undef nowritefile
  #undef PSD_N
  #undef PSD_p
  #undef PSD_p2
  return(_comp);
} /* class_PSD_monitor_finally */

_class_TOFLambda_monitor *class_TOFLambda_monitor_finally(_class_TOFLambda_monitor *_comp
) {
  #define nowritefile (_comp->_parameters.nowritefile)
  #define nL (_comp->_parameters.nL)
  #define nt (_comp->_parameters.nt)
  #define tmin (_comp->_parameters.tmin)
  #define tmax (_comp->_parameters.tmax)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Lmin (_comp->_parameters.Lmin)
  #define Lmax (_comp->_parameters.Lmax)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define TOFL_N (_comp->_parameters.TOFL_N)
  #define TOFL_p (_comp->_parameters.TOFL_p)
  #define TOFL_p2 (_comp->_parameters.TOFL_p2)
  #define tt_0 (_comp->_parameters.tt_0)
  #define tt_1 (_comp->_parameters.tt_1)
  SIG_MESSAGE("[_TOFLmon1_finally] component TOFLmon1=TOFLambda_monitor() FINALLY [TOFLambda_monitor:0]");

  destroy_darr2d (TOFL_N);
  destroy_darr2d (TOFL_p);
  destroy_darr2d (TOFL_p2);
  #undef nowritefile
  #undef nL
  #undef nt
  #undef tmin
  #undef tmax
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef Lmin
  #undef Lmax
  #undef restore_neutron
  #undef TOFL_N
  #undef TOFL_p
  #undef TOFL_p2
  #undef tt_0
  #undef tt_1
  return(_comp);
} /* class_TOFLambda_monitor_finally */

_class_E_monitor *class_E_monitor_finally(_class_E_monitor *_comp
) {
  #define nE (_comp->_parameters.nE)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Emin (_comp->_parameters.Emin)
  #define Emax (_comp->_parameters.Emax)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define E_N (_comp->_parameters.E_N)
  #define E_p (_comp->_parameters.E_p)
  #define E_p2 (_comp->_parameters.E_p2)
  #define S_p (_comp->_parameters.S_p)
  #define S_pE (_comp->_parameters.S_pE)
  #define S_pE2 (_comp->_parameters.S_pE2)
  SIG_MESSAGE("[_Esample_finally] component Esample=E_monitor() FINALLY [E_monitor:0]");

  destroy_darr1d (E_N);
  destroy_darr1d (E_p);
  destroy_darr1d (E_p2);
  #undef nE
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef nowritefile
  #undef xwidth
  #undef yheight
  #undef Emin
  #undef Emax
  #undef restore_neutron
  #undef E_N
  #undef E_p
  #undef E_p2
  #undef S_p
  #undef S_pE
  #undef S_pE2
  return(_comp);
} /* class_E_monitor_finally */

_class_TOF2E_monitor *class_TOF2E_monitor_finally(_class_TOF2E_monitor *_comp
) {
  #define nE (_comp->_parameters.nE)
  #define filename (_comp->_parameters.filename)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Emin (_comp->_parameters.Emin)
  #define Emax (_comp->_parameters.Emax)
  #define T_zero (_comp->_parameters.T_zero)
  #define L_flight (_comp->_parameters.L_flight)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define E_N (_comp->_parameters.E_N)
  #define E_p (_comp->_parameters.E_p)
  #define E_p2 (_comp->_parameters.E_p2)
  #define S_p (_comp->_parameters.S_p)
  #define S_pE (_comp->_parameters.S_pE)
  #define S_pE2 (_comp->_parameters.S_pE2)
  SIG_MESSAGE("[_TOF2Edetector_finally] component TOF2Edetector=TOF2E_monitor() FINALLY [TOF2E_monitor:0]");

  destroy_darr1d (E_N);
  destroy_darr1d (E_p);
  destroy_darr1d (E_p2);
  #undef nE
  #undef filename
  #undef nowritefile
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef Emin
  #undef Emax
  #undef T_zero
  #undef L_flight
  #undef restore_neutron
  #undef E_N
  #undef E_p
  #undef E_p2
  #undef S_p
  #undef S_pE
  #undef S_pE2
  return(_comp);
} /* class_TOF2E_monitor_finally */



int finally(void) { /* called by mccode_main for ESS_IN5_reprate:FINALLY */
#pragma acc update host(_source_var)
#pragma acc update host(_Origin_var)
#pragma acc update host(_TOFmoderator_zoom_var)
#pragma acc update host(_TOFmoderator_var)
#pragma acc update host(_Lmon_guistart_var)
#pragma acc update host(_Lmon_normalize_var)
#pragma acc update host(_Guide1_var)
#pragma acc update host(_Lmonslow1_var)
#pragma acc update host(_PSDslow1_var)
#pragma acc update host(_FOchop1_var)
#pragma acc update host(_TOFLmon1_var)
#pragma acc update host(_Lmon_afterslow1_var)
#pragma acc update host(_PSD_afterslow1_var)
#pragma acc update host(_Guidelong1_var)
#pragma acc update host(_Guidelong1b_var)
#pragma acc update host(_Lmon_slow2_var)
#pragma acc update host(_FOchop2_var)
#pragma acc update host(_Fastchop1_var)
#pragma acc update host(_PSD_afterslow2_var)
#pragma acc update host(_Lmon_afterslow2_var)
#pragma acc update host(_TOFL_afterslow2_var)
#pragma acc update host(_Guidelong2_var)
#pragma acc update host(_Lmon_beforeballistic_var)
#pragma acc update host(_PSD_beforeballistic_var)
#pragma acc update host(_Guidelong2a_var)
#pragma acc update host(_Lmonfast2_var)
#pragma acc update host(_Lmonfast2_zoom_var)
#pragma acc update host(_TOFLfast2_var)
#pragma acc update host(_TOFLfast2zoom_var)
#pragma acc update host(_PSDfast2_var)
#pragma acc update host(_Fastchop2_var)
#pragma acc update host(_Fastchop2counter_var)
#pragma acc update host(_FOchop3_var)
#pragma acc update host(_TOFfast2_zoom_var)
#pragma acc update host(_Lmon_afterfast2_var)
#pragma acc update host(_TOFL_afterfast2_var)
#pragma acc update host(_TOFL_afterfast2_zoom_var)
#pragma acc update host(_PSD_afterfast2_var)
#pragma acc update host(_Guidesample_var)
#pragma acc update host(_Lmon_guideend_var)
#pragma acc update host(_PSDsample_var)
#pragma acc update host(_TOFsample_zoom_var)
#pragma acc update host(_Esample_var)
#pragma acc update host(_Lmon_sample_zoom_var)
#pragma acc update host(_sample_var)
#pragma acc update host(_detectorarm_var)
#pragma acc update host(_TOFdetector_var)
#pragma acc update host(_TOFdetector_zoom_var)
#pragma acc update host(_Edetector_var)
#pragma acc update host(_TOF2Edetector_var)
#pragma acc update host(_instrument_var)

  siminfo_init(NULL);
  save(siminfo_file); /* save data when simulation ends */

  /* call iteratively all components FINALLY */

  class_Progress_bar_finally(&_Origin_var);

  class_TOF_monitor_finally(&_TOFmoderator_zoom_var);

  class_TOF_monitor_finally(&_TOFmoderator_var);

  class_L_monitor_finally(&_Lmon_guistart_var);

  class_L_monitor_finally(&_Lmon_normalize_var);


  class_L_monitor_finally(&_Lmonslow1_var);

  class_PSD_monitor_finally(&_PSDslow1_var);


  class_TOFLambda_monitor_finally(&_TOFLmon1_var);

  class_L_monitor_finally(&_Lmon_afterslow1_var);

  class_PSD_monitor_finally(&_PSD_afterslow1_var);



  class_L_monitor_finally(&_Lmon_slow2_var);



  class_PSD_monitor_finally(&_PSD_afterslow2_var);

  class_L_monitor_finally(&_Lmon_afterslow2_var);

  class_TOFLambda_monitor_finally(&_TOFL_afterslow2_var);


  class_L_monitor_finally(&_Lmon_beforeballistic_var);

  class_PSD_monitor_finally(&_PSD_beforeballistic_var);


  class_L_monitor_finally(&_Lmonfast2_var);

  class_L_monitor_finally(&_Lmonfast2_zoom_var);

  class_TOFLambda_monitor_finally(&_TOFLfast2_var);

  class_TOFLambda_monitor_finally(&_TOFLfast2zoom_var);

  class_PSD_monitor_finally(&_PSDfast2_var);




  class_TOF_monitor_finally(&_TOFfast2_zoom_var);

  class_L_monitor_finally(&_Lmon_afterfast2_var);

  class_TOFLambda_monitor_finally(&_TOFL_afterfast2_var);

  class_TOFLambda_monitor_finally(&_TOFL_afterfast2_zoom_var);

  class_PSD_monitor_finally(&_PSD_afterfast2_var);


  class_L_monitor_finally(&_Lmon_guideend_var);

  class_PSD_monitor_finally(&_PSDsample_var);

  class_TOF_monitor_finally(&_TOFsample_zoom_var);

  class_E_monitor_finally(&_Esample_var);

  class_L_monitor_finally(&_Lmon_sample_zoom_var);



  class_TOF_monitor_finally(&_TOFdetector_var);

  class_TOF_monitor_finally(&_TOFdetector_zoom_var);

  class_E_monitor_finally(&_Edetector_var);

  class_TOF2E_monitor_finally(&_TOF2Edetector_var);

  siminfo_close(); 

  return(0);
} /* finally */

/* *****************************************************************************
* instrument 'ESS_IN5_reprate' and components DISPLAY
***************************************************************************** */

  #define magnify     mcdis_magnify
  #define line        mcdis_line
  #define dashed_line mcdis_dashed_line
  #define multiline   mcdis_multiline
  #define rectangle   mcdis_rectangle
  #define box         mcdis_box
  #define circle      mcdis_circle
  #define cylinder    mcdis_cylinder
  #define sphere      mcdis_sphere
  #define cone        mcdis_cone
  #define polygon     mcdis_polygon
  #define polyhedron  mcdis_polyhedron
_class_ESS_butterfly *class_ESS_butterfly_display(_class_ESS_butterfly *_comp
) {
  #define sector (_comp->_parameters.sector)
  #define beamline (_comp->_parameters.beamline)
  #define yheight (_comp->_parameters.yheight)
  #define cold_frac (_comp->_parameters.cold_frac)
  #define target_index (_comp->_parameters.target_index)
  #define dist (_comp->_parameters.dist)
  #define focus_xw (_comp->_parameters.focus_xw)
  #define focus_yh (_comp->_parameters.focus_yh)
  #define c_performance (_comp->_parameters.c_performance)
  #define t_performance (_comp->_parameters.t_performance)
  #define Lmin (_comp->_parameters.Lmin)
  #define Lmax (_comp->_parameters.Lmax)
  #define tmax_multiplier (_comp->_parameters.tmax_multiplier)
  #define n_pulses (_comp->_parameters.n_pulses)
  #define acc_power (_comp->_parameters.acc_power)
  #define tfocus_dist (_comp->_parameters.tfocus_dist)
  #define tfocus_time (_comp->_parameters.tfocus_time)
  #define tfocus_width (_comp->_parameters.tfocus_width)
  #define ColdWidths (_comp->_parameters.ColdWidths)
  #define ThermalWidths (_comp->_parameters.ThermalWidths)
  #define ColdScalars (_comp->_parameters.ColdScalars)
  #define ThermalScalars (_comp->_parameters.ThermalScalars)
  #define Beamlines (_comp->_parameters.Beamlines)
  #define wfrac_cold (_comp->_parameters.wfrac_cold)
  #define wfrac_thermal (_comp->_parameters.wfrac_thermal)
  #define C1_x (_comp->_parameters.C1_x)
  #define C1_z (_comp->_parameters.C1_z)
  #define C2_x (_comp->_parameters.C2_x)
  #define C2_z (_comp->_parameters.C2_z)
  #define C3_x (_comp->_parameters.C3_x)
  #define C3_z (_comp->_parameters.C3_z)
  #define T1_x (_comp->_parameters.T1_x)
  #define T1_z (_comp->_parameters.T1_z)
  #define T2_x (_comp->_parameters.T2_x)
  #define T2_z (_comp->_parameters.T2_z)
  #define T3_x (_comp->_parameters.T3_x)
  #define T3_z (_comp->_parameters.T3_z)
  #define rC1_x (_comp->_parameters.rC1_x)
  #define rC1_z (_comp->_parameters.rC1_z)
  #define rC2_x (_comp->_parameters.rC2_x)
  #define rC2_z (_comp->_parameters.rC2_z)
  #define rC3_x (_comp->_parameters.rC3_x)
  #define rC3_z (_comp->_parameters.rC3_z)
  #define rT1_x (_comp->_parameters.rT1_x)
  #define rT1_z (_comp->_parameters.rT1_z)
  #define rT2_x (_comp->_parameters.rT2_x)
  #define rT2_z (_comp->_parameters.rT2_z)
  #define rT3_x (_comp->_parameters.rT3_x)
  #define rT3_z (_comp->_parameters.rT3_z)
  #define tx (_comp->_parameters.tx)
  #define ty (_comp->_parameters.ty)
  #define tz (_comp->_parameters.tz)
  #define r11 (_comp->_parameters.r11)
  #define r12 (_comp->_parameters.r12)
  #define r21 (_comp->_parameters.r21)
  #define r22 (_comp->_parameters.r22)
  #define delta_y (_comp->_parameters.delta_y)
  #define Mwidth_c (_comp->_parameters.Mwidth_c)
  #define Mwidth_t (_comp->_parameters.Mwidth_t)
  #define beamportangle (_comp->_parameters.beamportangle)
  #define w_mult (_comp->_parameters.w_mult)
  #define w_stat (_comp->_parameters.w_stat)
  #define w_focus (_comp->_parameters.w_focus)
  #define w_tfocus (_comp->_parameters.w_tfocus)
  #define w_geom_c (_comp->_parameters.w_geom_c)
  #define w_geom_t (_comp->_parameters.w_geom_t)
  #define isleft (_comp->_parameters.isleft)
  #define l_range (_comp->_parameters.l_range)
  #define cos_thermal (_comp->_parameters.cos_thermal)
  #define cos_cold (_comp->_parameters.cos_cold)
  #define orientation_angle (_comp->_parameters.orientation_angle)
  #define cx (_comp->_parameters.cx)
  #define cz (_comp->_parameters.cz)
  #define jmax (_comp->_parameters.jmax)
  #define dxC (_comp->_parameters.dxC)
  #define dxT (_comp->_parameters.dxT)
  SIG_MESSAGE("[_source_display] component source=ESS_butterfly() DISPLAY [ESS_butterfly:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);
  #ifndef OPENACC
  magnify ("");
  butterfly_geometry (delta_y, jmax, cx, cz, orientation_angle, Beamlines, tx, ty, tz, rC1_x, rC1_z, rC2_x, rC2_z, rC3_x, rC3_z, rT1_x, rT1_z, rT2_x, rT2_z,
                      rT3_x, rT3_z, r11, r12, r21, r22, focus_xw, focus_yh);
  #endif
  #undef sector
  #undef beamline
  #undef yheight
  #undef cold_frac
  #undef target_index
  #undef dist
  #undef focus_xw
  #undef focus_yh
  #undef c_performance
  #undef t_performance
  #undef Lmin
  #undef Lmax
  #undef tmax_multiplier
  #undef n_pulses
  #undef acc_power
  #undef tfocus_dist
  #undef tfocus_time
  #undef tfocus_width
  #undef ColdWidths
  #undef ThermalWidths
  #undef ColdScalars
  #undef ThermalScalars
  #undef Beamlines
  #undef wfrac_cold
  #undef wfrac_thermal
  #undef C1_x
  #undef C1_z
  #undef C2_x
  #undef C2_z
  #undef C3_x
  #undef C3_z
  #undef T1_x
  #undef T1_z
  #undef T2_x
  #undef T2_z
  #undef T3_x
  #undef T3_z
  #undef rC1_x
  #undef rC1_z
  #undef rC2_x
  #undef rC2_z
  #undef rC3_x
  #undef rC3_z
  #undef rT1_x
  #undef rT1_z
  #undef rT2_x
  #undef rT2_z
  #undef rT3_x
  #undef rT3_z
  #undef tx
  #undef ty
  #undef tz
  #undef r11
  #undef r12
  #undef r21
  #undef r22
  #undef delta_y
  #undef Mwidth_c
  #undef Mwidth_t
  #undef beamportangle
  #undef w_mult
  #undef w_stat
  #undef w_focus
  #undef w_tfocus
  #undef w_geom_c
  #undef w_geom_t
  #undef isleft
  #undef l_range
  #undef cos_thermal
  #undef cos_cold
  #undef orientation_angle
  #undef cx
  #undef cz
  #undef jmax
  #undef dxC
  #undef dxT
  return(_comp);
} /* class_ESS_butterfly_display */

_class_Progress_bar *class_Progress_bar_display(_class_Progress_bar *_comp
) {
  #define profile (_comp->_parameters.profile)
  #define percent (_comp->_parameters.percent)
  #define flag_save (_comp->_parameters.flag_save)
  #define minutes (_comp->_parameters.minutes)
  #define IntermediateCnts (_comp->_parameters.IntermediateCnts)
  #define StartTime (_comp->_parameters.StartTime)
  #define EndTime (_comp->_parameters.EndTime)
  #define CurrentTime (_comp->_parameters.CurrentTime)
  #define infostring (_comp->_parameters.infostring)
  SIG_MESSAGE("[_Origin_display] component Origin=Progress_bar() DISPLAY [Progress_bar:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);

  #undef profile
  #undef percent
  #undef flag_save
  #undef minutes
  #undef IntermediateCnts
  #undef StartTime
  #undef EndTime
  #undef CurrentTime
  #undef infostring
  return(_comp);
} /* class_Progress_bar_display */

_class_TOF_monitor *class_TOF_monitor_display(_class_TOF_monitor *_comp
) {
  #define nt (_comp->_parameters.nt)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define tmin (_comp->_parameters.tmin)
  #define tmax (_comp->_parameters.tmax)
  #define dt (_comp->_parameters.dt)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define TOF_N (_comp->_parameters.TOF_N)
  #define TOF_p (_comp->_parameters.TOF_p)
  #define TOF_p2 (_comp->_parameters.TOF_p2)
  #define t_min (_comp->_parameters.t_min)
  #define t_max (_comp->_parameters.t_max)
  #define delta_t (_comp->_parameters.delta_t)
  SIG_MESSAGE("[_TOFmoderator_zoom_display] component TOFmoderator_zoom=TOF_monitor() DISPLAY [TOF_monitor:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);

  multiline (5, (double)xmin, (double)ymin, 0.0, (double)xmax, (double)ymin, 0.0, (double)xmax, (double)ymax, 0.0, (double)xmin, (double)ymax, 0.0, (double)xmin,
             (double)ymin, 0.0);
  #undef nt
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef tmin
  #undef tmax
  #undef dt
  #undef restore_neutron
  #undef nowritefile
  #undef TOF_N
  #undef TOF_p
  #undef TOF_p2
  #undef t_min
  #undef t_max
  #undef delta_t
  return(_comp);
} /* class_TOF_monitor_display */

_class_L_monitor *class_L_monitor_display(_class_L_monitor *_comp
) {
  #define nL (_comp->_parameters.nL)
  #define filename (_comp->_parameters.filename)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Lmin (_comp->_parameters.Lmin)
  #define Lmax (_comp->_parameters.Lmax)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define L_N (_comp->_parameters.L_N)
  #define L_p (_comp->_parameters.L_p)
  #define L_p2 (_comp->_parameters.L_p2)
  SIG_MESSAGE("[_Lmon_guistart_display] component Lmon_guistart=L_monitor() DISPLAY [L_monitor:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);
  multiline (5, (double)xmin, (double)ymin, 0.0, (double)xmax, (double)ymin, 0.0, (double)xmax, (double)ymax, 0.0, (double)xmin, (double)ymax, 0.0, (double)xmin,
             (double)ymin, 0.0);
  #undef nL
  #undef filename
  #undef nowritefile
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef Lmin
  #undef Lmax
  #undef restore_neutron
  #undef L_N
  #undef L_p
  #undef L_p2
  return(_comp);
} /* class_L_monitor_display */

_class_Guide *class_Guide_display(_class_Guide *_comp
) {
  #define reflect (_comp->_parameters.reflect)
  #define w1 (_comp->_parameters.w1)
  #define h1 (_comp->_parameters.h1)
  #define w2 (_comp->_parameters.w2)
  #define h2 (_comp->_parameters.h2)
  #define l (_comp->_parameters.l)
  #define R0 (_comp->_parameters.R0)
  #define Qc (_comp->_parameters.Qc)
  #define alpha (_comp->_parameters.alpha)
  #define m (_comp->_parameters.m)
  #define W (_comp->_parameters.W)
  #define pTable (_comp->_parameters.pTable)
  #define table_present (_comp->_parameters.table_present)
  SIG_MESSAGE("[_Guide1_display] component Guide1=Guide() DISPLAY [Guide:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);
  /* V3, independent "polygons": */
  // TOP
  polygon (4, -w1 / 2.0, h1 / 2.0, 0.0, w1 / 2.0, h1 / 2.0, 0.0, w2 / 2.0, h2 / 2.0, (double)l, -w2 / 2.0, h2 / 2.0, (double)l);
  // BOTTOM
  polygon (4, -w1 / 2.0, -h1 / 2.0, 0.0, w1 / 2.0, -h1 / 2.0, 0.0, w2 / 2.0, -h2 / 2.0, (double)l, -w2 / 2.0, -h2 / 2.0, (double)l);

  // RIGHT
  polygon (4, -w1 / 2.0, h1 / 2.0, 0.0, -w1 / 2.0, -h1 / 2.0, 0.0, -w2 / 2.0, -h2 / 2.0, (double)l, -w2 / 2.0, h2 / 2.0, (double)l);

  // LEFT
  polygon (4, w1 / 2.0, h1 / 2.0, 0.0, w1 / 2.0, -h1 / 2.0, 0.0, w2 / 2.0, -h2 / 2.0, (double)l, w2 / 2.0, h2 / 2.0, (double)l);

  /* V2, draw top, bottom, sides independently: */
  // TOP
  /* multiline(5, */
  /*           -w1/2.0, h1/2.0, 0.0, */
  /*            w1/2.0, h1/2.0, 0.0, */
  /* 	     w2/2.0, h2/2.0, (double)l, */
  /*           -w2/2.0, h2/2.0, (double)l, */
  /*           -w1/2.0, h1/2.0, 0.0); */
  /* // BOTTOM */
  /* multiline(5, */
  /*           -w1/2.0, -h1/2.0, 0.0, */
  /*            w1/2.0, -h1/2.0, 0.0, */
  /* 	     w2/2.0, -h2/2.0, (double)l, */
  /*           -w2/2.0, -h2/2.0, (double)l, */
  /*           -w1/2.0, -h1/2.0, 0.0); */

  /* // RIGHT */
  /* multiline(5, */
  /*           -w1/2.0, h1/2.0, 0.0, */
  /*           -w1/2.0, -h1/2.0, 0.0, */
  /* 	    -w2/2.0, -h2/2.0, (double)l, */
  /*           -w2/2.0, h2/2.0, (double)l, */
  /* 	    -w1/2.0, h1/2.0, 0.0); */

  /* // LEFT */
  /* multiline(5, */
  /*           w1/2.0, h1/2.0, 0.0, */
  /*           w1/2.0, -h1/2.0, 0.0, */
  /* 	    w2/2.0, -h2/2.0, (double)l, */
  /*           w2/2.0, h2/2.0, (double)l, */
  /* 	    w1/2.0, h1/2.0, 0.0); */

  /* Original implementation:
  multiline(5,
            -w1/2.0, -h1/2.0, 0.0,
             w1/2.0, -h1/2.0, 0.0,
             w1/2.0,  h1/2.0, 0.0,
            -w1/2.0,  h1/2.0, 0.0,
            -w1/2.0, -h1/2.0, 0.0);
  multiline(5,
            -w2/2.0, -h2/2.0, (double)l,
             w2/2.0, -h2/2.0, (double)l,
             w2/2.0,  h2/2.0, (double)l,
            -w2/2.0,  h2/2.0, (double)l,
            -w2/2.0, -h2/2.0, (double)l);
  line(-w1/2.0, -h1/2.0, 0, -w2/2.0, -h2/2.0, (double)l);
  line( w1/2.0, -h1/2.0, 0,  w2/2.0, -h2/2.0, (double)l);
  line( w1/2.0,  h1/2.0, 0,  w2/2.0,  h2/2.0, (double)l);
  line(-w1/2.0,  h1/2.0, 0, -w2/2.0,  h2/2.0, (double)l);
  */
  #undef reflect
  #undef w1
  #undef h1
  #undef w2
  #undef h2
  #undef l
  #undef R0
  #undef Qc
  #undef alpha
  #undef m
  #undef W
  #undef pTable
  #undef table_present
  return(_comp);
} /* class_Guide_display */

_class_PSD_monitor *class_PSD_monitor_display(_class_PSD_monitor *_comp
) {
  #define nx (_comp->_parameters.nx)
  #define ny (_comp->_parameters.ny)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define PSD_N (_comp->_parameters.PSD_N)
  #define PSD_p (_comp->_parameters.PSD_p)
  #define PSD_p2 (_comp->_parameters.PSD_p2)
  SIG_MESSAGE("[_PSDslow1_display] component PSDslow1=PSD_monitor() DISPLAY [PSD_monitor:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);

  multiline (5, (double)xmin, (double)ymin, 0.0, (double)xmax, (double)ymin, 0.0, (double)xmax, (double)ymax, 0.0, (double)xmin, (double)ymax, 0.0, (double)xmin,
             (double)ymin, 0.0);
  #undef nx
  #undef ny
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef restore_neutron
  #undef nowritefile
  #undef PSD_N
  #undef PSD_p
  #undef PSD_p2
  return(_comp);
} /* class_PSD_monitor_display */

_class_DiskChopper *class_DiskChopper_display(_class_DiskChopper *_comp
) {
  #define theta_0 (_comp->_parameters.theta_0)
  #define radius (_comp->_parameters.radius)
  #define yheight (_comp->_parameters.yheight)
  #define nu (_comp->_parameters.nu)
  #define nslit (_comp->_parameters.nslit)
  #define jitter (_comp->_parameters.jitter)
  #define delay (_comp->_parameters.delay)
  #define isfirst (_comp->_parameters.isfirst)
  #define n_pulse (_comp->_parameters.n_pulse)
  #define abs_out (_comp->_parameters.abs_out)
  #define phase (_comp->_parameters.phase)
  #define xwidth (_comp->_parameters.xwidth)
  #define verbose (_comp->_parameters.verbose)
  #define Tg (_comp->_parameters.Tg)
  #define To (_comp->_parameters.To)
  #define delta_y (_comp->_parameters.delta_y)
  #define height (_comp->_parameters.height)
  #define omega (_comp->_parameters.omega)
  SIG_MESSAGE("[_FOchop1_display] component FOchop1=DiskChopper() DISPLAY [DiskChopper:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);

  int j;
  /* Arrays for storing geometry of slit/beamstop */

  circle ("xy", 0, -delta_y, 0, radius);

  /* Drawing the slit(s) */
  for (j = 0; j < nslit; j++) {
    /* Angular start/end of slit */
    double tmin = j * (2.0 * PI / nslit) - theta_0 / 2.0 + phase;
    double tmax = tmin + theta_0;
    /* Draw lines for each slit. */

    line (radius * sin (tmin), radius * cos (tmin) - delta_y, 0, (radius - height) * sin (tmin), (radius - height) * cos (tmin) - delta_y, 0);
    line ((radius - height) * sin (tmin), (radius - height) * cos (tmin) - delta_y, 0, (radius - height) * sin (tmax), (radius - height) * cos (tmax) - delta_y,
          0);
    line ((radius - height) * sin (tmax), (radius - height) * cos (tmax) - delta_y, 0, radius * sin (tmax), radius * cos (tmax) - delta_y, 0);
  }
  #undef theta_0
  #undef radius
  #undef yheight
  #undef nu
  #undef nslit
  #undef jitter
  #undef delay
  #undef isfirst
  #undef n_pulse
  #undef abs_out
  #undef phase
  #undef xwidth
  #undef verbose
  #undef Tg
  #undef To
  #undef delta_y
  #undef height
  #undef omega
  return(_comp);
} /* class_DiskChopper_display */

_class_TOFLambda_monitor *class_TOFLambda_monitor_display(_class_TOFLambda_monitor *_comp
) {
  #define nowritefile (_comp->_parameters.nowritefile)
  #define nL (_comp->_parameters.nL)
  #define nt (_comp->_parameters.nt)
  #define tmin (_comp->_parameters.tmin)
  #define tmax (_comp->_parameters.tmax)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Lmin (_comp->_parameters.Lmin)
  #define Lmax (_comp->_parameters.Lmax)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define TOFL_N (_comp->_parameters.TOFL_N)
  #define TOFL_p (_comp->_parameters.TOFL_p)
  #define TOFL_p2 (_comp->_parameters.TOFL_p2)
  #define tt_0 (_comp->_parameters.tt_0)
  #define tt_1 (_comp->_parameters.tt_1)
  SIG_MESSAGE("[_TOFLmon1_display] component TOFLmon1=TOFLambda_monitor() DISPLAY [TOFLambda_monitor:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);
  multiline (5, (double)xmin, (double)ymin, 0.0, (double)xmax, (double)ymin, 0.0, (double)xmax, (double)ymax, 0.0, (double)xmin, (double)ymax, 0.0, (double)xmin,
             (double)ymin, 0.0);
  #undef nowritefile
  #undef nL
  #undef nt
  #undef tmin
  #undef tmax
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef Lmin
  #undef Lmax
  #undef restore_neutron
  #undef TOFL_N
  #undef TOFL_p
  #undef TOFL_p2
  #undef tt_0
  #undef tt_1
  return(_comp);
} /* class_TOFLambda_monitor_display */

_class_E_monitor *class_E_monitor_display(_class_E_monitor *_comp
) {
  #define nE (_comp->_parameters.nE)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Emin (_comp->_parameters.Emin)
  #define Emax (_comp->_parameters.Emax)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define E_N (_comp->_parameters.E_N)
  #define E_p (_comp->_parameters.E_p)
  #define E_p2 (_comp->_parameters.E_p2)
  #define S_p (_comp->_parameters.S_p)
  #define S_pE (_comp->_parameters.S_pE)
  #define S_pE2 (_comp->_parameters.S_pE2)
  SIG_MESSAGE("[_Esample_display] component Esample=E_monitor() DISPLAY [E_monitor:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);

  multiline (5, (double)xmin, (double)ymin, 0.0, (double)xmax, (double)ymin, 0.0, (double)xmax, (double)ymax, 0.0, (double)xmin, (double)ymax, 0.0, (double)xmin,
             (double)ymin, 0.0);
  #undef nE
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef nowritefile
  #undef xwidth
  #undef yheight
  #undef Emin
  #undef Emax
  #undef restore_neutron
  #undef E_N
  #undef E_p
  #undef E_p2
  #undef S_p
  #undef S_pE
  #undef S_pE2
  return(_comp);
} /* class_E_monitor_display */

_class_Tunneling_sample *class_Tunneling_sample_display(_class_Tunneling_sample *_comp
) {
  #define thickness (_comp->_parameters.thickness)
  #define radius (_comp->_parameters.radius)
  #define focus_r (_comp->_parameters.focus_r)
  #define p_interact (_comp->_parameters.p_interact)
  #define f_QE (_comp->_parameters.f_QE)
  #define f_tun (_comp->_parameters.f_tun)
  #define gamma (_comp->_parameters.gamma)
  #define E_tun (_comp->_parameters.E_tun)
  #define target_x (_comp->_parameters.target_x)
  #define target_y (_comp->_parameters.target_y)
  #define target_z (_comp->_parameters.target_z)
  #define focus_xw (_comp->_parameters.focus_xw)
  #define focus_yh (_comp->_parameters.focus_yh)
  #define focus_aw (_comp->_parameters.focus_aw)
  #define focus_ah (_comp->_parameters.focus_ah)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define zdepth (_comp->_parameters.zdepth)
  #define sigma_abs (_comp->_parameters.sigma_abs)
  #define sigma_inc (_comp->_parameters.sigma_inc)
  #define Vc (_comp->_parameters.Vc)
  #define target_index (_comp->_parameters.target_index)
  #define VarsV (_comp->_parameters.VarsV)
  #define ftun (_comp->_parameters.ftun)
  #define fQE (_comp->_parameters.fQE)
  SIG_MESSAGE("[_sample_display] component sample=Tunneling_sample() DISPLAY [Tunneling_sample:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);

  if (!VarsV.isrect) {
    circle ("xz", 0, yheight / 2.0, 0, radius);
    circle ("xz", 0, -yheight / 2.0, 0, radius);
    line (-radius, -yheight / 2.0, 0, -radius, +yheight / 2.0, 0);
    line (+radius, -yheight / 2.0, 0, +radius, +yheight / 2.0, 0);
    line (0, -yheight / 2.0, -radius, 0, +yheight / 2.0, -radius);
    line (0, -yheight / 2.0, +radius, 0, +yheight / 2.0, +radius);
    if (thickness) {
      double radius_i = radius - thickness;
      circle ("xz", 0, yheight / 2.0, 0, radius_i);
      circle ("xz", 0, -yheight / 2.0, 0, radius_i);
      line (-radius_i, -yheight / 2.0, 0, -radius_i, +yheight / 2.0, 0);
      line (+radius_i, -yheight / 2.0, 0, +radius_i, +yheight / 2.0, 0);
      line (0, -yheight / 2.0, -radius_i, 0, +yheight / 2.0, -radius_i);
      line (0, -yheight / 2.0, +radius_i, 0, +yheight / 2.0, +radius_i);
    }
  } else {
    double xmin = -0.5 * xwidth;
    double xmax = 0.5 * xwidth;
    double ymin = -0.5 * yheight;
    double ymax = 0.5 * yheight;
    double zmin = -0.5 * zdepth;
    double zmax = 0.5 * zdepth;
    multiline (5, xmin, ymin, zmin, xmax, ymin, zmin, xmax, ymax, zmin, xmin, ymax, zmin, xmin, ymin, zmin);
    multiline (5, xmin, ymin, zmax, xmax, ymin, zmax, xmax, ymax, zmax, xmin, ymax, zmax, xmin, ymin, zmax);
    line (xmin, ymin, zmin, xmin, ymin, zmax);
    line (xmax, ymin, zmin, xmax, ymin, zmax);
    line (xmin, ymax, zmin, xmin, ymax, zmax);
    line (xmax, ymax, zmin, xmax, ymax, zmax);
  }
  #undef thickness
  #undef radius
  #undef focus_r
  #undef p_interact
  #undef f_QE
  #undef f_tun
  #undef gamma
  #undef E_tun
  #undef target_x
  #undef target_y
  #undef target_z
  #undef focus_xw
  #undef focus_yh
  #undef focus_aw
  #undef focus_ah
  #undef xwidth
  #undef yheight
  #undef zdepth
  #undef sigma_abs
  #undef sigma_inc
  #undef Vc
  #undef target_index
  #undef VarsV
  #undef ftun
  #undef fQE
  return(_comp);
} /* class_Tunneling_sample_display */

_class_Arm *class_Arm_display(_class_Arm *_comp
) {
  SIG_MESSAGE("[_detectorarm_display] component detectorarm=Arm() DISPLAY [Arm:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);
  /* A bit ugly; hard-coded dimensions. */

  line (0, 0, 0, 0.2, 0, 0);
  line (0, 0, 0, 0, 0.2, 0);
  line (0, 0, 0, 0, 0, 0.2);

  cone (0.2, 0, 0, 0.01, 0.02, 1, 0, 0);
  cone (0, 0.2, 0, 0.01, 0.02, 0, 1, 0);
  cone (0, 0, 0.2, 0.01, 0.02, 0, 0, 1);
  return(_comp);
} /* class_Arm_display */

_class_TOF2E_monitor *class_TOF2E_monitor_display(_class_TOF2E_monitor *_comp
) {
  #define nE (_comp->_parameters.nE)
  #define filename (_comp->_parameters.filename)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define Emin (_comp->_parameters.Emin)
  #define Emax (_comp->_parameters.Emax)
  #define T_zero (_comp->_parameters.T_zero)
  #define L_flight (_comp->_parameters.L_flight)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define E_N (_comp->_parameters.E_N)
  #define E_p (_comp->_parameters.E_p)
  #define E_p2 (_comp->_parameters.E_p2)
  #define S_p (_comp->_parameters.S_p)
  #define S_pE (_comp->_parameters.S_pE)
  #define S_pE2 (_comp->_parameters.S_pE2)
  SIG_MESSAGE("[_TOF2Edetector_display] component TOF2Edetector=TOF2E_monitor() DISPLAY [TOF2E_monitor:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);
  multiline (5, (double)xmin, (double)ymin, 0.0, (double)xmax, (double)ymin, 0.0, (double)xmax, (double)ymax, 0.0, (double)xmin, (double)ymax, 0.0, (double)xmin,
             (double)ymin, 0.0);
  #undef nE
  #undef filename
  #undef nowritefile
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef Emin
  #undef Emax
  #undef T_zero
  #undef L_flight
  #undef restore_neutron
  #undef E_N
  #undef E_p
  #undef E_p2
  #undef S_p
  #undef S_pE
  #undef S_pE2
  return(_comp);
} /* class_TOF2E_monitor_display */


  #undef magnify
  #undef line
  #undef dashed_line
  #undef multiline
  #undef rectangle
  #undef box
  #undef circle
  #undef cylinder
  #undef sphere

int display(void) { /* called by mccode_main for ESS_IN5_reprate:DISPLAY */
  printf("MCDISPLAY: start\n");

  /* call iteratively all components DISPLAY */
  class_ESS_butterfly_display(&_source_var);

  class_Progress_bar_display(&_Origin_var);

  class_TOF_monitor_display(&_TOFmoderator_zoom_var);

  class_TOF_monitor_display(&_TOFmoderator_var);

  class_L_monitor_display(&_Lmon_guistart_var);

  class_L_monitor_display(&_Lmon_normalize_var);

  class_Guide_display(&_Guide1_var);

  class_L_monitor_display(&_Lmonslow1_var);

  class_PSD_monitor_display(&_PSDslow1_var);

  class_DiskChopper_display(&_FOchop1_var);

  class_TOFLambda_monitor_display(&_TOFLmon1_var);

  class_L_monitor_display(&_Lmon_afterslow1_var);

  class_PSD_monitor_display(&_PSD_afterslow1_var);

  class_Guide_display(&_Guidelong1_var);

  class_Guide_display(&_Guidelong1b_var);

  class_L_monitor_display(&_Lmon_slow2_var);

  class_DiskChopper_display(&_FOchop2_var);

  class_DiskChopper_display(&_Fastchop1_var);

  class_PSD_monitor_display(&_PSD_afterslow2_var);

  class_L_monitor_display(&_Lmon_afterslow2_var);

  class_TOFLambda_monitor_display(&_TOFL_afterslow2_var);

  class_Guide_display(&_Guidelong2_var);

  class_L_monitor_display(&_Lmon_beforeballistic_var);

  class_PSD_monitor_display(&_PSD_beforeballistic_var);

  class_Guide_display(&_Guidelong2a_var);

  class_L_monitor_display(&_Lmonfast2_var);

  class_L_monitor_display(&_Lmonfast2_zoom_var);

  class_TOFLambda_monitor_display(&_TOFLfast2_var);

  class_TOFLambda_monitor_display(&_TOFLfast2zoom_var);

  class_PSD_monitor_display(&_PSDfast2_var);

  class_DiskChopper_display(&_Fastchop2_var);

  class_DiskChopper_display(&_Fastchop2counter_var);

  class_DiskChopper_display(&_FOchop3_var);

  class_TOF_monitor_display(&_TOFfast2_zoom_var);

  class_L_monitor_display(&_Lmon_afterfast2_var);

  class_TOFLambda_monitor_display(&_TOFL_afterfast2_var);

  class_TOFLambda_monitor_display(&_TOFL_afterfast2_zoom_var);

  class_PSD_monitor_display(&_PSD_afterfast2_var);

  class_Guide_display(&_Guidesample_var);

  class_L_monitor_display(&_Lmon_guideend_var);

  class_PSD_monitor_display(&_PSDsample_var);

  class_TOF_monitor_display(&_TOFsample_zoom_var);

  class_E_monitor_display(&_Esample_var);

  class_L_monitor_display(&_Lmon_sample_zoom_var);

  class_Tunneling_sample_display(&_sample_var);

  class_Arm_display(&_detectorarm_var);

  class_TOF_monitor_display(&_TOFdetector_var);

  class_TOF_monitor_display(&_TOFdetector_zoom_var);

  class_E_monitor_display(&_Edetector_var);

  class_TOF2E_monitor_display(&_TOF2Edetector_var);

  printf("MCDISPLAY: end\n");

  return(0);
} /* display */

void* _getvar_parameters(char* compname)
/* enables settings parameters based use of the GETPAR macro */
{
  #ifdef OPENACC
    #define strcmp(a,b) str_comp(a,b)
  #endif
  if (!strcmp(compname, "source")) return (void *) &(_source_var._parameters);
  if (!strcmp(compname, "Origin")) return (void *) &(_Origin_var._parameters);
  if (!strcmp(compname, "TOFmoderator_zoom")) return (void *) &(_TOFmoderator_zoom_var._parameters);
  if (!strcmp(compname, "TOFmoderator")) return (void *) &(_TOFmoderator_var._parameters);
  if (!strcmp(compname, "Lmon_guistart")) return (void *) &(_Lmon_guistart_var._parameters);
  if (!strcmp(compname, "Lmon_normalize")) return (void *) &(_Lmon_normalize_var._parameters);
  if (!strcmp(compname, "Guide1")) return (void *) &(_Guide1_var._parameters);
  if (!strcmp(compname, "Lmonslow1")) return (void *) &(_Lmonslow1_var._parameters);
  if (!strcmp(compname, "PSDslow1")) return (void *) &(_PSDslow1_var._parameters);
  if (!strcmp(compname, "FOchop1")) return (void *) &(_FOchop1_var._parameters);
  if (!strcmp(compname, "TOFLmon1")) return (void *) &(_TOFLmon1_var._parameters);
  if (!strcmp(compname, "Lmon_afterslow1")) return (void *) &(_Lmon_afterslow1_var._parameters);
  if (!strcmp(compname, "PSD_afterslow1")) return (void *) &(_PSD_afterslow1_var._parameters);
  if (!strcmp(compname, "Guidelong1")) return (void *) &(_Guidelong1_var._parameters);
  if (!strcmp(compname, "Guidelong1b")) return (void *) &(_Guidelong1b_var._parameters);
  if (!strcmp(compname, "Lmon_slow2")) return (void *) &(_Lmon_slow2_var._parameters);
  if (!strcmp(compname, "FOchop2")) return (void *) &(_FOchop2_var._parameters);
  if (!strcmp(compname, "Fastchop1")) return (void *) &(_Fastchop1_var._parameters);
  if (!strcmp(compname, "PSD_afterslow2")) return (void *) &(_PSD_afterslow2_var._parameters);
  if (!strcmp(compname, "Lmon_afterslow2")) return (void *) &(_Lmon_afterslow2_var._parameters);
  if (!strcmp(compname, "TOFL_afterslow2")) return (void *) &(_TOFL_afterslow2_var._parameters);
  if (!strcmp(compname, "Guidelong2")) return (void *) &(_Guidelong2_var._parameters);
  if (!strcmp(compname, "Lmon_beforeballistic")) return (void *) &(_Lmon_beforeballistic_var._parameters);
  if (!strcmp(compname, "PSD_beforeballistic")) return (void *) &(_PSD_beforeballistic_var._parameters);
  if (!strcmp(compname, "Guidelong2a")) return (void *) &(_Guidelong2a_var._parameters);
  if (!strcmp(compname, "Lmonfast2")) return (void *) &(_Lmonfast2_var._parameters);
  if (!strcmp(compname, "Lmonfast2_zoom")) return (void *) &(_Lmonfast2_zoom_var._parameters);
  if (!strcmp(compname, "TOFLfast2")) return (void *) &(_TOFLfast2_var._parameters);
  if (!strcmp(compname, "TOFLfast2zoom")) return (void *) &(_TOFLfast2zoom_var._parameters);
  if (!strcmp(compname, "PSDfast2")) return (void *) &(_PSDfast2_var._parameters);
  if (!strcmp(compname, "Fastchop2")) return (void *) &(_Fastchop2_var._parameters);
  if (!strcmp(compname, "Fastchop2counter")) return (void *) &(_Fastchop2counter_var._parameters);
  if (!strcmp(compname, "FOchop3")) return (void *) &(_FOchop3_var._parameters);
  if (!strcmp(compname, "TOFfast2_zoom")) return (void *) &(_TOFfast2_zoom_var._parameters);
  if (!strcmp(compname, "Lmon_afterfast2")) return (void *) &(_Lmon_afterfast2_var._parameters);
  if (!strcmp(compname, "TOFL_afterfast2")) return (void *) &(_TOFL_afterfast2_var._parameters);
  if (!strcmp(compname, "TOFL_afterfast2_zoom")) return (void *) &(_TOFL_afterfast2_zoom_var._parameters);
  if (!strcmp(compname, "PSD_afterfast2")) return (void *) &(_PSD_afterfast2_var._parameters);
  if (!strcmp(compname, "Guidesample")) return (void *) &(_Guidesample_var._parameters);
  if (!strcmp(compname, "Lmon_guideend")) return (void *) &(_Lmon_guideend_var._parameters);
  if (!strcmp(compname, "PSDsample")) return (void *) &(_PSDsample_var._parameters);
  if (!strcmp(compname, "TOFsample_zoom")) return (void *) &(_TOFsample_zoom_var._parameters);
  if (!strcmp(compname, "Esample")) return (void *) &(_Esample_var._parameters);
  if (!strcmp(compname, "Lmon_sample_zoom")) return (void *) &(_Lmon_sample_zoom_var._parameters);
  if (!strcmp(compname, "sample")) return (void *) &(_sample_var._parameters);
  if (!strcmp(compname, "detectorarm")) return (void *) &(_detectorarm_var._parameters);
  if (!strcmp(compname, "TOFdetector")) return (void *) &(_TOFdetector_var._parameters);
  if (!strcmp(compname, "TOFdetector_zoom")) return (void *) &(_TOFdetector_zoom_var._parameters);
  if (!strcmp(compname, "Edetector")) return (void *) &(_Edetector_var._parameters);
  if (!strcmp(compname, "TOF2Edetector")) return (void *) &(_TOF2Edetector_var._parameters);
  return 0;
}

void* _get_particle_var(char *token, _class_particle *p)
/* enables setpars based use of GET_PARTICLE_DVAR macro and similar */
{
  return 0;
}

int _getcomp_index(char* compname)
/* Enables retrieving the component position & rotation when the index is not known.
 * Component indexing into MACROS, e.g., POS_A_COMP_INDEX, are 1-based! */
{
  if (!strcmp(compname, "source")) return 1;
  if (!strcmp(compname, "Origin")) return 2;
  if (!strcmp(compname, "TOFmoderator_zoom")) return 3;
  if (!strcmp(compname, "TOFmoderator")) return 4;
  if (!strcmp(compname, "Lmon_guistart")) return 5;
  if (!strcmp(compname, "Lmon_normalize")) return 6;
  if (!strcmp(compname, "Guide1")) return 7;
  if (!strcmp(compname, "Lmonslow1")) return 8;
  if (!strcmp(compname, "PSDslow1")) return 9;
  if (!strcmp(compname, "FOchop1")) return 10;
  if (!strcmp(compname, "TOFLmon1")) return 11;
  if (!strcmp(compname, "Lmon_afterslow1")) return 12;
  if (!strcmp(compname, "PSD_afterslow1")) return 13;
  if (!strcmp(compname, "Guidelong1")) return 14;
  if (!strcmp(compname, "Guidelong1b")) return 15;
  if (!strcmp(compname, "Lmon_slow2")) return 16;
  if (!strcmp(compname, "FOchop2")) return 17;
  if (!strcmp(compname, "Fastchop1")) return 18;
  if (!strcmp(compname, "PSD_afterslow2")) return 19;
  if (!strcmp(compname, "Lmon_afterslow2")) return 20;
  if (!strcmp(compname, "TOFL_afterslow2")) return 21;
  if (!strcmp(compname, "Guidelong2")) return 22;
  if (!strcmp(compname, "Lmon_beforeballistic")) return 23;
  if (!strcmp(compname, "PSD_beforeballistic")) return 24;
  if (!strcmp(compname, "Guidelong2a")) return 25;
  if (!strcmp(compname, "Lmonfast2")) return 26;
  if (!strcmp(compname, "Lmonfast2_zoom")) return 27;
  if (!strcmp(compname, "TOFLfast2")) return 28;
  if (!strcmp(compname, "TOFLfast2zoom")) return 29;
  if (!strcmp(compname, "PSDfast2")) return 30;
  if (!strcmp(compname, "Fastchop2")) return 31;
  if (!strcmp(compname, "Fastchop2counter")) return 32;
  if (!strcmp(compname, "FOchop3")) return 33;
  if (!strcmp(compname, "TOFfast2_zoom")) return 34;
  if (!strcmp(compname, "Lmon_afterfast2")) return 35;
  if (!strcmp(compname, "TOFL_afterfast2")) return 36;
  if (!strcmp(compname, "TOFL_afterfast2_zoom")) return 37;
  if (!strcmp(compname, "PSD_afterfast2")) return 38;
  if (!strcmp(compname, "Guidesample")) return 39;
  if (!strcmp(compname, "Lmon_guideend")) return 40;
  if (!strcmp(compname, "PSDsample")) return 41;
  if (!strcmp(compname, "TOFsample_zoom")) return 42;
  if (!strcmp(compname, "Esample")) return 43;
  if (!strcmp(compname, "Lmon_sample_zoom")) return 44;
  if (!strcmp(compname, "sample")) return 45;
  if (!strcmp(compname, "detectorarm")) return 46;
  if (!strcmp(compname, "TOFdetector")) return 47;
  if (!strcmp(compname, "TOFdetector_zoom")) return 48;
  if (!strcmp(compname, "Edetector")) return 49;
  if (!strcmp(compname, "TOF2Edetector")) return 50;
  return -1;
}

/* embedding file "metadata-r.c" */

/** --- Contents of  metadata-r.c ---------------------------------------------------------------------------------- */
// Created by Gregory Tucker, Data Management Software Centre, European Spallation Source ERIC on 07/07/23.
#ifndef MCCODE_NAME
#include "metadata-r.h"
#endif

char * metadata_table_key_component(char* key){
  if (strlen(key) == 0) return NULL;
  char sep[2] = ":\0"; // matches any number of repeated colons
  // look for the separator in the provided key; strtok is allowed to modify the string, so copy it
  char * tok = malloc((strlen(key) + 1) * sizeof(char));
  if (!tok) {
    fprintf(stderr,"Error allocating token\n");
    exit(-1);
  }
  strcpy(tok, key);
  char * pch = strtok(tok, sep); // this *is* the component name (if provided) -- but we need to move the pointer
  char * comp = malloc((1 + strlen(pch)) * sizeof(char));
  if (!comp) {
    fprintf(stderr,"Error allocating comp\n");
    exit(-1);
  }
  strcpy(comp, pch);
  if (tok) free(tok);
  return comp;
}
char * metadata_table_key_literal(char * key){
  if (strlen(key) == 0) return NULL;
  char sep[3] = ":\0";
  char * tok = malloc((strlen(key) + 1 ) * sizeof(char));
  if (!tok) {
    fprintf(stderr,"Error allocating token\n");
    exit(-1);
  }
  strcpy(tok, key);
  char * pch = strtok(tok, sep); // this *is* the component name (if provided)
  if (pch) pch = strtok(NULL, sep); // either NULL or the literal name
  char * name = NULL;
  if (pch) {
    name = malloc((1 + strlen(pch)) * sizeof(char));
    if (!name) {
      fprintf(stderr,"Error allocating name\n");
	exit(-1);
    }
    strcpy(name, pch);
  }
  if (tok) free(tok);
  return name;
}
int metadata_table_defined(int no, metadata_table_t * tab, char * key){
  if (strlen(key) == 0){
    /* This is 0 instead of `no` independent of any wildcard-matching logic
     * because a caller _already_ knows `no` and can verify
     * that `key` is not "" at call-time. So returning `no` is useless.
     */
    return 0;
  }
  char * comp = metadata_table_key_component(key);
  char * name = metadata_table_key_literal(key);
  // look through the table for the matching component and literal names
  int number = 0;
  for (int i=0; i<no; ++i){
    if (!strcmp(comp, tab[i].source)){
      if (name == NULL || !strcmp(name, tab[i].name)) ++number;
    }
  }
  if (comp) free(comp);
  if (name) free(name);
  return number;
}

char * metadata_table_name(int no, metadata_table_t * tab, char *key){
    if (strlen(key) == 0){
        return NULL;
    }
    char * comp = metadata_table_key_component(key);
    char * name = metadata_table_key_literal(key);
    if (name == NULL) {
        for (int i=0; i<no; ++i){
            if (!strcmp(comp, tab[i].source)){
                name = malloc((strlen(tab[i].name) + 1) * sizeof(char));
		if (!name) {
		  fprintf(stderr,"Error allocating metadata_table_name\n");
		  exit(-1);
		}
                strcpy(name, tab[i].name);
                break;
            }
        }
    } else {
        int found=0;
        for (int i=0; i<no; ++i){
            if (!strcmp(comp, tab[i].source) && !strcmp(name, tab[i].name)) {
                found = 1;
                break;
            }
        }
        if (!found) free(name);
    }
    free(comp);
    return name;
}

char * metadata_table_type(int no, metadata_table_t * tab, char * key){
  if (strlen(key) == 0) {
    fprintf(stderr, "Unable to check type of non-existent key\n");
    exit(1);
  }
  char * comp = metadata_table_key_component(key);
  char * name = metadata_table_key_literal(key);
  if (name == NULL){
    fprintf(stderr, "Unable to check type of literal for component %s without its name\n", comp);
    free(comp);
    exit(1);
  }
  char * type = NULL;
  for (int i=0; i<no; ++i){
    if (!strcmp(comp, tab[i].source) && !strcmp(name, tab[i].name)) type = tab[i].type;
  }
  if (comp) free(comp);
  if (name) free(name);
  return type;
}

char * metadata_table_literal(int no, metadata_table_t * tab, char * key){
  if (strlen(key) == 0) {
    fprintf(stderr, "Unable to retrieve literal for non-existent key\n");
    exit(1);
  }
  char * comp = metadata_table_key_component(key);
  char * name = metadata_table_key_literal(key);
  if (name == NULL){
    fprintf(stderr, "Unable to retrieve literal for component %s without its name\n", comp);
    free(comp);
    exit(1);
  }
  char * type = NULL;
  for (int i=0; i<no; ++i){
    if (!strcmp(comp, tab[i].source) && !strcmp(name, tab[i].name)) type = tab[i].value;
  }
  if (comp) free(comp);
  if (name) free(name);
  return type;
}
void metadata_table_print_all_keys(int no, metadata_table_t * tab){
  for (int i=0; i<no; ++i){
    printf("%s::%s ", tab[i].source, tab[i].name);
  }
  printf("\n");
}
int metadata_table_print_all_components(int no, metadata_table_t * tab){
  int count = 0;
  char ** known = malloc(no * sizeof(char*));
  if (!known) {
    fprintf(stderr,"Error allocating table of known metadata\n");
    exit(-1);
  }
  for (int i=0; i<no; ++i){
    int unknown = 1;
    for (int j=0; j<count; ++j) if (!strcmp(tab[i].source, known[j])) unknown = 0;
    if (unknown) known[count++] = tab[i].source;
  }
  size_t nchar = 0;
  for (int i=0; i<count; ++i) nchar += strlen(known[i]) + 1;
  char * line = malloc((nchar + 1) * sizeof(char));
  char * linetmp = malloc((nchar + 1) * sizeof(char));
  if (!line || !linetmp) {
    fprintf(stderr,"Error allocating metadata print arrays\n");
    exit(-1);
  }
  line[0] = '\0';
  for (int i=0; i<count; ++i) sprintf(linetmp, "%s%s ", line, known[i]);
  line=linetmp;
  line[strlen(line)] = '\0'; // eat the trailing space
  printf("%s\n", line);
  free(line);
  free(linetmp);
  free(known);
  return count;
}
int metadata_table_print_component_keys(int no, metadata_table_t * tab, char * key){
  char * comp = metadata_table_key_component(key);
  char * name = metadata_table_key_literal(key);
  int count = 0;
  for (int i=0; i<no; ++i) if (!strcmp(tab[i].source, comp) && (name == NULL || !strcmp(tab[i].name, name))) {
    if (name == NULL) printf("%s ", tab[i].name);
    ++count;
  }
  if (name != NULL) printf("%d", count); // replace count by strlen(tab[i].value)?
  printf("\n");
  return count;
}
/* -------------------------------------------------------------------------------------Contents of  metadata-r.c --- */
/* End of file "metadata-r.c". */

/* embedding file "mccode_main.c" */

/*******************************************************************************
* mccode_main: McCode main() function.
*******************************************************************************/
int mccode_main(int argc, char *argv[])
{
  /*  double run_num = 0; */
  time_t  t;
  clock_t ct;

#ifdef USE_MPI
  char mpi_node_name[MPI_MAX_PROCESSOR_NAME];
  int  mpi_node_name_len;
#endif /* USE_MPI */

#ifdef MAC
  argc = ccommand(&argv);
#endif

#ifdef USE_MPI
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_node_count); /* get number of nodes */
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_node_rank);
  MPI_Comm_set_name(MPI_COMM_WORLD, instrument_name);
  MPI_Get_processor_name(mpi_node_name, &mpi_node_name_len);
#endif /* USE_MPI */

  ct = clock();

  // device and host functional RNG seed
  struct timeval tm;
  gettimeofday(&tm, NULL);
  mcseed = (long) tm.tv_sec*1000000 + tm.tv_usec;
  mcstartdate = (long)tm.tv_sec;  /* set start date before parsing options and creating sim file */
  // init global _particle.randstate for random number use
  // during init(), finally() and display(). NOTE: during trace, a local
  // "_particle" variable is present and thus used instead.
  //
  // PW: srandom deferred until init() since we did not read seed input from commandline
  //srandom(_hash(mcseed-1));

#ifdef USE_MPI
  /* *** print number of nodes *********************************************** */
  if (mpi_node_count > 1) {
    MPI_MASTER(
    printf("Simulation '%s' (%s): running on %i nodes (master is '%s', MPI version %i.%i).\n",
      instrument_name, instrument_source, mpi_node_count, mpi_node_name, MPI_VERSION, MPI_SUBVERSION);
    );
    /* share the same seed, then adapt random seed for each node */
    MPI_Bcast(&mcseed, 1, MPI_LONG, 0, MPI_COMM_WORLD); /* root sends its seed to slaves */
    mcseed += mpi_node_rank; /* make sure we use different seeds per noe */
  }
#endif /* USE_MPI */

#ifdef OPENACC
#ifdef USE_MPI
  int num_devices = acc_get_num_devices(acc_device_nvidia);
  if(num_devices>0){
    int my_device = mpi_node_rank % num_devices;
    acc_set_device_num( my_device, acc_device_nvidia );
    printf("Have found %d GPU devices on rank %d. Will use device %d.\n", num_devices, mpi_node_rank, my_device);
  }else{
    printf("There was an issue probing acc_get_num_devices, fallback to host\n");
    acc_set_device_type( acc_device_host );
  }
#endif
#endif

  /* *** parse options ******************************************************* */
  SIG_MESSAGE("[" __FILE__ "] main START");
  mcformat = getenv(FLAVOR_UPPER "_FORMAT") ?
             getenv(FLAVOR_UPPER "_FORMAT") : FLAVOR_UPPER;
  instrument_exe = argv[0]; /* store the executable path */
  /* read simulation parameters and options */
  mcparseoptions(argc, argv); /* sets output dir and format */


#ifdef USE_MPI
  if (mpi_node_count > 1) {
    /* share the same seed, then adapt random seed for each node */
    MPI_Bcast(&mcseed, 1, MPI_LONG, 0, MPI_COMM_WORLD); /* root sends its seed to slaves */
    mcseed += mpi_node_rank; /* make sure we use different seeds per node */
  }
#endif


/* *** install sig handler, but only once !! after parameters parsing ******* */
#ifndef NOSIGNALS
#ifdef SIGQUIT
  if (signal( SIGQUIT ,sighandler) == SIG_IGN)
    signal( SIGQUIT,SIG_IGN);   /* quit (ASCII FS) */
#endif
#ifdef SIGABRT
  if (signal( SIGABRT ,sighandler) == SIG_IGN)
    signal( SIGABRT,SIG_IGN);   /* used by abort, replace SIGIOT in the future */
#endif
#ifdef SIGTERM
  if (signal( SIGTERM ,sighandler) == SIG_IGN)
    signal( SIGTERM,SIG_IGN);   /* software termination signal from kill */
#endif
#ifdef SIGUSR1
  if (signal( SIGUSR1 ,sighandler) == SIG_IGN)
    signal( SIGUSR1,SIG_IGN);   /* display simulation status */
#endif
#ifdef SIGUSR2
  if (signal( SIGUSR2 ,sighandler) == SIG_IGN)
    signal( SIGUSR2,SIG_IGN);
#endif
#ifdef SIGHUP
  if (signal( SIGHUP ,sighandler) == SIG_IGN)
    signal( SIGHUP,SIG_IGN);
#endif
#ifdef SIGILL
  if (signal( SIGILL ,sighandler) == SIG_IGN)
    signal( SIGILL,SIG_IGN);    /* illegal instruction (not reset when caught) */
#endif
#ifdef SIGFPE
  if (signal( SIGFPE ,sighandler) == SIG_IGN)
    signal( SIGSEGV,SIG_IGN);    /* floating point exception */
#endif
#ifdef SIGBUS
  if (signal( SIGBUS ,sighandler) == SIG_IGN)
    signal( SIGSEGV,SIG_IGN);    /* bus error */
#endif
#ifdef SIGSEGV
  if (signal( SIGSEGV ,sighandler) == SIG_IGN)
    signal( SIGSEGV,SIG_IGN);   /* segmentation violation */
#endif
#endif /* !NOSIGNALS */


  // init executed by master/host
  siminfo_init(NULL); /* open SIM */
  SIG_MESSAGE("[" __FILE__ "] main INITIALISE");
  init();


#ifndef NOSIGNALS
#ifdef SIGINT
  if (signal( SIGINT ,sighandler) == SIG_IGN)
    signal( SIGINT,SIG_IGN);    /* interrupt (rubout) only after INIT */
#endif
#endif /* !NOSIGNALS */

/* ================ main particle generation/propagation loop ================ */
#ifdef USE_MPI
  /* sliced Ncount on each MPI node */
  mcncount = mpi_node_count > 1 ?
    floor(mcncount / mpi_node_count) :
    mcncount; /* number of rays per node */
#endif

// MT specific init, note that per-ray init is empty
#if RNG_ALG == 2
  mt_srandom(mcseed);
#endif


// main raytrace work loop
#ifndef FUNNEL
  // legacy version
  raytrace_all(mcncount, mcseed);
#else
  MPI_MASTER(
  // "funneled" version in which propagation is more parallelizable
  printf("\nNOTE: CPU COMPONENT grammar activated:\n 1) \"FUNNEL\" raytrace algorithm enabled.\n 2) Any SPLIT's are dynamically allocated based on available buffer size. \n");
	     );
  raytrace_all_funnel(mcncount, mcseed);
#endif


#ifdef USE_MPI
 /* merge run_num from MPI nodes */
  if (mpi_node_count > 1) {
  double mcrun_num_double = (double)mcrun_num;
  mc_MPI_Sum(&mcrun_num_double, 1);
  mcrun_num = (unsigned long long)mcrun_num_double;
  }
#endif


  // save/finally executed by master node/thread/host
  finally();


#ifdef USE_MPI
  MPI_Finalize();
#endif /* USE_MPI */


  return 0;
} /* mccode_main */
/* End of file "mccode_main.c". */

/* end of generated C code ./ESS_IN5_reprate.c */
