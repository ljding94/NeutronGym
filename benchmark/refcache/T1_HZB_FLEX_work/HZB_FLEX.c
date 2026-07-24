/* Automatically generated file. Do not edit. 
 * Format:     ANSI C source code
 * Creator:    McStas <http://www.mcstas.org>
 * Instrument: /opt/homebrew/Caskroom/miniconda/base/envs/mcstas/share/mcstas/resources/examples/HZB/HZB_FLEX/HZB_FLEX.instr (HZB_FLEX)
 * Date:       Fri Jul 24 15:28:09 2026
 * File:       ./HZB_FLEX.c
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
  // user variables and comp-injections:
  int  ncol_25;
  int  nrow_25;
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
  if(!str_comp("ncol_25",name)){rval=*( (double *)(&(p->ncol_25)) );s=0;}
  if(!str_comp("nrow_25",name)){rval=*( (double *)(&(p->nrow_25)) );s=0;}
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
  if(!str_comp("ncol_25",name)){rval=(void*)&(p->ncol_25);s=0;}
  if(!str_comp("nrow_25",name)){rval=(void*)&(p->nrow_25);s=0;}
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
  if(!str_comp("ncol_25",name)){memcpy(&(p->ncol_25), value, sizeof(int )); rval=0;}
  if(!str_comp("nrow_25",name)){memcpy(&(p->nrow_25), value, sizeof(int )); rval=0;}
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
  case 0: { rval=*( (double *)(&(p->ncol_25)) );s=0;break;}
  case 1: { rval=*( (double *)(&(p->nrow_25)) );s=0;break;}
  }
  if (suc!=0x0) {*suc=s;}
  return rval;
}

#pragma acc routine
void particle_uservar_init(_class_particle *p){
  p->ncol_25=0;
  p->nrow_25=0;
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
* Start of instrument 'HZB_FLEX' generated code
***************************************************************************** */

#ifdef MC_TRACE_ENABLED
int traceenabled = 1;
#else
int traceenabled = 0;
#endif
#define MCSTAS "/opt/homebrew/Caskroom/miniconda/base/envs/mcstas/share/mcstas/resources/"
int   defaultmain         = 1;
char  instrument_name[]   = "HZB_FLEX";
char  instrument_source[] = "/opt/homebrew/Caskroom/miniconda/base/envs/mcstas/share/mcstas/resources/examples/HZB/HZB_FLEX/HZB_FLEX.instr";
char *instrument_exe      = NULL; /* will be set to argv[0] in main */
char  instrument_code[]   = "Instrument HZB_FLEX source code /opt/homebrew/Caskroom/miniconda/base/envs/mcstas/share/mcstas/resources/examples/HZB/HZB_FLEX/HZB_FLEX.instr is not embedded in this executable.\n  Use --source option when running mcstas.\n";

int main(int argc, char *argv[]){return mccode_main(argc, argv);}

/* *****************************************************************************
* instrument 'HZB_FLEX' and components DECLARE
***************************************************************************** */

/* Instrument parameters: structure and a table for the initialisation
   (Used in e.g. inputparse and I/O function (e.g. detector_out) */

struct _struct_instrument_parameters {
  MCNUM kI;
  MCNUM kF;
  MCNUM wVS;
  MCNUM tilt;
  MCNUM SA;
  MCNUM A3;
  MCNUM A4;
  MCNUM L3;
  MCNUM L4;
  int Mono_flatswitch;
};
typedef struct _struct_instrument_parameters _class_instrument_parameters;

struct _instrument_struct {
  char   _name[256]; /* the name of this instrument e.g. 'HZB_FLEX' */
/* Counters per component instance */
  double counter_AbsorbProp[36]; /* absorbed events in PROP routines */
  double counter_N[36], counter_P[36], counter_P2[36]; /* event counters after each component instance */
  _class_particle _trajectory[36]; /* current trajectory for STORE/RESTORE */
/* Components position table (absolute and relative coords) */
  Coords _position_relative[36]; /* positions of all components */
  Coords _position_absolute[36];
  _class_instrument_parameters _parameters; /* instrument parameters */
} _instrument_var;
struct _instrument_struct *instrument = & _instrument_var;
#pragma acc declare create ( _instrument_var )
#pragma acc declare create ( instrument )

int numipar = 10;
struct mcinputtable_struct mcinputtable[] = {
  "kI", &(_instrument_var._parameters.kI), instr_type_double, "1.55", "",
  "kF", &(_instrument_var._parameters.kF), instr_type_double, "1.55", "",
  "wVS", &(_instrument_var._parameters.wVS), instr_type_double, "0.03", "",
  "tilt", &(_instrument_var._parameters.tilt), instr_type_double, "0", "",
  "SA", &(_instrument_var._parameters.SA), instr_type_double, "-1", "",
  "A3", &(_instrument_var._parameters.A3), instr_type_double, "0", "",
  "A4", &(_instrument_var._parameters.A4), instr_type_double, "70", "",
  "L3", &(_instrument_var._parameters.L3), instr_type_double, "1.00", "",
  "L4", &(_instrument_var._parameters.L4), instr_type_double, "1.00", "",
  "Mono_flatswitch", &(_instrument_var._parameters.Mono_flatswitch), instr_type_int, "0", "",
  NULL, NULL, instr_type_double, ""
};

struct metadata_table_struct metadata_table[] = {
  "", "", "", ""
};
int num_metadata = 0;

/* ************************************************************************** */
/*             SHARE user declarations for all components                     */
/* ************************************************************************** */

/* Shared user declarations for all components types 'Source_gen'. */
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


#ifndef SOURCE_GEN_DEF
#define SOURCE_GEN_DEF
/*******************************************************************************
* str_dup_numeric: replaces non 'valid name' chars with spaces
*******************************************************************************/
char *str_dup_numeric(char *orig)
  {
    long i;

    if (!orig || !strlen(orig)) return(NULL);

    for (i=0; i < strlen(orig); i++)
    {
      if ( (orig[i] > 122)
        || (orig[i] < 32)
        || (strchr("!\"#$%&'()*,:;<=>?@[\\]^`/ ", orig[i]) != NULL) )
      {
        orig[i] = ' ';
      }
    }
    orig[i] = '\0';
    /* now skip spaces */
    for (i=0; i < strlen(orig); i++) {
      if (*orig == ' ') orig++;
      else break;
    }

    return(orig);
  } /* str_dup_numeric */

  /* A normalised Maxwellian distribution : Integral over all l = 1 */
#pragma acc routine seq
  double SG_Maxwell(double l, double temp)
  {
    double a=949.0/temp;
    return 2*a*a*exp(-a/(l*l))/(l*l*l*l*l);
  }
#endif


/* Shared user declarations for all components types 'Guide'. */

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


/* Shared user declarations for all components types 'Guide_curved'. */


/* Shared user declarations for all components types 'Guide_tapering'. */


/* Shared user declarations for all components types 'Slit'. */
  void
  slit_print_if (int condition, char* level, char* message, char* component) {
    if (condition)
      fprintf (stderr, "Slit: %s: %s: %s\n", component, level, message);
  }
  void
  slit_error_if (int condition, char* message, char* component) {
    slit_print_if (condition, "Error", message, component);
    if (condition)
      exit (-1);
  }
  void
  slit_warning_if (int condition, char* message, char* component) {
    slit_print_if (condition, "Warning", message, component);
  }

/* Shared user declarations for all components types 'Guide_channeled'. */


/* Shared user declarations for all components types 'Monochromator_curved'. */
  #pragma acc routine
  double
  GAUSS_monocurved (double x, double mean, double rms) {
    return (exp (-((x) - (mean)) * ((x) - (mean)) / (2 * (rms) * (rms))) / (sqrt (2 * PI) * (rms)));
  }





/* ************************************************************************** */
/*             End of SHARE user declarations for all components              */
/* ************************************************************************** */


/* ********************** component definition declarations. **************** */

/* component Origin=Progress_bar() [1] DECLARE */
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

/* component Gen_Source=Source_gen() [2] DECLARE */
/* Parameter definition for component type 'Source_gen' */
struct _struct_Source_gen_parameters {
  /* Component type 'Source_gen' setting parameters */
  char flux_file[16384];
  char xdiv_file[16384];
  char ydiv_file[16384];
  MCNUM radius;
  MCNUM dist;
  MCNUM focus_xw;
  MCNUM focus_yh;
  MCNUM focus_aw;
  MCNUM focus_ah;
  MCNUM E0;
  MCNUM dE;
  MCNUM lambda0;
  MCNUM dlambda;
  MCNUM I1;
  MCNUM yheight;
  MCNUM xwidth;
  MCNUM verbose;
  MCNUM T1;
  MCNUM flux_file_perAA;
  MCNUM flux_file_log;
  MCNUM Lmin;
  MCNUM Lmax;
  MCNUM Emin;
  MCNUM Emax;
  MCNUM T2;
  MCNUM I2;
  MCNUM T3;
  MCNUM I3;
  MCNUM zdepth;
  int target_index;
  /* Component type 'Source_gen' private parameters */
  double  p_in;
  double  lambda1;
  double  lambda2;
  double  lambda3;
  t_Table  pTable;
  t_Table  pTable_x;
  t_Table  pTable_y;
  double  pTable_xmin;
  double  pTable_xmax;
  double  pTable_xsum;
  double  pTable_ymin;
  double  pTable_ymax;
  double  pTable_ysum;
  double  pTable_dxmin;
  double  pTable_dxmax;
  double  pTable_dymin;
  double  pTable_dymax;
}; /* _struct_Source_gen_parameters */
typedef struct _struct_Source_gen_parameters _class_Source_gen_parameters;

/* Parameters for component type 'Source_gen' */
struct _struct_Source_gen {
  char     _name[256]; /* e.g. Gen_Source */
  char     _type[256]; /* Source_gen */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_Source_gen_parameters _parameters;
};
typedef struct _struct_Source_gen _class_Source_gen;
_class_Source_gen _Gen_Source_var;
#pragma acc declare create ( _Gen_Source_var )

/* component NL1A_NL1B_NL2_NL3_InPile_Entrance_Window=Arm() [3] DECLARE */
/* Parameter definition for component type 'Arm' */
struct _struct_Arm_parameters {
  char Arm_has_no_parameters;
}; /* _struct_Arm_parameters */
typedef struct _struct_Arm_parameters _class_Arm_parameters;

/* Parameters for component type 'Arm' */
struct _struct_Arm {
  char     _name[256]; /* e.g. NL1A_NL1B_NL2_NL3_InPile_Entrance_Window */
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
_class_Arm _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var;
#pragma acc declare create ( _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var )

/* component NL1A_NL1B_NL2_NL3_InPile=Guide() [4] DECLARE */
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
  char     _name[256]; /* e.g. NL1A_NL1B_NL2_NL3_InPile */
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
_class_Guide _NL1A_NL1B_NL2_NL3_InPile_var;
#pragma acc declare create ( _NL1A_NL1B_NL2_NL3_InPile_var )

_class_Arm _NL1B_Straight1_Entrance_Window_var;
#pragma acc declare create ( _NL1B_Straight1_Entrance_Window_var )

_class_Guide _NL1B_Straight1_var;
#pragma acc declare create ( _NL1B_Straight1_var )

_class_Arm _NL1B_Curved_Entrance_Window_var;
#pragma acc declare create ( _NL1B_Curved_Entrance_Window_var )

/* component NL1B_Curved_Guide_1=Guide_curved() [8] DECLARE */
/* Parameter definition for component type 'Guide_curved' */
struct _struct_Guide_curved_parameters {
  /* Component type 'Guide_curved' setting parameters */
  MCNUM w1;
  MCNUM h1;
  MCNUM l;
  MCNUM R0;
  MCNUM Qc;
  MCNUM alpha;
  MCNUM m;
  MCNUM W;
  MCNUM curvature;
}; /* _struct_Guide_curved_parameters */
typedef struct _struct_Guide_curved_parameters _class_Guide_curved_parameters;

/* Parameters for component type 'Guide_curved' */
struct _struct_Guide_curved {
  char     _name[256]; /* e.g. NL1B_Curved_Guide_1 */
  char     _type[256]; /* Guide_curved */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_Guide_curved_parameters _parameters;
};
typedef struct _struct_Guide_curved _class_Guide_curved;
_class_Guide_curved _NL1B_Curved_Guide_1_var;
#pragma acc declare create ( _NL1B_Curved_Guide_1_var )

_class_Arm _NL1B_Velocity_Selector_Gap_Entrance_Window_var;
#pragma acc declare create ( _NL1B_Velocity_Selector_Gap_Entrance_Window_var )

/* component Before_selec=PSD_monitor() [10] DECLARE */
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
  char     _name[256]; /* e.g. Before_selec */
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
_class_PSD_monitor _Before_selec_var;
#pragma acc declare create ( _Before_selec_var )

/* component SELEC=Selector() [11] DECLARE */
/* Parameter definition for component type 'Selector' */
struct _struct_Selector_parameters {
  /* Component type 'Selector' setting parameters */
  MCNUM xmin;
  MCNUM xmax;
  MCNUM ymin;
  MCNUM ymax;
  MCNUM length;
  MCNUM xwidth;
  MCNUM yheight;
  MCNUM nslit;
  MCNUM d;
  MCNUM radius;
  MCNUM alpha;
  MCNUM nu;
}; /* _struct_Selector_parameters */
typedef struct _struct_Selector_parameters _class_Selector_parameters;

/* Parameters for component type 'Selector' */
struct _struct_Selector {
  char     _name[256]; /* e.g. SELEC */
  char     _type[256]; /* Selector */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_Selector_parameters _parameters;
};
typedef struct _struct_Selector _class_Selector;
_class_Selector _SELEC_var;
#pragma acc declare create ( _SELEC_var )

_class_PSD_monitor _After_selec_var;
#pragma acc declare create ( _After_selec_var )

_class_Arm _NL1B_Curved_2_Entrance_Window_var;
#pragma acc declare create ( _NL1B_Curved_2_Entrance_Window_var )

_class_Guide_curved _NL1B_Curved_Guide_2_var;
#pragma acc declare create ( _NL1B_Curved_Guide_2_var )

_class_Arm _NL1B_Straight2_Entrance_Window_var;
#pragma acc declare create ( _NL1B_Straight2_Entrance_Window_var )

_class_Guide _NL1B_Straight2_var;
#pragma acc declare create ( _NL1B_Straight2_var )

_class_Arm _NL1B_Elliptical_Entrance_Window_var;
#pragma acc declare create ( _NL1B_Elliptical_Entrance_Window_var )

/* component elliptical_piece=Guide_tapering() [18] DECLARE */
/* Parameter definition for component type 'Guide_tapering' */
struct _struct_Guide_tapering_parameters {
  /* Component type 'Guide_tapering' setting parameters */
  char option[16384];
  MCNUM w1;
  MCNUM h1;
  MCNUM l;
  MCNUM linw;
  MCNUM loutw;
  MCNUM linh;
  MCNUM louth;
  MCNUM R0;
  MCNUM Qcx;
  MCNUM Qcy;
  MCNUM alphax;
  MCNUM alphay;
  MCNUM W;
  MCNUM mx;
  MCNUM my;
  MCNUM segno;
  MCNUM curvature;
  MCNUM curvature_v;
  /* Component type 'Guide_tapering' private parameters */
  double*  w1c;
  double*  w2c;
  double*  ww;
  double*  hh;
  double*  whalf;
  double*  hhalf;
  double*  lwhalf;
  double*  lhhalf;
  double*  h1_in;
  double*  h2_out;
  double*  w1_in;
  double*  w2_out;
  double  l_seg;
  double  h12;
  double  h2;
  double  w12;
  double  w2;
  double  a_ell_q;
  double  b_ell_q;
  double  lbw;
  double  lbh;
  double  mxi;
  double  u1;
  double  u2;
  double  div1;
  double  p2_para;
  double  test;
  double  Div1;
  int  seg;
  char*  fu;
  char*  pos;
  char  file_name[1024];
  char*  ep;
  FILE*  num;
  double  rotation_h;
  double  rotation_v;
}; /* _struct_Guide_tapering_parameters */
typedef struct _struct_Guide_tapering_parameters _class_Guide_tapering_parameters;

/* Parameters for component type 'Guide_tapering' */
struct _struct_Guide_tapering {
  char     _name[256]; /* e.g. elliptical_piece */
  char     _type[256]; /* Guide_tapering */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_Guide_tapering_parameters _parameters;
};
typedef struct _struct_Guide_tapering _class_Guide_tapering;
_class_Guide_tapering _elliptical_piece_var;
#pragma acc declare create ( _elliptical_piece_var )

/* component Virtual_source=Slit() [19] DECLARE */
/* Parameter definition for component type 'Slit' */
struct _struct_Slit_parameters {
  /* Component type 'Slit' setting parameters */
  MCNUM xmin;
  MCNUM xmax;
  MCNUM ymin;
  MCNUM ymax;
  MCNUM radius;
  MCNUM xwidth;
  MCNUM yheight;
  /* Component type 'Slit' private parameters */
  char  isradial;
}; /* _struct_Slit_parameters */
typedef struct _struct_Slit_parameters _class_Slit_parameters;

/* Parameters for component type 'Slit' */
struct _struct_Slit {
  char     _name[256]; /* e.g. Virtual_source */
  char     _type[256]; /* Slit */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_Slit_parameters _parameters;
};
typedef struct _struct_Slit _class_Slit;
_class_Slit _Virtual_source_var;
#pragma acc declare create ( _Virtual_source_var )

_class_Arm _NL1B_Vertical_Guide_Entrance_Window_var;
#pragma acc declare create ( _NL1B_Vertical_Guide_Entrance_Window_var )

/* component NL1B_Vertical_Guide=Guide_channeled() [21] DECLARE */
/* Parameter definition for component type 'Guide_channeled' */
struct _struct_Guide_channeled_parameters {
  /* Component type 'Guide_channeled' setting parameters */
  MCNUM w1;
  MCNUM h1;
  MCNUM w2;
  MCNUM h2;
  MCNUM l;
  MCNUM R0;
  MCNUM Qc;
  MCNUM alpha;
  MCNUM m;
  MCNUM nslit;
  MCNUM d;
  MCNUM Qcx;
  MCNUM Qcy;
  MCNUM alphax;
  MCNUM alphay;
  MCNUM W;
  MCNUM mx;
  MCNUM my;
  MCNUM nu;
  MCNUM phase;
  /* Component type 'Guide_channeled' private parameters */
  double  w1c;
  double  w2c;
  double  ww;
  double  hh;
  double  whalf;
  double  hhalf;
  double  lwhalf;
  double  lhhalf;
}; /* _struct_Guide_channeled_parameters */
typedef struct _struct_Guide_channeled_parameters _class_Guide_channeled_parameters;

/* Parameters for component type 'Guide_channeled' */
struct _struct_Guide_channeled {
  char     _name[256]; /* e.g. NL1B_Vertical_Guide */
  char     _type[256]; /* Guide_channeled */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_Guide_channeled_parameters _parameters;
};
typedef struct _struct_Guide_channeled _class_Guide_channeled;
_class_Guide_channeled _NL1B_Vertical_Guide_var;
#pragma acc declare create ( _NL1B_Vertical_Guide_var )

_class_Arm _NL1B_Guide_Exit_var;
#pragma acc declare create ( _NL1B_Guide_Exit_var )

/* component energy_endguide=E_monitor() [23] DECLARE */
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
  char     _name[256]; /* e.g. energy_endguide */
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
_class_E_monitor _energy_endguide_var;
#pragma acc declare create ( _energy_endguide_var )

_class_Arm _Mono_center_var;
#pragma acc declare create ( _Mono_center_var )

/* component Monochromator=Monochromator_curved() [25] DECLARE */
/* Parameter definition for component type 'Monochromator_curved' */
struct _struct_Monochromator_curved_parameters {
  /* Component type 'Monochromator_curved' setting parameters */
  char reflect[16384];
  char transmit[16384];
  MCNUM zwidth;
  MCNUM yheight;
  MCNUM gap;
  int NH;
  int NV;
  MCNUM mosaich;
  MCNUM mosaicv;
  MCNUM r0;
  MCNUM t0;
  MCNUM Q;
  MCNUM RV;
  MCNUM RH;
  MCNUM DM;
  MCNUM mosaic;
  MCNUM width;
  MCNUM height;
  MCNUM verbose;
  MCNUM order;
  /* Component type 'Monochromator_curved' private parameters */
  double  mos_rms_y;
  double  mos_rms_z;
  double  mos_rms_max;
  double  mono_Q;
  double  SlabWidth;
  double  SlabHeight;
  t_Table  rTable;
  t_Table  tTable;
  int  rTableFlag;
  int  tTableFlag;
  double*  tiltH;
  double*  tiltV;
  char  ncol_var[128];
  char  nrow_var[128];
}; /* _struct_Monochromator_curved_parameters */
typedef struct _struct_Monochromator_curved_parameters _class_Monochromator_curved_parameters;

/* Parameters for component type 'Monochromator_curved' */
struct _struct_Monochromator_curved {
  char     _name[256]; /* e.g. Monochromator */
  char     _type[256]; /* Monochromator_curved */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_Monochromator_curved_parameters _parameters;
};
typedef struct _struct_Monochromator_curved _class_Monochromator_curved;
_class_Monochromator_curved _Monochromator_var;
#pragma acc declare create ( _Monochromator_var )

_class_Arm _Mono_sample_arm_var;
#pragma acc declare create ( _Mono_sample_arm_var )

_class_E_monitor _energy_mono_var;
#pragma acc declare create ( _energy_mono_var )

_class_E_monitor _energy_pre_sample_var;
#pragma acc declare create ( _energy_pre_sample_var )

_class_Arm _Sample_center_var;
#pragma acc declare create ( _Sample_center_var )

_class_Arm _Sample_analyser_arm_var;
#pragma acc declare create ( _Sample_analyser_arm_var )

/* component div_mono=Divergence_monitor() [31] DECLARE */
/* Parameter definition for component type 'Divergence_monitor' */
struct _struct_Divergence_monitor_parameters {
  /* Component type 'Divergence_monitor' setting parameters */
  int nh;
  int nv;
  char filename[16384];
  MCNUM xmin;
  MCNUM xmax;
  MCNUM ymin;
  MCNUM ymax;
  int nowritefile;
  MCNUM xwidth;
  MCNUM yheight;
  MCNUM maxdiv_h;
  MCNUM maxdiv_v;
  int restore_neutron;
  MCNUM nx;
  MCNUM ny;
  MCNUM nz;
  /* Component type 'Divergence_monitor' private parameters */
  DArray2d  Div_N;
  DArray2d  Div_p;
  DArray2d  Div_p2;
}; /* _struct_Divergence_monitor_parameters */
typedef struct _struct_Divergence_monitor_parameters _class_Divergence_monitor_parameters;

/* Parameters for component type 'Divergence_monitor' */
struct _struct_Divergence_monitor {
  char     _name[256]; /* e.g. div_mono */
  char     _type[256]; /* Divergence_monitor */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_Divergence_monitor_parameters _parameters;
};
typedef struct _struct_Divergence_monitor _class_Divergence_monitor;
_class_Divergence_monitor _div_mono_var;
#pragma acc declare create ( _div_mono_var )

/* component div_mono_H=Div1D_monitor() [32] DECLARE */
/* Parameter definition for component type 'Div1D_monitor' */
struct _struct_Div1D_monitor_parameters {
  /* Component type 'Div1D_monitor' setting parameters */
  int ndiv;
  char filename[16384];
  MCNUM xmin;
  MCNUM xmax;
  MCNUM ymin;
  MCNUM ymax;
  MCNUM xwidth;
  MCNUM yheight;
  MCNUM maxdiv;
  int restore_neutron;
  int nowritefile;
  int vertical;
  /* Component type 'Div1D_monitor' private parameters */
  DArray1d  Div_N;
  DArray1d  Div_p;
  DArray1d  Div_p2;
}; /* _struct_Div1D_monitor_parameters */
typedef struct _struct_Div1D_monitor_parameters _class_Div1D_monitor_parameters;

/* Parameters for component type 'Div1D_monitor' */
struct _struct_Div1D_monitor {
  char     _name[256]; /* e.g. div_mono_H */
  char     _type[256]; /* Div1D_monitor */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_Div1D_monitor_parameters _parameters;
};
typedef struct _struct_Div1D_monitor _class_Div1D_monitor;
_class_Div1D_monitor _div_mono_H_var;
#pragma acc declare create ( _div_mono_H_var )

_class_PSD_monitor _psd_sam_var;
#pragma acc declare create ( _psd_sam_var )

/* component dpsd1=PSDlin_monitor() [34] DECLARE */
/* Parameter definition for component type 'PSDlin_monitor' */
struct _struct_PSDlin_monitor_parameters {
  /* Component type 'PSDlin_monitor' setting parameters */
  int nbins;
  char filename[16384];
  MCNUM xmin;
  MCNUM xmax;
  MCNUM ymin;
  MCNUM ymax;
  int nowritefile;
  MCNUM xwidth;
  MCNUM yheight;
  int restore_neutron;
  int vertical;
  /* Component type 'PSDlin_monitor' private parameters */
  DArray1d  PSDlin_N;
  DArray1d  PSDlin_p;
  DArray1d  PSDlin_p2;
}; /* _struct_PSDlin_monitor_parameters */
typedef struct _struct_PSDlin_monitor_parameters _class_PSDlin_monitor_parameters;

/* Parameters for component type 'PSDlin_monitor' */
struct _struct_PSDlin_monitor {
  char     _name[256]; /* e.g. dpsd1 */
  char     _type[256]; /* PSDlin_monitor */
  long     _index; /* e.g. 2 index in TRACE list */
  Coords   _position_absolute;
  Coords   _position_relative; /* wrt PREVIOUS */
  Rotation _rotation_absolute;
  Rotation _rotation_relative; /* wrt PREVIOUS */
  int      _rotation_is_identity;
  int      _position_relative_is_zero;
  _class_PSDlin_monitor_parameters _parameters;
};
typedef struct _struct_PSDlin_monitor _class_PSDlin_monitor;
_class_PSDlin_monitor _dpsd1_var;
#pragma acc declare create ( _dpsd1_var )

int mcNUMCOMP = 34;

/* User declarations from instrument definition. Can define functions. */

// unit conversions
double m_n=1.674928e-27;     // in kg
double hbar=1.054572e-34;    // in J sec
double meV_to_SI=1.6022e-22; // conversion
double SI_to_meV=6.2414e21;

// guide coating properties
double MGUIDE=3.2;
double W_para = 0.0025;
double R0_para = 0.99;
double Qc_para = 0.0217;
double alpha_para=3.90;

// curved guide properties
double LGUIDE_1=18.65;
double LGUIDE_2=17.8;
double beta_guide_1, s_guide_1;
double beta_guide_2, s_guide_2;

// monochromator properties
double RMH, RMV, rho_MH, rho_MV;
double lambdaI, EI;

// analyser properties
// double RAH, RAV, rho_AH, rho_AV;
// rho_AV=1.00 for kf=0.952
// rho_AV=1.23 for kf=1.55
// rho_AV=1.40 for kf=2.62
// rho_AV=1.97 for kf=3.696
double RAH, rho_AV=1.23, RAV;
double lambdaF, EF;

// angles
double A1, A2, A5, A6;

// distances
double L1=1.75;
double L2=2.2; // This is an actual variable in the real experiment
// double L3=1.10;
// double L4=1.00;


//velocity selector frequency
double SelFreq;

// monitor/source properties
double dlambdaI,dkI,dEI,l_min,l_max,e_min,e_max;


#undef compcurname
#undef compcurtype
#undef compcurindex
/* end of instrument 'HZB_FLEX' and components DECLARE */

/* *****************************************************************************
* instrument 'HZB_FLEX' and components INITIALISE
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

/* component Origin=Progress_bar() SETTING, POSITION/ROTATION */
int _Origin_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Origin_setpos] component Origin=Progress_bar() SETTING [Progress_bar:0]");
  stracpy(_Origin_var._name, "Origin", 16384);
  stracpy(_Origin_var._type, "Progress_bar", 16384);
  _Origin_var._index=1;
  int current_setpos_index = 1;
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
    rot_copy(_Origin_var._rotation_relative, _Origin_var._rotation_absolute);
    _Origin_var._rotation_is_identity =  rot_test_identity(_Origin_var._rotation_relative);
    _Origin_var._position_absolute = coords_set(
      0, 0, 0);
    tc1 = coords_neg(_Origin_var._position_absolute);
    _Origin_var._position_relative = rot_apply(_Origin_var._rotation_absolute, tc1);
  } /* Origin=Progress_bar() AT ROTATED */
  DEBUG_COMPONENT("Origin", _Origin_var._position_absolute, _Origin_var._rotation_absolute);
  instrument->_position_absolute[1] = _Origin_var._position_absolute;
  instrument->_position_relative[1] = _Origin_var._position_relative;
    _Origin_var._position_relative_is_zero =  coords_test_zero(_Origin_var._position_relative);
  instrument->counter_N[1]  = instrument->counter_P[1] = instrument->counter_P2[1] = 0;
  instrument->counter_AbsorbProp[1]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0000_Origin", _Origin_var._position_absolute, _Origin_var._rotation_absolute, "Progress_bar");
        mccomp_param_nexus(nxhandle,"0000_Origin", "profile", "NULL", "NULL", "char*");
        mccomp_param_nexus(nxhandle,"0000_Origin", "percent", "10", "10","MCNUM");
        mccomp_param_nexus(nxhandle,"0000_Origin", "flag_save", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0000_Origin", "minutes", "0", "0","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Origin_setpos */

/* component Gen_Source=Source_gen() SETTING, POSITION/ROTATION */
int _Gen_Source_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Gen_Source_setpos] component Gen_Source=Source_gen() SETTING [Source_gen:0]");
  stracpy(_Gen_Source_var._name, "Gen_Source", 16384);
  stracpy(_Gen_Source_var._type, "Source_gen", 16384);
  _Gen_Source_var._index=2;
  int current_setpos_index = 2;
  if("NULL" && strlen("NULL"))
    stracpy(_Gen_Source_var._parameters.flux_file, "NULL" ? "NULL" : "", 16384);
  else 
  _Gen_Source_var._parameters.flux_file[0]='\0';
  if("NULL" && strlen("NULL"))
    stracpy(_Gen_Source_var._parameters.xdiv_file, "NULL" ? "NULL" : "", 16384);
  else 
  _Gen_Source_var._parameters.xdiv_file[0]='\0';
  if("NULL" && strlen("NULL"))
    stracpy(_Gen_Source_var._parameters.ydiv_file, "NULL" ? "NULL" : "", 16384);
  else 
  _Gen_Source_var._parameters.ydiv_file[0]='\0';
  _Gen_Source_var._parameters.radius = 0.0775;
  _Gen_Source_var._parameters.dist = 1.53;
  _Gen_Source_var._parameters.focus_xw = 0.2;
  _Gen_Source_var._parameters.focus_yh = 0.125;
  _Gen_Source_var._parameters.focus_aw = 0;
  _Gen_Source_var._parameters.focus_ah = 0;
  _Gen_Source_var._parameters.E0 = 0;
  _Gen_Source_var._parameters.dE = 0;
  _Gen_Source_var._parameters.lambda0 = 0;
  _Gen_Source_var._parameters.dlambda = 0;
  _Gen_Source_var._parameters.I1 = 1e10;
  _Gen_Source_var._parameters.yheight = 0.1;
  _Gen_Source_var._parameters.xwidth = 0.1;
  _Gen_Source_var._parameters.verbose = 0;
  _Gen_Source_var._parameters.T1 = 45;
  _Gen_Source_var._parameters.flux_file_perAA = 0;
  _Gen_Source_var._parameters.flux_file_log = 0;
  _Gen_Source_var._parameters.Lmin = l_min;
  _Gen_Source_var._parameters.Lmax = l_max;
  _Gen_Source_var._parameters.Emin = 0;
  _Gen_Source_var._parameters.Emax = 0;
  _Gen_Source_var._parameters.T2 = 0;
  _Gen_Source_var._parameters.I2 = 0;
  _Gen_Source_var._parameters.T3 = 0;
  _Gen_Source_var._parameters.I3 = 0;
  _Gen_Source_var._parameters.zdepth = 0;
  _Gen_Source_var._parameters.target_index = 1;


  /* component Gen_Source=Source_gen() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (0)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _Origin_var._rotation_absolute, _Gen_Source_var._rotation_absolute);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    rot_mul(_Gen_Source_var._rotation_absolute, tr1, _Gen_Source_var._rotation_relative);
    _Gen_Source_var._rotation_is_identity =  rot_test_identity(_Gen_Source_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Gen_Source_var._position_absolute = coords_add(_Origin_var._position_absolute, tc2);
    tc1 = coords_sub(_Origin_var._position_absolute, _Gen_Source_var._position_absolute);
    _Gen_Source_var._position_relative = rot_apply(_Gen_Source_var._rotation_absolute, tc1);
  } /* Gen_Source=Source_gen() AT ROTATED */
  DEBUG_COMPONENT("Gen_Source", _Gen_Source_var._position_absolute, _Gen_Source_var._rotation_absolute);
  instrument->_position_absolute[2] = _Gen_Source_var._position_absolute;
  instrument->_position_relative[2] = _Gen_Source_var._position_relative;
    _Gen_Source_var._position_relative_is_zero =  coords_test_zero(_Gen_Source_var._position_relative);
  instrument->counter_N[2]  = instrument->counter_P[2] = instrument->counter_P2[2] = 0;
  instrument->counter_AbsorbProp[2]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0001_Gen_Source", _Gen_Source_var._position_absolute, _Gen_Source_var._rotation_absolute, "Source_gen");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "flux_file", "NULL", "NULL", "char*");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "xdiv_file", "NULL", "NULL", "char*");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "ydiv_file", "NULL", "NULL", "char*");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "radius", "0.0", "0.0775","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "dist", "0", "1.53","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "focus_xw", "0.045", "0.2","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "focus_yh", "0.12", "0.125","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "focus_aw", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "focus_ah", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "E0", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "dE", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "lambda0", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "dlambda", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "I1", "1", "1e10","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "yheight", "0.1", "0.1","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "xwidth", "0.1", "0.1","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "verbose", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "T1", "0", "45","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "flux_file_perAA", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "flux_file_log", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "Lmin", "0", "l_min","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "Lmax", "0", "l_max","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "Emin", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "Emax", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "T2", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "I2", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "T3", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "I3", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "zdepth", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0001_Gen_Source", "target_index", "1", "1","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Gen_Source_setpos */

/* component NL1A_NL1B_NL2_NL3_InPile_Entrance_Window=Arm() SETTING, POSITION/ROTATION */
int _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_setpos] component NL1A_NL1B_NL2_NL3_InPile_Entrance_Window=Arm() SETTING [Arm:0]");
  stracpy(_NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._name, "NL1A_NL1B_NL2_NL3_InPile_Entrance_Window", 16384);
  stracpy(_NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._type, "Arm", 16384);
  _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._index=3;
  int current_setpos_index = 3;
  /* component NL1A_NL1B_NL2_NL3_InPile_Entrance_Window=Arm() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (0)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _Origin_var._rotation_absolute, _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._rotation_absolute);
    rot_transpose(_Gen_Source_var._rotation_absolute, tr1);
    rot_mul(_NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._rotation_absolute, tr1, _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._rotation_relative);
    _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._rotation_is_identity =  rot_test_identity(_NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1.530);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._position_absolute = coords_add(_Origin_var._position_absolute, tc2);
    tc1 = coords_sub(_Gen_Source_var._position_absolute, _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._position_absolute);
    _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._position_relative = rot_apply(_NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._rotation_absolute, tc1);
  } /* NL1A_NL1B_NL2_NL3_InPile_Entrance_Window=Arm() AT ROTATED */
  DEBUG_COMPONENT("NL1A_NL1B_NL2_NL3_InPile_Entrance_Window", _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._position_absolute, _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._rotation_absolute);
  instrument->_position_absolute[3] = _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._position_absolute;
  instrument->_position_relative[3] = _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._position_relative;
    _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._position_relative_is_zero =  coords_test_zero(_NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._position_relative);
  instrument->counter_N[3]  = instrument->counter_P[3] = instrument->counter_P2[3] = 0;
  instrument->counter_AbsorbProp[3]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0002_NL1A_NL1B_NL2_NL3_InPile_Entrance_Window", _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._position_absolute, _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._rotation_absolute, "Arm");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_setpos */

/* component NL1A_NL1B_NL2_NL3_InPile=Guide() SETTING, POSITION/ROTATION */
int _NL1A_NL1B_NL2_NL3_InPile_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_NL1A_NL1B_NL2_NL3_InPile_setpos] component NL1A_NL1B_NL2_NL3_InPile=Guide() SETTING [Guide:0]");
  stracpy(_NL1A_NL1B_NL2_NL3_InPile_var._name, "NL1A_NL1B_NL2_NL3_InPile", 16384);
  stracpy(_NL1A_NL1B_NL2_NL3_InPile_var._type, "Guide", 16384);
  _NL1A_NL1B_NL2_NL3_InPile_var._index=4;
  int current_setpos_index = 4;
  _NL1A_NL1B_NL2_NL3_InPile_var._parameters.reflect[0]='\0';
  _NL1A_NL1B_NL2_NL3_InPile_var._parameters.w1 = 0.170696;
  _NL1A_NL1B_NL2_NL3_InPile_var._parameters.h1 = 0.129;
  _NL1A_NL1B_NL2_NL3_InPile_var._parameters.w2 = 0.340522;
  _NL1A_NL1B_NL2_NL3_InPile_var._parameters.h2 = 0.129;
  _NL1A_NL1B_NL2_NL3_InPile_var._parameters.l = 1.870;
  _NL1A_NL1B_NL2_NL3_InPile_var._parameters.R0 = R0_para;
  _NL1A_NL1B_NL2_NL3_InPile_var._parameters.Qc = Qc_para;
  _NL1A_NL1B_NL2_NL3_InPile_var._parameters.alpha = alpha_para;
  _NL1A_NL1B_NL2_NL3_InPile_var._parameters.m = MGUIDE;
  _NL1A_NL1B_NL2_NL3_InPile_var._parameters.W = W_para;


  /* component NL1A_NL1B_NL2_NL3_InPile=Guide() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (0)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._rotation_absolute, _NL1A_NL1B_NL2_NL3_InPile_var._rotation_absolute);
    rot_transpose(_Gen_Source_var._rotation_absolute, tr1);
    rot_mul(_NL1A_NL1B_NL2_NL3_InPile_var._rotation_absolute, tr1, _NL1A_NL1B_NL2_NL3_InPile_var._rotation_relative);
    _NL1A_NL1B_NL2_NL3_InPile_var._rotation_is_identity =  rot_test_identity(_NL1A_NL1B_NL2_NL3_InPile_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0);
    rot_transpose(_NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _NL1A_NL1B_NL2_NL3_InPile_var._position_absolute = coords_add(_NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._position_absolute, tc2);
    tc1 = coords_sub(_Gen_Source_var._position_absolute, _NL1A_NL1B_NL2_NL3_InPile_var._position_absolute);
    _NL1A_NL1B_NL2_NL3_InPile_var._position_relative = rot_apply(_NL1A_NL1B_NL2_NL3_InPile_var._rotation_absolute, tc1);
  } /* NL1A_NL1B_NL2_NL3_InPile=Guide() AT ROTATED */
  DEBUG_COMPONENT("NL1A_NL1B_NL2_NL3_InPile", _NL1A_NL1B_NL2_NL3_InPile_var._position_absolute, _NL1A_NL1B_NL2_NL3_InPile_var._rotation_absolute);
  instrument->_position_absolute[4] = _NL1A_NL1B_NL2_NL3_InPile_var._position_absolute;
  instrument->_position_relative[4] = _NL1A_NL1B_NL2_NL3_InPile_var._position_relative;
    _NL1A_NL1B_NL2_NL3_InPile_var._position_relative_is_zero =  coords_test_zero(_NL1A_NL1B_NL2_NL3_InPile_var._position_relative);
  instrument->counter_N[4]  = instrument->counter_P[4] = instrument->counter_P2[4] = 0;
  instrument->counter_AbsorbProp[4]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0003_NL1A_NL1B_NL2_NL3_InPile", _NL1A_NL1B_NL2_NL3_InPile_var._position_absolute, _NL1A_NL1B_NL2_NL3_InPile_var._rotation_absolute, "Guide");
        mccomp_param_nexus(nxhandle,"0003_NL1A_NL1B_NL2_NL3_InPile", "reflect", 0, 0, "char*");
        mccomp_param_nexus(nxhandle,"0003_NL1A_NL1B_NL2_NL3_InPile", "w1", "NONE", "0.170696","MCNUM");
        mccomp_param_nexus(nxhandle,"0003_NL1A_NL1B_NL2_NL3_InPile", "h1", "NONE", "0.129","MCNUM");
        mccomp_param_nexus(nxhandle,"0003_NL1A_NL1B_NL2_NL3_InPile", "w2", "0", "0.340522","MCNUM");
        mccomp_param_nexus(nxhandle,"0003_NL1A_NL1B_NL2_NL3_InPile", "h2", "0", "0.129","MCNUM");
        mccomp_param_nexus(nxhandle,"0003_NL1A_NL1B_NL2_NL3_InPile", "l", "NONE", "1.870","MCNUM");
        mccomp_param_nexus(nxhandle,"0003_NL1A_NL1B_NL2_NL3_InPile", "R0", "0.99", "R0_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0003_NL1A_NL1B_NL2_NL3_InPile", "Qc", "0.0219", "Qc_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0003_NL1A_NL1B_NL2_NL3_InPile", "alpha", "6.07", "alpha_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0003_NL1A_NL1B_NL2_NL3_InPile", "m", "2", "MGUIDE","MCNUM");
        mccomp_param_nexus(nxhandle,"0003_NL1A_NL1B_NL2_NL3_InPile", "W", "0.003", "W_para","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _NL1A_NL1B_NL2_NL3_InPile_setpos */

/* component NL1B_Straight1_Entrance_Window=Arm() SETTING, POSITION/ROTATION */
int _NL1B_Straight1_Entrance_Window_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_NL1B_Straight1_Entrance_Window_setpos] component NL1B_Straight1_Entrance_Window=Arm() SETTING [Arm:0]");
  stracpy(_NL1B_Straight1_Entrance_Window_var._name, "NL1B_Straight1_Entrance_Window", 16384);
  stracpy(_NL1B_Straight1_Entrance_Window_var._type, "Arm", 16384);
  _NL1B_Straight1_Entrance_Window_var._index=5;
  int current_setpos_index = 5;
  /* component NL1B_Straight1_Entrance_Window=Arm() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (-2.15)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _Origin_var._rotation_absolute, _NL1B_Straight1_Entrance_Window_var._rotation_absolute);
    rot_transpose(_NL1A_NL1B_NL2_NL3_InPile_var._rotation_absolute, tr1);
    rot_mul(_NL1B_Straight1_Entrance_Window_var._rotation_absolute, tr1, _NL1B_Straight1_Entrance_Window_var._rotation_relative);
    _NL1B_Straight1_Entrance_Window_var._rotation_is_identity =  rot_test_identity(_NL1B_Straight1_Entrance_Window_var._rotation_relative);
    tc1 = coords_set(
      -0.1277, 0, 3.405);
    rot_transpose(_Origin_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _NL1B_Straight1_Entrance_Window_var._position_absolute = coords_add(_Origin_var._position_absolute, tc2);
    tc1 = coords_sub(_NL1A_NL1B_NL2_NL3_InPile_var._position_absolute, _NL1B_Straight1_Entrance_Window_var._position_absolute);
    _NL1B_Straight1_Entrance_Window_var._position_relative = rot_apply(_NL1B_Straight1_Entrance_Window_var._rotation_absolute, tc1);
  } /* NL1B_Straight1_Entrance_Window=Arm() AT ROTATED */
  DEBUG_COMPONENT("NL1B_Straight1_Entrance_Window", _NL1B_Straight1_Entrance_Window_var._position_absolute, _NL1B_Straight1_Entrance_Window_var._rotation_absolute);
  instrument->_position_absolute[5] = _NL1B_Straight1_Entrance_Window_var._position_absolute;
  instrument->_position_relative[5] = _NL1B_Straight1_Entrance_Window_var._position_relative;
    _NL1B_Straight1_Entrance_Window_var._position_relative_is_zero =  coords_test_zero(_NL1B_Straight1_Entrance_Window_var._position_relative);
  instrument->counter_N[5]  = instrument->counter_P[5] = instrument->counter_P2[5] = 0;
  instrument->counter_AbsorbProp[5]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0004_NL1B_Straight1_Entrance_Window", _NL1B_Straight1_Entrance_Window_var._position_absolute, _NL1B_Straight1_Entrance_Window_var._rotation_absolute, "Arm");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _NL1B_Straight1_Entrance_Window_setpos */

/* component NL1B_Straight1=Guide() SETTING, POSITION/ROTATION */
int _NL1B_Straight1_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_NL1B_Straight1_setpos] component NL1B_Straight1=Guide() SETTING [Guide:0]");
  stracpy(_NL1B_Straight1_var._name, "NL1B_Straight1", 16384);
  stracpy(_NL1B_Straight1_var._type, "Guide", 16384);
  _NL1B_Straight1_var._index=6;
  int current_setpos_index = 6;
  _NL1B_Straight1_var._parameters.reflect[0]='\0';
  _NL1B_Straight1_var._parameters.w1 = 0.06;
  _NL1B_Straight1_var._parameters.h1 = 0.125;
  _NL1B_Straight1_var._parameters.w2 = 0.06;
  _NL1B_Straight1_var._parameters.h2 = 0.125;
  _NL1B_Straight1_var._parameters.l = 1.549;
  _NL1B_Straight1_var._parameters.R0 = R0_para;
  _NL1B_Straight1_var._parameters.Qc = Qc_para;
  _NL1B_Straight1_var._parameters.alpha = alpha_para;
  _NL1B_Straight1_var._parameters.m = MGUIDE;
  _NL1B_Straight1_var._parameters.W = W_para;


  /* component NL1B_Straight1=Guide() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (0)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _NL1B_Straight1_Entrance_Window_var._rotation_absolute, _NL1B_Straight1_var._rotation_absolute);
    rot_transpose(_NL1A_NL1B_NL2_NL3_InPile_var._rotation_absolute, tr1);
    rot_mul(_NL1B_Straight1_var._rotation_absolute, tr1, _NL1B_Straight1_var._rotation_relative);
    _NL1B_Straight1_var._rotation_is_identity =  rot_test_identity(_NL1B_Straight1_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0);
    rot_transpose(_NL1B_Straight1_Entrance_Window_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _NL1B_Straight1_var._position_absolute = coords_add(_NL1B_Straight1_Entrance_Window_var._position_absolute, tc2);
    tc1 = coords_sub(_NL1A_NL1B_NL2_NL3_InPile_var._position_absolute, _NL1B_Straight1_var._position_absolute);
    _NL1B_Straight1_var._position_relative = rot_apply(_NL1B_Straight1_var._rotation_absolute, tc1);
  } /* NL1B_Straight1=Guide() AT ROTATED */
  DEBUG_COMPONENT("NL1B_Straight1", _NL1B_Straight1_var._position_absolute, _NL1B_Straight1_var._rotation_absolute);
  instrument->_position_absolute[6] = _NL1B_Straight1_var._position_absolute;
  instrument->_position_relative[6] = _NL1B_Straight1_var._position_relative;
    _NL1B_Straight1_var._position_relative_is_zero =  coords_test_zero(_NL1B_Straight1_var._position_relative);
  instrument->counter_N[6]  = instrument->counter_P[6] = instrument->counter_P2[6] = 0;
  instrument->counter_AbsorbProp[6]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0005_NL1B_Straight1", _NL1B_Straight1_var._position_absolute, _NL1B_Straight1_var._rotation_absolute, "Guide");
        mccomp_param_nexus(nxhandle,"0005_NL1B_Straight1", "reflect", 0, 0, "char*");
        mccomp_param_nexus(nxhandle,"0005_NL1B_Straight1", "w1", "NONE", "0.06","MCNUM");
        mccomp_param_nexus(nxhandle,"0005_NL1B_Straight1", "h1", "NONE", "0.125","MCNUM");
        mccomp_param_nexus(nxhandle,"0005_NL1B_Straight1", "w2", "0", "0.06","MCNUM");
        mccomp_param_nexus(nxhandle,"0005_NL1B_Straight1", "h2", "0", "0.125","MCNUM");
        mccomp_param_nexus(nxhandle,"0005_NL1B_Straight1", "l", "NONE", "1.549","MCNUM");
        mccomp_param_nexus(nxhandle,"0005_NL1B_Straight1", "R0", "0.99", "R0_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0005_NL1B_Straight1", "Qc", "0.0219", "Qc_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0005_NL1B_Straight1", "alpha", "6.07", "alpha_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0005_NL1B_Straight1", "m", "2", "MGUIDE","MCNUM");
        mccomp_param_nexus(nxhandle,"0005_NL1B_Straight1", "W", "0.003", "W_para","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _NL1B_Straight1_setpos */

/* component NL1B_Curved_Entrance_Window=Arm() SETTING, POSITION/ROTATION */
int _NL1B_Curved_Entrance_Window_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_NL1B_Curved_Entrance_Window_setpos] component NL1B_Curved_Entrance_Window=Arm() SETTING [Arm:0]");
  stracpy(_NL1B_Curved_Entrance_Window_var._name, "NL1B_Curved_Entrance_Window", 16384);
  stracpy(_NL1B_Curved_Entrance_Window_var._type, "Arm", 16384);
  _NL1B_Curved_Entrance_Window_var._index=7;
  int current_setpos_index = 7;
  /* component NL1B_Curved_Entrance_Window=Arm() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (0)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _NL1B_Straight1_Entrance_Window_var._rotation_absolute, _NL1B_Curved_Entrance_Window_var._rotation_absolute);
    rot_transpose(_NL1B_Straight1_var._rotation_absolute, tr1);
    rot_mul(_NL1B_Curved_Entrance_Window_var._rotation_absolute, tr1, _NL1B_Curved_Entrance_Window_var._rotation_relative);
    _NL1B_Curved_Entrance_Window_var._rotation_is_identity =  rot_test_identity(_NL1B_Curved_Entrance_Window_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1.549);
    rot_transpose(_NL1B_Straight1_Entrance_Window_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _NL1B_Curved_Entrance_Window_var._position_absolute = coords_add(_NL1B_Straight1_Entrance_Window_var._position_absolute, tc2);
    tc1 = coords_sub(_NL1B_Straight1_var._position_absolute, _NL1B_Curved_Entrance_Window_var._position_absolute);
    _NL1B_Curved_Entrance_Window_var._position_relative = rot_apply(_NL1B_Curved_Entrance_Window_var._rotation_absolute, tc1);
  } /* NL1B_Curved_Entrance_Window=Arm() AT ROTATED */
  DEBUG_COMPONENT("NL1B_Curved_Entrance_Window", _NL1B_Curved_Entrance_Window_var._position_absolute, _NL1B_Curved_Entrance_Window_var._rotation_absolute);
  instrument->_position_absolute[7] = _NL1B_Curved_Entrance_Window_var._position_absolute;
  instrument->_position_relative[7] = _NL1B_Curved_Entrance_Window_var._position_relative;
    _NL1B_Curved_Entrance_Window_var._position_relative_is_zero =  coords_test_zero(_NL1B_Curved_Entrance_Window_var._position_relative);
  instrument->counter_N[7]  = instrument->counter_P[7] = instrument->counter_P2[7] = 0;
  instrument->counter_AbsorbProp[7]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0006_NL1B_Curved_Entrance_Window", _NL1B_Curved_Entrance_Window_var._position_absolute, _NL1B_Curved_Entrance_Window_var._rotation_absolute, "Arm");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _NL1B_Curved_Entrance_Window_setpos */

/* component NL1B_Curved_Guide_1=Guide_curved() SETTING, POSITION/ROTATION */
int _NL1B_Curved_Guide_1_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_NL1B_Curved_Guide_1_setpos] component NL1B_Curved_Guide_1=Guide_curved() SETTING [Guide_curved:0]");
  stracpy(_NL1B_Curved_Guide_1_var._name, "NL1B_Curved_Guide_1", 16384);
  stracpy(_NL1B_Curved_Guide_1_var._type, "Guide_curved", 16384);
  _NL1B_Curved_Guide_1_var._index=8;
  int current_setpos_index = 8;
  _NL1B_Curved_Guide_1_var._parameters.w1 = 0.06;
  _NL1B_Curved_Guide_1_var._parameters.h1 = 0.125;
  _NL1B_Curved_Guide_1_var._parameters.l = LGUIDE_1;
  _NL1B_Curved_Guide_1_var._parameters.R0 = R0_para;
  _NL1B_Curved_Guide_1_var._parameters.Qc = Qc_para;
  _NL1B_Curved_Guide_1_var._parameters.alpha = alpha_para;
  _NL1B_Curved_Guide_1_var._parameters.m = MGUIDE;
  _NL1B_Curved_Guide_1_var._parameters.W = W_para;
  _NL1B_Curved_Guide_1_var._parameters.curvature = 2800;

  /* component NL1B_Curved_Guide_1=Guide_curved() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (0)*DEG2RAD, (180)*DEG2RAD);
    rot_mul(tr1, _NL1B_Curved_Entrance_Window_var._rotation_absolute, _NL1B_Curved_Guide_1_var._rotation_absolute);
    rot_transpose(_NL1B_Straight1_var._rotation_absolute, tr1);
    rot_mul(_NL1B_Curved_Guide_1_var._rotation_absolute, tr1, _NL1B_Curved_Guide_1_var._rotation_relative);
    _NL1B_Curved_Guide_1_var._rotation_is_identity =  rot_test_identity(_NL1B_Curved_Guide_1_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0);
    rot_transpose(_NL1B_Curved_Entrance_Window_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _NL1B_Curved_Guide_1_var._position_absolute = coords_add(_NL1B_Curved_Entrance_Window_var._position_absolute, tc2);
    tc1 = coords_sub(_NL1B_Straight1_var._position_absolute, _NL1B_Curved_Guide_1_var._position_absolute);
    _NL1B_Curved_Guide_1_var._position_relative = rot_apply(_NL1B_Curved_Guide_1_var._rotation_absolute, tc1);
  } /* NL1B_Curved_Guide_1=Guide_curved() AT ROTATED */
  DEBUG_COMPONENT("NL1B_Curved_Guide_1", _NL1B_Curved_Guide_1_var._position_absolute, _NL1B_Curved_Guide_1_var._rotation_absolute);
  instrument->_position_absolute[8] = _NL1B_Curved_Guide_1_var._position_absolute;
  instrument->_position_relative[8] = _NL1B_Curved_Guide_1_var._position_relative;
    _NL1B_Curved_Guide_1_var._position_relative_is_zero =  coords_test_zero(_NL1B_Curved_Guide_1_var._position_relative);
  instrument->counter_N[8]  = instrument->counter_P[8] = instrument->counter_P2[8] = 0;
  instrument->counter_AbsorbProp[8]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0007_NL1B_Curved_Guide_1", _NL1B_Curved_Guide_1_var._position_absolute, _NL1B_Curved_Guide_1_var._rotation_absolute, "Guide_curved");
        mccomp_param_nexus(nxhandle,"0007_NL1B_Curved_Guide_1", "w1", "NONE", "0.06","MCNUM");
        mccomp_param_nexus(nxhandle,"0007_NL1B_Curved_Guide_1", "h1", "NONE", "0.125","MCNUM");
        mccomp_param_nexus(nxhandle,"0007_NL1B_Curved_Guide_1", "l", "NONE", "LGUIDE_1","MCNUM");
        mccomp_param_nexus(nxhandle,"0007_NL1B_Curved_Guide_1", "R0", "0.995", "R0_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0007_NL1B_Curved_Guide_1", "Qc", "0.0218", "Qc_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0007_NL1B_Curved_Guide_1", "alpha", "4.38", "alpha_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0007_NL1B_Curved_Guide_1", "m", "2", "MGUIDE","MCNUM");
        mccomp_param_nexus(nxhandle,"0007_NL1B_Curved_Guide_1", "W", "0.003", "W_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0007_NL1B_Curved_Guide_1", "curvature", "2700", "2800","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _NL1B_Curved_Guide_1_setpos */

/* component NL1B_Velocity_Selector_Gap_Entrance_Window=Arm() SETTING, POSITION/ROTATION */
int _NL1B_Velocity_Selector_Gap_Entrance_Window_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_NL1B_Velocity_Selector_Gap_Entrance_Window_setpos] component NL1B_Velocity_Selector_Gap_Entrance_Window=Arm() SETTING [Arm:0]");
  stracpy(_NL1B_Velocity_Selector_Gap_Entrance_Window_var._name, "NL1B_Velocity_Selector_Gap_Entrance_Window", 16384);
  stracpy(_NL1B_Velocity_Selector_Gap_Entrance_Window_var._type, "Arm", 16384);
  _NL1B_Velocity_Selector_Gap_Entrance_Window_var._index=9;
  int current_setpos_index = 9;
  /* component NL1B_Velocity_Selector_Gap_Entrance_Window=Arm() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (- beta_guide_1)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _NL1B_Curved_Entrance_Window_var._rotation_absolute, _NL1B_Velocity_Selector_Gap_Entrance_Window_var._rotation_absolute);
    rot_transpose(_NL1B_Curved_Guide_1_var._rotation_absolute, tr1);
    rot_mul(_NL1B_Velocity_Selector_Gap_Entrance_Window_var._rotation_absolute, tr1, _NL1B_Velocity_Selector_Gap_Entrance_Window_var._rotation_relative);
    _NL1B_Velocity_Selector_Gap_Entrance_Window_var._rotation_is_identity =  rot_test_identity(_NL1B_Velocity_Selector_Gap_Entrance_Window_var._rotation_relative);
    tc1 = coords_set(
      - s_guide_1, 0, LGUIDE_1);
    rot_transpose(_NL1B_Curved_Entrance_Window_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _NL1B_Velocity_Selector_Gap_Entrance_Window_var._position_absolute = coords_add(_NL1B_Curved_Entrance_Window_var._position_absolute, tc2);
    tc1 = coords_sub(_NL1B_Curved_Guide_1_var._position_absolute, _NL1B_Velocity_Selector_Gap_Entrance_Window_var._position_absolute);
    _NL1B_Velocity_Selector_Gap_Entrance_Window_var._position_relative = rot_apply(_NL1B_Velocity_Selector_Gap_Entrance_Window_var._rotation_absolute, tc1);
  } /* NL1B_Velocity_Selector_Gap_Entrance_Window=Arm() AT ROTATED */
  DEBUG_COMPONENT("NL1B_Velocity_Selector_Gap_Entrance_Window", _NL1B_Velocity_Selector_Gap_Entrance_Window_var._position_absolute, _NL1B_Velocity_Selector_Gap_Entrance_Window_var._rotation_absolute);
  instrument->_position_absolute[9] = _NL1B_Velocity_Selector_Gap_Entrance_Window_var._position_absolute;
  instrument->_position_relative[9] = _NL1B_Velocity_Selector_Gap_Entrance_Window_var._position_relative;
    _NL1B_Velocity_Selector_Gap_Entrance_Window_var._position_relative_is_zero =  coords_test_zero(_NL1B_Velocity_Selector_Gap_Entrance_Window_var._position_relative);
  instrument->counter_N[9]  = instrument->counter_P[9] = instrument->counter_P2[9] = 0;
  instrument->counter_AbsorbProp[9]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0008_NL1B_Velocity_Selector_Gap_Entrance_Window", _NL1B_Velocity_Selector_Gap_Entrance_Window_var._position_absolute, _NL1B_Velocity_Selector_Gap_Entrance_Window_var._rotation_absolute, "Arm");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _NL1B_Velocity_Selector_Gap_Entrance_Window_setpos */

/* component Before_selec=PSD_monitor() SETTING, POSITION/ROTATION */
int _Before_selec_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Before_selec_setpos] component Before_selec=PSD_monitor() SETTING [PSD_monitor:0]");
  stracpy(_Before_selec_var._name, "Before_selec", 16384);
  stracpy(_Before_selec_var._type, "PSD_monitor", 16384);
  _Before_selec_var._index=10;
  int current_setpos_index = 10;
  _Before_selec_var._parameters.nx = 50;
  _Before_selec_var._parameters.ny = 50;
  if("PSD_monitor_before.dat" && strlen("PSD_monitor_before.dat"))
    stracpy(_Before_selec_var._parameters.filename, "PSD_monitor_before.dat" ? "PSD_monitor_before.dat" : "", 16384);
  else 
  _Before_selec_var._parameters.filename[0]='\0';
  _Before_selec_var._parameters.xmin = -0.05;
  _Before_selec_var._parameters.xmax = 0.05;
  _Before_selec_var._parameters.ymin = -0.05;
  _Before_selec_var._parameters.ymax = 0.05;
  _Before_selec_var._parameters.xwidth = 0.20;
  _Before_selec_var._parameters.yheight = 0.20;
  _Before_selec_var._parameters.restore_neutron = 1;
  _Before_selec_var._parameters.nowritefile = 0;


  /* component Before_selec=PSD_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _NL1B_Velocity_Selector_Gap_Entrance_Window_var._rotation_absolute, _Before_selec_var._rotation_absolute);
    rot_transpose(_NL1B_Curved_Guide_1_var._rotation_absolute, tr1);
    rot_mul(_Before_selec_var._rotation_absolute, tr1, _Before_selec_var._rotation_relative);
    _Before_selec_var._rotation_is_identity =  rot_test_identity(_Before_selec_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0.19);
    rot_transpose(_NL1B_Velocity_Selector_Gap_Entrance_Window_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Before_selec_var._position_absolute = coords_add(_NL1B_Velocity_Selector_Gap_Entrance_Window_var._position_absolute, tc2);
    tc1 = coords_sub(_NL1B_Curved_Guide_1_var._position_absolute, _Before_selec_var._position_absolute);
    _Before_selec_var._position_relative = rot_apply(_Before_selec_var._rotation_absolute, tc1);
  } /* Before_selec=PSD_monitor() AT ROTATED */
  DEBUG_COMPONENT("Before_selec", _Before_selec_var._position_absolute, _Before_selec_var._rotation_absolute);
  instrument->_position_absolute[10] = _Before_selec_var._position_absolute;
  instrument->_position_relative[10] = _Before_selec_var._position_relative;
    _Before_selec_var._position_relative_is_zero =  coords_test_zero(_Before_selec_var._position_relative);
  instrument->counter_N[10]  = instrument->counter_P[10] = instrument->counter_P2[10] = 0;
  instrument->counter_AbsorbProp[10]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0009_Before_selec", _Before_selec_var._position_absolute, _Before_selec_var._rotation_absolute, "PSD_monitor");
        mccomp_param_nexus(nxhandle,"0009_Before_selec", "nx", "90", "50","int");
        mccomp_param_nexus(nxhandle,"0009_Before_selec", "ny", "90", "50","int");
        mccomp_param_nexus(nxhandle,"0009_Before_selec", "filename", 0, "PSD_monitor_before.dat", "char*");
        mccomp_param_nexus(nxhandle,"0009_Before_selec", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0009_Before_selec", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0009_Before_selec", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0009_Before_selec", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0009_Before_selec", "xwidth", "0", "0.20","MCNUM");
        mccomp_param_nexus(nxhandle,"0009_Before_selec", "yheight", "0", "0.20","MCNUM");
        mccomp_param_nexus(nxhandle,"0009_Before_selec", "restore_neutron", "0", "1","int");
        mccomp_param_nexus(nxhandle,"0009_Before_selec", "nowritefile", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Before_selec_setpos */

/* component SELEC=Selector() SETTING, POSITION/ROTATION */
int _SELEC_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_SELEC_setpos] component SELEC=Selector() SETTING [Selector:0]");
  stracpy(_SELEC_var._name, "SELEC", 16384);
  stracpy(_SELEC_var._type, "Selector", 16384);
  _SELEC_var._index=11;
  int current_setpos_index = 11;
  _SELEC_var._parameters.xmin = -0.0625;
  _SELEC_var._parameters.xmax = 0.0625;
  _SELEC_var._parameters.ymin = -0.03;
  _SELEC_var._parameters.ymax = 0.03;
  _SELEC_var._parameters.length = 0.25;
  _SELEC_var._parameters.xwidth = 0;
  _SELEC_var._parameters.yheight = 0;
  _SELEC_var._parameters.nslit = 72;
  _SELEC_var._parameters.d = 0.0004;
  _SELEC_var._parameters.radius = 0.123;
  _SELEC_var._parameters.alpha = 19.7;
  _SELEC_var._parameters.nu = SelFreq;

  /* component SELEC=Selector() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (_instrument_var._parameters.tilt)*DEG2RAD, (90)*DEG2RAD);
    rot_mul(tr1, _NL1B_Velocity_Selector_Gap_Entrance_Window_var._rotation_absolute, _SELEC_var._rotation_absolute);
    rot_transpose(_Before_selec_var._rotation_absolute, tr1);
    rot_mul(_SELEC_var._rotation_absolute, tr1, _SELEC_var._rotation_relative);
    _SELEC_var._rotation_is_identity =  rot_test_identity(_SELEC_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0.2);
    rot_transpose(_NL1B_Velocity_Selector_Gap_Entrance_Window_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _SELEC_var._position_absolute = coords_add(_NL1B_Velocity_Selector_Gap_Entrance_Window_var._position_absolute, tc2);
    tc1 = coords_sub(_Before_selec_var._position_absolute, _SELEC_var._position_absolute);
    _SELEC_var._position_relative = rot_apply(_SELEC_var._rotation_absolute, tc1);
  } /* SELEC=Selector() AT ROTATED */
  DEBUG_COMPONENT("SELEC", _SELEC_var._position_absolute, _SELEC_var._rotation_absolute);
  instrument->_position_absolute[11] = _SELEC_var._position_absolute;
  instrument->_position_relative[11] = _SELEC_var._position_relative;
    _SELEC_var._position_relative_is_zero =  coords_test_zero(_SELEC_var._position_relative);
  instrument->counter_N[11]  = instrument->counter_P[11] = instrument->counter_P2[11] = 0;
  instrument->counter_AbsorbProp[11]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0010_SELEC", _SELEC_var._position_absolute, _SELEC_var._rotation_absolute, "Selector");
        mccomp_param_nexus(nxhandle,"0010_SELEC", "xmin", "-0.015", "-0.0625","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_SELEC", "xmax", "0.015", "0.0625","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_SELEC", "ymin", "-0.025", "-0.03","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_SELEC", "ymax", "0.025", "0.03","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_SELEC", "length", "0.25", "0.25","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_SELEC", "xwidth", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_SELEC", "yheight", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_SELEC", "nslit", "72", "72","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_SELEC", "d", "0.0004", "0.0004","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_SELEC", "radius", "0.12", "0.123","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_SELEC", "alpha", "48.298", "19.7","MCNUM");
        mccomp_param_nexus(nxhandle,"0010_SELEC", "nu", "500", "SelFreq","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _SELEC_setpos */

/* component After_selec=PSD_monitor() SETTING, POSITION/ROTATION */
int _After_selec_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_After_selec_setpos] component After_selec=PSD_monitor() SETTING [PSD_monitor:0]");
  stracpy(_After_selec_var._name, "After_selec", 16384);
  stracpy(_After_selec_var._type, "PSD_monitor", 16384);
  _After_selec_var._index=12;
  int current_setpos_index = 12;
  _After_selec_var._parameters.nx = 50;
  _After_selec_var._parameters.ny = 50;
  if("PSD_monitor_after.dat" && strlen("PSD_monitor_after.dat"))
    stracpy(_After_selec_var._parameters.filename, "PSD_monitor_after.dat" ? "PSD_monitor_after.dat" : "", 16384);
  else 
  _After_selec_var._parameters.filename[0]='\0';
  _After_selec_var._parameters.xmin = -0.05;
  _After_selec_var._parameters.xmax = 0.05;
  _After_selec_var._parameters.ymin = -0.05;
  _After_selec_var._parameters.ymax = 0.05;
  _After_selec_var._parameters.xwidth = 0.20;
  _After_selec_var._parameters.yheight = 0.20;
  _After_selec_var._parameters.restore_neutron = 1;
  _After_selec_var._parameters.nowritefile = 0;


  /* component After_selec=PSD_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _NL1B_Velocity_Selector_Gap_Entrance_Window_var._rotation_absolute, _After_selec_var._rotation_absolute);
    rot_transpose(_SELEC_var._rotation_absolute, tr1);
    rot_mul(_After_selec_var._rotation_absolute, tr1, _After_selec_var._rotation_relative);
    _After_selec_var._rotation_is_identity =  rot_test_identity(_After_selec_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0.46);
    rot_transpose(_NL1B_Velocity_Selector_Gap_Entrance_Window_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _After_selec_var._position_absolute = coords_add(_NL1B_Velocity_Selector_Gap_Entrance_Window_var._position_absolute, tc2);
    tc1 = coords_sub(_SELEC_var._position_absolute, _After_selec_var._position_absolute);
    _After_selec_var._position_relative = rot_apply(_After_selec_var._rotation_absolute, tc1);
  } /* After_selec=PSD_monitor() AT ROTATED */
  DEBUG_COMPONENT("After_selec", _After_selec_var._position_absolute, _After_selec_var._rotation_absolute);
  instrument->_position_absolute[12] = _After_selec_var._position_absolute;
  instrument->_position_relative[12] = _After_selec_var._position_relative;
    _After_selec_var._position_relative_is_zero =  coords_test_zero(_After_selec_var._position_relative);
  instrument->counter_N[12]  = instrument->counter_P[12] = instrument->counter_P2[12] = 0;
  instrument->counter_AbsorbProp[12]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0011_After_selec", _After_selec_var._position_absolute, _After_selec_var._rotation_absolute, "PSD_monitor");
        mccomp_param_nexus(nxhandle,"0011_After_selec", "nx", "90", "50","int");
        mccomp_param_nexus(nxhandle,"0011_After_selec", "ny", "90", "50","int");
        mccomp_param_nexus(nxhandle,"0011_After_selec", "filename", 0, "PSD_monitor_after.dat", "char*");
        mccomp_param_nexus(nxhandle,"0011_After_selec", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0011_After_selec", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0011_After_selec", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0011_After_selec", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0011_After_selec", "xwidth", "0", "0.20","MCNUM");
        mccomp_param_nexus(nxhandle,"0011_After_selec", "yheight", "0", "0.20","MCNUM");
        mccomp_param_nexus(nxhandle,"0011_After_selec", "restore_neutron", "0", "1","int");
        mccomp_param_nexus(nxhandle,"0011_After_selec", "nowritefile", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _After_selec_setpos */

/* component NL1B_Curved_2_Entrance_Window=Arm() SETTING, POSITION/ROTATION */
int _NL1B_Curved_2_Entrance_Window_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_NL1B_Curved_2_Entrance_Window_setpos] component NL1B_Curved_2_Entrance_Window=Arm() SETTING [Arm:0]");
  stracpy(_NL1B_Curved_2_Entrance_Window_var._name, "NL1B_Curved_2_Entrance_Window", 16384);
  stracpy(_NL1B_Curved_2_Entrance_Window_var._type, "Arm", 16384);
  _NL1B_Curved_2_Entrance_Window_var._index=13;
  int current_setpos_index = 13;
  /* component NL1B_Curved_2_Entrance_Window=Arm() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (0)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _NL1B_Velocity_Selector_Gap_Entrance_Window_var._rotation_absolute, _NL1B_Curved_2_Entrance_Window_var._rotation_absolute);
    rot_transpose(_After_selec_var._rotation_absolute, tr1);
    rot_mul(_NL1B_Curved_2_Entrance_Window_var._rotation_absolute, tr1, _NL1B_Curved_2_Entrance_Window_var._rotation_relative);
    _NL1B_Curved_2_Entrance_Window_var._rotation_is_identity =  rot_test_identity(_NL1B_Curved_2_Entrance_Window_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0.47);
    rot_transpose(_NL1B_Velocity_Selector_Gap_Entrance_Window_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _NL1B_Curved_2_Entrance_Window_var._position_absolute = coords_add(_NL1B_Velocity_Selector_Gap_Entrance_Window_var._position_absolute, tc2);
    tc1 = coords_sub(_After_selec_var._position_absolute, _NL1B_Curved_2_Entrance_Window_var._position_absolute);
    _NL1B_Curved_2_Entrance_Window_var._position_relative = rot_apply(_NL1B_Curved_2_Entrance_Window_var._rotation_absolute, tc1);
  } /* NL1B_Curved_2_Entrance_Window=Arm() AT ROTATED */
  DEBUG_COMPONENT("NL1B_Curved_2_Entrance_Window", _NL1B_Curved_2_Entrance_Window_var._position_absolute, _NL1B_Curved_2_Entrance_Window_var._rotation_absolute);
  instrument->_position_absolute[13] = _NL1B_Curved_2_Entrance_Window_var._position_absolute;
  instrument->_position_relative[13] = _NL1B_Curved_2_Entrance_Window_var._position_relative;
    _NL1B_Curved_2_Entrance_Window_var._position_relative_is_zero =  coords_test_zero(_NL1B_Curved_2_Entrance_Window_var._position_relative);
  instrument->counter_N[13]  = instrument->counter_P[13] = instrument->counter_P2[13] = 0;
  instrument->counter_AbsorbProp[13]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0012_NL1B_Curved_2_Entrance_Window", _NL1B_Curved_2_Entrance_Window_var._position_absolute, _NL1B_Curved_2_Entrance_Window_var._rotation_absolute, "Arm");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _NL1B_Curved_2_Entrance_Window_setpos */

/* component NL1B_Curved_Guide_2=Guide_curved() SETTING, POSITION/ROTATION */
int _NL1B_Curved_Guide_2_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_NL1B_Curved_Guide_2_setpos] component NL1B_Curved_Guide_2=Guide_curved() SETTING [Guide_curved:0]");
  stracpy(_NL1B_Curved_Guide_2_var._name, "NL1B_Curved_Guide_2", 16384);
  stracpy(_NL1B_Curved_Guide_2_var._type, "Guide_curved", 16384);
  _NL1B_Curved_Guide_2_var._index=14;
  int current_setpos_index = 14;
  _NL1B_Curved_Guide_2_var._parameters.w1 = 0.06;
  _NL1B_Curved_Guide_2_var._parameters.h1 = 0.125;
  _NL1B_Curved_Guide_2_var._parameters.l = LGUIDE_2;
  _NL1B_Curved_Guide_2_var._parameters.R0 = R0_para;
  _NL1B_Curved_Guide_2_var._parameters.Qc = Qc_para;
  _NL1B_Curved_Guide_2_var._parameters.alpha = alpha_para;
  _NL1B_Curved_Guide_2_var._parameters.m = MGUIDE;
  _NL1B_Curved_Guide_2_var._parameters.W = W_para;
  _NL1B_Curved_Guide_2_var._parameters.curvature = 2800;

  /* component NL1B_Curved_Guide_2=Guide_curved() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (0)*DEG2RAD, (180)*DEG2RAD);
    rot_mul(tr1, _NL1B_Curved_2_Entrance_Window_var._rotation_absolute, _NL1B_Curved_Guide_2_var._rotation_absolute);
    rot_transpose(_After_selec_var._rotation_absolute, tr1);
    rot_mul(_NL1B_Curved_Guide_2_var._rotation_absolute, tr1, _NL1B_Curved_Guide_2_var._rotation_relative);
    _NL1B_Curved_Guide_2_var._rotation_is_identity =  rot_test_identity(_NL1B_Curved_Guide_2_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0);
    rot_transpose(_NL1B_Curved_2_Entrance_Window_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _NL1B_Curved_Guide_2_var._position_absolute = coords_add(_NL1B_Curved_2_Entrance_Window_var._position_absolute, tc2);
    tc1 = coords_sub(_After_selec_var._position_absolute, _NL1B_Curved_Guide_2_var._position_absolute);
    _NL1B_Curved_Guide_2_var._position_relative = rot_apply(_NL1B_Curved_Guide_2_var._rotation_absolute, tc1);
  } /* NL1B_Curved_Guide_2=Guide_curved() AT ROTATED */
  DEBUG_COMPONENT("NL1B_Curved_Guide_2", _NL1B_Curved_Guide_2_var._position_absolute, _NL1B_Curved_Guide_2_var._rotation_absolute);
  instrument->_position_absolute[14] = _NL1B_Curved_Guide_2_var._position_absolute;
  instrument->_position_relative[14] = _NL1B_Curved_Guide_2_var._position_relative;
    _NL1B_Curved_Guide_2_var._position_relative_is_zero =  coords_test_zero(_NL1B_Curved_Guide_2_var._position_relative);
  instrument->counter_N[14]  = instrument->counter_P[14] = instrument->counter_P2[14] = 0;
  instrument->counter_AbsorbProp[14]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0013_NL1B_Curved_Guide_2", _NL1B_Curved_Guide_2_var._position_absolute, _NL1B_Curved_Guide_2_var._rotation_absolute, "Guide_curved");
        mccomp_param_nexus(nxhandle,"0013_NL1B_Curved_Guide_2", "w1", "NONE", "0.06","MCNUM");
        mccomp_param_nexus(nxhandle,"0013_NL1B_Curved_Guide_2", "h1", "NONE", "0.125","MCNUM");
        mccomp_param_nexus(nxhandle,"0013_NL1B_Curved_Guide_2", "l", "NONE", "LGUIDE_2","MCNUM");
        mccomp_param_nexus(nxhandle,"0013_NL1B_Curved_Guide_2", "R0", "0.995", "R0_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0013_NL1B_Curved_Guide_2", "Qc", "0.0218", "Qc_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0013_NL1B_Curved_Guide_2", "alpha", "4.38", "alpha_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0013_NL1B_Curved_Guide_2", "m", "2", "MGUIDE","MCNUM");
        mccomp_param_nexus(nxhandle,"0013_NL1B_Curved_Guide_2", "W", "0.003", "W_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0013_NL1B_Curved_Guide_2", "curvature", "2700", "2800","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _NL1B_Curved_Guide_2_setpos */

/* component NL1B_Straight2_Entrance_Window=Arm() SETTING, POSITION/ROTATION */
int _NL1B_Straight2_Entrance_Window_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_NL1B_Straight2_Entrance_Window_setpos] component NL1B_Straight2_Entrance_Window=Arm() SETTING [Arm:0]");
  stracpy(_NL1B_Straight2_Entrance_Window_var._name, "NL1B_Straight2_Entrance_Window", 16384);
  stracpy(_NL1B_Straight2_Entrance_Window_var._type, "Arm", 16384);
  _NL1B_Straight2_Entrance_Window_var._index=15;
  int current_setpos_index = 15;
  /* component NL1B_Straight2_Entrance_Window=Arm() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (- beta_guide_2)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _NL1B_Curved_2_Entrance_Window_var._rotation_absolute, _NL1B_Straight2_Entrance_Window_var._rotation_absolute);
    rot_transpose(_NL1B_Curved_Guide_2_var._rotation_absolute, tr1);
    rot_mul(_NL1B_Straight2_Entrance_Window_var._rotation_absolute, tr1, _NL1B_Straight2_Entrance_Window_var._rotation_relative);
    _NL1B_Straight2_Entrance_Window_var._rotation_is_identity =  rot_test_identity(_NL1B_Straight2_Entrance_Window_var._rotation_relative);
    tc1 = coords_set(
      - s_guide_2, 0, LGUIDE_2);
    rot_transpose(_NL1B_Curved_2_Entrance_Window_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _NL1B_Straight2_Entrance_Window_var._position_absolute = coords_add(_NL1B_Curved_2_Entrance_Window_var._position_absolute, tc2);
    tc1 = coords_sub(_NL1B_Curved_Guide_2_var._position_absolute, _NL1B_Straight2_Entrance_Window_var._position_absolute);
    _NL1B_Straight2_Entrance_Window_var._position_relative = rot_apply(_NL1B_Straight2_Entrance_Window_var._rotation_absolute, tc1);
  } /* NL1B_Straight2_Entrance_Window=Arm() AT ROTATED */
  DEBUG_COMPONENT("NL1B_Straight2_Entrance_Window", _NL1B_Straight2_Entrance_Window_var._position_absolute, _NL1B_Straight2_Entrance_Window_var._rotation_absolute);
  instrument->_position_absolute[15] = _NL1B_Straight2_Entrance_Window_var._position_absolute;
  instrument->_position_relative[15] = _NL1B_Straight2_Entrance_Window_var._position_relative;
    _NL1B_Straight2_Entrance_Window_var._position_relative_is_zero =  coords_test_zero(_NL1B_Straight2_Entrance_Window_var._position_relative);
  instrument->counter_N[15]  = instrument->counter_P[15] = instrument->counter_P2[15] = 0;
  instrument->counter_AbsorbProp[15]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0014_NL1B_Straight2_Entrance_Window", _NL1B_Straight2_Entrance_Window_var._position_absolute, _NL1B_Straight2_Entrance_Window_var._rotation_absolute, "Arm");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _NL1B_Straight2_Entrance_Window_setpos */

/* component NL1B_Straight2=Guide() SETTING, POSITION/ROTATION */
int _NL1B_Straight2_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_NL1B_Straight2_setpos] component NL1B_Straight2=Guide() SETTING [Guide:0]");
  stracpy(_NL1B_Straight2_var._name, "NL1B_Straight2", 16384);
  stracpy(_NL1B_Straight2_var._type, "Guide", 16384);
  _NL1B_Straight2_var._index=16;
  int current_setpos_index = 16;
  _NL1B_Straight2_var._parameters.reflect[0]='\0';
  _NL1B_Straight2_var._parameters.w1 = 0.06;
  _NL1B_Straight2_var._parameters.h1 = 0.125;
  _NL1B_Straight2_var._parameters.w2 = 0.06;
  _NL1B_Straight2_var._parameters.h2 = 0.125;
  _NL1B_Straight2_var._parameters.l = 3.500;
  _NL1B_Straight2_var._parameters.R0 = R0_para;
  _NL1B_Straight2_var._parameters.Qc = Qc_para;
  _NL1B_Straight2_var._parameters.alpha = alpha_para;
  _NL1B_Straight2_var._parameters.m = MGUIDE;
  _NL1B_Straight2_var._parameters.W = W_para;


  /* component NL1B_Straight2=Guide() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (0)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _NL1B_Straight2_Entrance_Window_var._rotation_absolute, _NL1B_Straight2_var._rotation_absolute);
    rot_transpose(_NL1B_Curved_Guide_2_var._rotation_absolute, tr1);
    rot_mul(_NL1B_Straight2_var._rotation_absolute, tr1, _NL1B_Straight2_var._rotation_relative);
    _NL1B_Straight2_var._rotation_is_identity =  rot_test_identity(_NL1B_Straight2_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0);
    rot_transpose(_NL1B_Straight2_Entrance_Window_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _NL1B_Straight2_var._position_absolute = coords_add(_NL1B_Straight2_Entrance_Window_var._position_absolute, tc2);
    tc1 = coords_sub(_NL1B_Curved_Guide_2_var._position_absolute, _NL1B_Straight2_var._position_absolute);
    _NL1B_Straight2_var._position_relative = rot_apply(_NL1B_Straight2_var._rotation_absolute, tc1);
  } /* NL1B_Straight2=Guide() AT ROTATED */
  DEBUG_COMPONENT("NL1B_Straight2", _NL1B_Straight2_var._position_absolute, _NL1B_Straight2_var._rotation_absolute);
  instrument->_position_absolute[16] = _NL1B_Straight2_var._position_absolute;
  instrument->_position_relative[16] = _NL1B_Straight2_var._position_relative;
    _NL1B_Straight2_var._position_relative_is_zero =  coords_test_zero(_NL1B_Straight2_var._position_relative);
  instrument->counter_N[16]  = instrument->counter_P[16] = instrument->counter_P2[16] = 0;
  instrument->counter_AbsorbProp[16]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0015_NL1B_Straight2", _NL1B_Straight2_var._position_absolute, _NL1B_Straight2_var._rotation_absolute, "Guide");
        mccomp_param_nexus(nxhandle,"0015_NL1B_Straight2", "reflect", 0, 0, "char*");
        mccomp_param_nexus(nxhandle,"0015_NL1B_Straight2", "w1", "NONE", "0.06","MCNUM");
        mccomp_param_nexus(nxhandle,"0015_NL1B_Straight2", "h1", "NONE", "0.125","MCNUM");
        mccomp_param_nexus(nxhandle,"0015_NL1B_Straight2", "w2", "0", "0.06","MCNUM");
        mccomp_param_nexus(nxhandle,"0015_NL1B_Straight2", "h2", "0", "0.125","MCNUM");
        mccomp_param_nexus(nxhandle,"0015_NL1B_Straight2", "l", "NONE", "3.500","MCNUM");
        mccomp_param_nexus(nxhandle,"0015_NL1B_Straight2", "R0", "0.99", "R0_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0015_NL1B_Straight2", "Qc", "0.0219", "Qc_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0015_NL1B_Straight2", "alpha", "6.07", "alpha_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0015_NL1B_Straight2", "m", "2", "MGUIDE","MCNUM");
        mccomp_param_nexus(nxhandle,"0015_NL1B_Straight2", "W", "0.003", "W_para","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _NL1B_Straight2_setpos */

/* component NL1B_Elliptical_Entrance_Window=Arm() SETTING, POSITION/ROTATION */
int _NL1B_Elliptical_Entrance_Window_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_NL1B_Elliptical_Entrance_Window_setpos] component NL1B_Elliptical_Entrance_Window=Arm() SETTING [Arm:0]");
  stracpy(_NL1B_Elliptical_Entrance_Window_var._name, "NL1B_Elliptical_Entrance_Window", 16384);
  stracpy(_NL1B_Elliptical_Entrance_Window_var._type, "Arm", 16384);
  _NL1B_Elliptical_Entrance_Window_var._index=17;
  int current_setpos_index = 17;
  /* component NL1B_Elliptical_Entrance_Window=Arm() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (0.000)*DEG2RAD, (0.00)*DEG2RAD);
    rot_mul(tr1, _NL1B_Straight2_Entrance_Window_var._rotation_absolute, _NL1B_Elliptical_Entrance_Window_var._rotation_absolute);
    rot_transpose(_NL1B_Straight2_var._rotation_absolute, tr1);
    rot_mul(_NL1B_Elliptical_Entrance_Window_var._rotation_absolute, tr1, _NL1B_Elliptical_Entrance_Window_var._rotation_relative);
    _NL1B_Elliptical_Entrance_Window_var._rotation_is_identity =  rot_test_identity(_NL1B_Elliptical_Entrance_Window_var._rotation_relative);
    tc1 = coords_set(
      0.0000, 0, 3.500);
    rot_transpose(_NL1B_Straight2_Entrance_Window_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _NL1B_Elliptical_Entrance_Window_var._position_absolute = coords_add(_NL1B_Straight2_Entrance_Window_var._position_absolute, tc2);
    tc1 = coords_sub(_NL1B_Straight2_var._position_absolute, _NL1B_Elliptical_Entrance_Window_var._position_absolute);
    _NL1B_Elliptical_Entrance_Window_var._position_relative = rot_apply(_NL1B_Elliptical_Entrance_Window_var._rotation_absolute, tc1);
  } /* NL1B_Elliptical_Entrance_Window=Arm() AT ROTATED */
  DEBUG_COMPONENT("NL1B_Elliptical_Entrance_Window", _NL1B_Elliptical_Entrance_Window_var._position_absolute, _NL1B_Elliptical_Entrance_Window_var._rotation_absolute);
  instrument->_position_absolute[17] = _NL1B_Elliptical_Entrance_Window_var._position_absolute;
  instrument->_position_relative[17] = _NL1B_Elliptical_Entrance_Window_var._position_relative;
    _NL1B_Elliptical_Entrance_Window_var._position_relative_is_zero =  coords_test_zero(_NL1B_Elliptical_Entrance_Window_var._position_relative);
  instrument->counter_N[17]  = instrument->counter_P[17] = instrument->counter_P2[17] = 0;
  instrument->counter_AbsorbProp[17]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0016_NL1B_Elliptical_Entrance_Window", _NL1B_Elliptical_Entrance_Window_var._position_absolute, _NL1B_Elliptical_Entrance_Window_var._rotation_absolute, "Arm");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _NL1B_Elliptical_Entrance_Window_setpos */

/* component elliptical_piece=Guide_tapering() SETTING, POSITION/ROTATION */
int _elliptical_piece_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_elliptical_piece_setpos] component elliptical_piece=Guide_tapering() SETTING [Guide_tapering:0]");
  stracpy(_elliptical_piece_var._name, "elliptical_piece", 16384);
  stracpy(_elliptical_piece_var._type, "Guide_tapering", 16384);
  _elliptical_piece_var._index=18;
  int current_setpos_index = 18;
  if("elliptical" && strlen("elliptical"))
    stracpy(_elliptical_piece_var._parameters.option, "elliptical" ? "elliptical" : "", 16384);
  else 
  _elliptical_piece_var._parameters.option[0]='\0';
  _elliptical_piece_var._parameters.w1 = 0.06;
  _elliptical_piece_var._parameters.h1 = 0.125;
  _elliptical_piece_var._parameters.l = 2.5;
  _elliptical_piece_var._parameters.linw = 2.9;
  _elliptical_piece_var._parameters.loutw = 0.4;
  _elliptical_piece_var._parameters.linh = 0;
  _elliptical_piece_var._parameters.louth = 0;
  _elliptical_piece_var._parameters.R0 = 0.99;
  _elliptical_piece_var._parameters.Qcx = 0.0217;
  _elliptical_piece_var._parameters.Qcy = 0.0217;
  _elliptical_piece_var._parameters.alphax = 4.00;
  _elliptical_piece_var._parameters.alphay = alpha_para;
  _elliptical_piece_var._parameters.W = 0.002;
  _elliptical_piece_var._parameters.mx = 5.2;
  _elliptical_piece_var._parameters.my = MGUIDE;
  _elliptical_piece_var._parameters.segno = 800;
  _elliptical_piece_var._parameters.curvature = 0;
  _elliptical_piece_var._parameters.curvature_v = 0;


  /* component elliptical_piece=Guide_tapering() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (0)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _NL1B_Elliptical_Entrance_Window_var._rotation_absolute, _elliptical_piece_var._rotation_absolute);
    rot_transpose(_NL1B_Straight2_var._rotation_absolute, tr1);
    rot_mul(_elliptical_piece_var._rotation_absolute, tr1, _elliptical_piece_var._rotation_relative);
    _elliptical_piece_var._rotation_is_identity =  rot_test_identity(_elliptical_piece_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0);
    rot_transpose(_NL1B_Elliptical_Entrance_Window_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _elliptical_piece_var._position_absolute = coords_add(_NL1B_Elliptical_Entrance_Window_var._position_absolute, tc2);
    tc1 = coords_sub(_NL1B_Straight2_var._position_absolute, _elliptical_piece_var._position_absolute);
    _elliptical_piece_var._position_relative = rot_apply(_elliptical_piece_var._rotation_absolute, tc1);
  } /* elliptical_piece=Guide_tapering() AT ROTATED */
  DEBUG_COMPONENT("elliptical_piece", _elliptical_piece_var._position_absolute, _elliptical_piece_var._rotation_absolute);
  instrument->_position_absolute[18] = _elliptical_piece_var._position_absolute;
  instrument->_position_relative[18] = _elliptical_piece_var._position_relative;
    _elliptical_piece_var._position_relative_is_zero =  coords_test_zero(_elliptical_piece_var._position_relative);
  instrument->counter_N[18]  = instrument->counter_P[18] = instrument->counter_P2[18] = 0;
  instrument->counter_AbsorbProp[18]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0017_elliptical_piece", _elliptical_piece_var._position_absolute, _elliptical_piece_var._rotation_absolute, "Guide_tapering");
        mccomp_param_nexus(nxhandle,"0017_elliptical_piece", "option", 0, "elliptical", "char*");
        mccomp_param_nexus(nxhandle,"0017_elliptical_piece", "w1", "0", "0.06","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_elliptical_piece", "h1", "0", "0.125","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_elliptical_piece", "l", "NONE", "2.5","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_elliptical_piece", "linw", "0", "2.9","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_elliptical_piece", "loutw", "0", "0.4","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_elliptical_piece", "linh", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_elliptical_piece", "louth", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_elliptical_piece", "R0", "0.99", "0.99","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_elliptical_piece", "Qcx", "0.021", "0.0217","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_elliptical_piece", "Qcy", "0.021", "0.0217","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_elliptical_piece", "alphax", "6.07", "4.00","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_elliptical_piece", "alphay", "6.07", "alpha_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_elliptical_piece", "W", "0.003", "0.002","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_elliptical_piece", "mx", "1", "5.2","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_elliptical_piece", "my", "1", "MGUIDE","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_elliptical_piece", "segno", "800", "800","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_elliptical_piece", "curvature", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0017_elliptical_piece", "curvature_v", "0", "0","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _elliptical_piece_setpos */

/* component Virtual_source=Slit() SETTING, POSITION/ROTATION */
int _Virtual_source_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Virtual_source_setpos] component Virtual_source=Slit() SETTING [Slit:0]");
  stracpy(_Virtual_source_var._name, "Virtual_source", 16384);
  stracpy(_Virtual_source_var._type, "Slit", 16384);
  _Virtual_source_var._index=19;
  int current_setpos_index = 19;
  _Virtual_source_var._parameters.xmin = UNSET;
  _Virtual_source_var._parameters.xmax = UNSET;
  _Virtual_source_var._parameters.ymin = UNSET;
  _Virtual_source_var._parameters.ymax = UNSET;
  _Virtual_source_var._parameters.radius = UNSET;
  _Virtual_source_var._parameters.xwidth = _instrument_var._parameters.wVS;
  _Virtual_source_var._parameters.yheight = 0.2;


  /* component Virtual_source=Slit() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _NL1B_Elliptical_Entrance_Window_var._rotation_absolute, _Virtual_source_var._rotation_absolute);
    rot_transpose(_elliptical_piece_var._rotation_absolute, tr1);
    rot_mul(_Virtual_source_var._rotation_absolute, tr1, _Virtual_source_var._rotation_relative);
    _Virtual_source_var._rotation_is_identity =  rot_test_identity(_Virtual_source_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 2.900);
    rot_transpose(_NL1B_Elliptical_Entrance_Window_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Virtual_source_var._position_absolute = coords_add(_NL1B_Elliptical_Entrance_Window_var._position_absolute, tc2);
    tc1 = coords_sub(_elliptical_piece_var._position_absolute, _Virtual_source_var._position_absolute);
    _Virtual_source_var._position_relative = rot_apply(_Virtual_source_var._rotation_absolute, tc1);
  } /* Virtual_source=Slit() AT ROTATED */
  DEBUG_COMPONENT("Virtual_source", _Virtual_source_var._position_absolute, _Virtual_source_var._rotation_absolute);
  instrument->_position_absolute[19] = _Virtual_source_var._position_absolute;
  instrument->_position_relative[19] = _Virtual_source_var._position_relative;
    _Virtual_source_var._position_relative_is_zero =  coords_test_zero(_Virtual_source_var._position_relative);
  instrument->counter_N[19]  = instrument->counter_P[19] = instrument->counter_P2[19] = 0;
  instrument->counter_AbsorbProp[19]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0018_Virtual_source", _Virtual_source_var._position_absolute, _Virtual_source_var._rotation_absolute, "Slit");
        mccomp_param_nexus(nxhandle,"0018_Virtual_source", "xmin", "UNSET", "UNSET","MCNUM");
        mccomp_param_nexus(nxhandle,"0018_Virtual_source", "xmax", "UNSET", "UNSET","MCNUM");
        mccomp_param_nexus(nxhandle,"0018_Virtual_source", "ymin", "UNSET", "UNSET","MCNUM");
        mccomp_param_nexus(nxhandle,"0018_Virtual_source", "ymax", "UNSET", "UNSET","MCNUM");
        mccomp_param_nexus(nxhandle,"0018_Virtual_source", "radius", "UNSET", "UNSET","MCNUM");
        mccomp_param_nexus(nxhandle,"0018_Virtual_source", "xwidth", "UNSET", "_instrument_var._parameters.wVS","MCNUM");
        mccomp_param_nexus(nxhandle,"0018_Virtual_source", "yheight", "UNSET", "0.2","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Virtual_source_setpos */

/* component NL1B_Vertical_Guide_Entrance_Window=Arm() SETTING, POSITION/ROTATION */
int _NL1B_Vertical_Guide_Entrance_Window_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_NL1B_Vertical_Guide_Entrance_Window_setpos] component NL1B_Vertical_Guide_Entrance_Window=Arm() SETTING [Arm:0]");
  stracpy(_NL1B_Vertical_Guide_Entrance_Window_var._name, "NL1B_Vertical_Guide_Entrance_Window", 16384);
  stracpy(_NL1B_Vertical_Guide_Entrance_Window_var._type, "Arm", 16384);
  _NL1B_Vertical_Guide_Entrance_Window_var._index=20;
  int current_setpos_index = 20;
  /* component NL1B_Vertical_Guide_Entrance_Window=Arm() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (0.000)*DEG2RAD, (0.00)*DEG2RAD);
    rot_mul(tr1, _NL1B_Elliptical_Entrance_Window_var._rotation_absolute, _NL1B_Vertical_Guide_Entrance_Window_var._rotation_absolute);
    rot_transpose(_Virtual_source_var._rotation_absolute, tr1);
    rot_mul(_NL1B_Vertical_Guide_Entrance_Window_var._rotation_absolute, tr1, _NL1B_Vertical_Guide_Entrance_Window_var._rotation_relative);
    _NL1B_Vertical_Guide_Entrance_Window_var._rotation_is_identity =  rot_test_identity(_NL1B_Vertical_Guide_Entrance_Window_var._rotation_relative);
    tc1 = coords_set(
      0.0000, 0, 2.925);
    rot_transpose(_NL1B_Elliptical_Entrance_Window_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _NL1B_Vertical_Guide_Entrance_Window_var._position_absolute = coords_add(_NL1B_Elliptical_Entrance_Window_var._position_absolute, tc2);
    tc1 = coords_sub(_Virtual_source_var._position_absolute, _NL1B_Vertical_Guide_Entrance_Window_var._position_absolute);
    _NL1B_Vertical_Guide_Entrance_Window_var._position_relative = rot_apply(_NL1B_Vertical_Guide_Entrance_Window_var._rotation_absolute, tc1);
  } /* NL1B_Vertical_Guide_Entrance_Window=Arm() AT ROTATED */
  DEBUG_COMPONENT("NL1B_Vertical_Guide_Entrance_Window", _NL1B_Vertical_Guide_Entrance_Window_var._position_absolute, _NL1B_Vertical_Guide_Entrance_Window_var._rotation_absolute);
  instrument->_position_absolute[20] = _NL1B_Vertical_Guide_Entrance_Window_var._position_absolute;
  instrument->_position_relative[20] = _NL1B_Vertical_Guide_Entrance_Window_var._position_relative;
    _NL1B_Vertical_Guide_Entrance_Window_var._position_relative_is_zero =  coords_test_zero(_NL1B_Vertical_Guide_Entrance_Window_var._position_relative);
  instrument->counter_N[20]  = instrument->counter_P[20] = instrument->counter_P2[20] = 0;
  instrument->counter_AbsorbProp[20]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0019_NL1B_Vertical_Guide_Entrance_Window", _NL1B_Vertical_Guide_Entrance_Window_var._position_absolute, _NL1B_Vertical_Guide_Entrance_Window_var._rotation_absolute, "Arm");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _NL1B_Vertical_Guide_Entrance_Window_setpos */

/* component NL1B_Vertical_Guide=Guide_channeled() SETTING, POSITION/ROTATION */
int _NL1B_Vertical_Guide_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_NL1B_Vertical_Guide_setpos] component NL1B_Vertical_Guide=Guide_channeled() SETTING [Guide_channeled:0]");
  stracpy(_NL1B_Vertical_Guide_var._name, "NL1B_Vertical_Guide", 16384);
  stracpy(_NL1B_Vertical_Guide_var._type, "Guide_channeled", 16384);
  _NL1B_Vertical_Guide_var._index=21;
  int current_setpos_index = 21;
  _NL1B_Vertical_Guide_var._parameters.w1 = 0.03;
  _NL1B_Vertical_Guide_var._parameters.h1 = 0.125;
  _NL1B_Vertical_Guide_var._parameters.w2 = 0.15;
  _NL1B_Vertical_Guide_var._parameters.h2 = 0.125;
  _NL1B_Vertical_Guide_var._parameters.l = 1.475;
  _NL1B_Vertical_Guide_var._parameters.R0 = R0_para;
  _NL1B_Vertical_Guide_var._parameters.Qc = 0;
  _NL1B_Vertical_Guide_var._parameters.alpha = 0;
  _NL1B_Vertical_Guide_var._parameters.m = 0;
  _NL1B_Vertical_Guide_var._parameters.nslit = 1;
  _NL1B_Vertical_Guide_var._parameters.d = 0.0005;
  _NL1B_Vertical_Guide_var._parameters.Qcx = Qc_para;
  _NL1B_Vertical_Guide_var._parameters.Qcy = Qc_para;
  _NL1B_Vertical_Guide_var._parameters.alphax = alpha_para;
  _NL1B_Vertical_Guide_var._parameters.alphay = alpha_para;
  _NL1B_Vertical_Guide_var._parameters.W = W_para;
  _NL1B_Vertical_Guide_var._parameters.mx = 0;
  _NL1B_Vertical_Guide_var._parameters.my = MGUIDE;
  _NL1B_Vertical_Guide_var._parameters.nu = 0;
  _NL1B_Vertical_Guide_var._parameters.phase = 0;


  /* component NL1B_Vertical_Guide=Guide_channeled() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (0)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _NL1B_Vertical_Guide_Entrance_Window_var._rotation_absolute, _NL1B_Vertical_Guide_var._rotation_absolute);
    rot_transpose(_Virtual_source_var._rotation_absolute, tr1);
    rot_mul(_NL1B_Vertical_Guide_var._rotation_absolute, tr1, _NL1B_Vertical_Guide_var._rotation_relative);
    _NL1B_Vertical_Guide_var._rotation_is_identity =  rot_test_identity(_NL1B_Vertical_Guide_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0);
    rot_transpose(_NL1B_Vertical_Guide_Entrance_Window_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _NL1B_Vertical_Guide_var._position_absolute = coords_add(_NL1B_Vertical_Guide_Entrance_Window_var._position_absolute, tc2);
    tc1 = coords_sub(_Virtual_source_var._position_absolute, _NL1B_Vertical_Guide_var._position_absolute);
    _NL1B_Vertical_Guide_var._position_relative = rot_apply(_NL1B_Vertical_Guide_var._rotation_absolute, tc1);
  } /* NL1B_Vertical_Guide=Guide_channeled() AT ROTATED */
  DEBUG_COMPONENT("NL1B_Vertical_Guide", _NL1B_Vertical_Guide_var._position_absolute, _NL1B_Vertical_Guide_var._rotation_absolute);
  instrument->_position_absolute[21] = _NL1B_Vertical_Guide_var._position_absolute;
  instrument->_position_relative[21] = _NL1B_Vertical_Guide_var._position_relative;
    _NL1B_Vertical_Guide_var._position_relative_is_zero =  coords_test_zero(_NL1B_Vertical_Guide_var._position_relative);
  instrument->counter_N[21]  = instrument->counter_P[21] = instrument->counter_P2[21] = 0;
  instrument->counter_AbsorbProp[21]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0020_NL1B_Vertical_Guide", _NL1B_Vertical_Guide_var._position_absolute, _NL1B_Vertical_Guide_var._rotation_absolute, "Guide_channeled");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "w1", "NONE", "0.03","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "h1", "NONE", "0.125","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "w2", "0", "0.15","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "h2", "0", "0.125","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "l", "NONE", "1.475","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "R0", "0.995", "R0_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "Qc", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "alpha", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "m", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "nslit", "1", "1","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "d", "0.0005", "0.0005","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "Qcx", "0.0218", "Qc_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "Qcy", "0.0218", "Qc_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "alphax", "4.38", "alpha_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "alphay", "4.38", "alpha_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "W", "0.003", "W_para","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "mx", "1", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "my", "1", "MGUIDE","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "nu", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0020_NL1B_Vertical_Guide", "phase", "0", "0","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _NL1B_Vertical_Guide_setpos */

/* component NL1B_Guide_Exit=Arm() SETTING, POSITION/ROTATION */
int _NL1B_Guide_Exit_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_NL1B_Guide_Exit_setpos] component NL1B_Guide_Exit=Arm() SETTING [Arm:0]");
  stracpy(_NL1B_Guide_Exit_var._name, "NL1B_Guide_Exit", 16384);
  stracpy(_NL1B_Guide_Exit_var._type, "Arm", 16384);
  _NL1B_Guide_Exit_var._index=22;
  int current_setpos_index = 22;
  /* component NL1B_Guide_Exit=Arm() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (0)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _NL1B_Vertical_Guide_Entrance_Window_var._rotation_absolute, _NL1B_Guide_Exit_var._rotation_absolute);
    rot_transpose(_NL1B_Vertical_Guide_var._rotation_absolute, tr1);
    rot_mul(_NL1B_Guide_Exit_var._rotation_absolute, tr1, _NL1B_Guide_Exit_var._rotation_relative);
    _NL1B_Guide_Exit_var._rotation_is_identity =  rot_test_identity(_NL1B_Guide_Exit_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 1.475);
    rot_transpose(_NL1B_Vertical_Guide_Entrance_Window_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _NL1B_Guide_Exit_var._position_absolute = coords_add(_NL1B_Vertical_Guide_Entrance_Window_var._position_absolute, tc2);
    tc1 = coords_sub(_NL1B_Vertical_Guide_var._position_absolute, _NL1B_Guide_Exit_var._position_absolute);
    _NL1B_Guide_Exit_var._position_relative = rot_apply(_NL1B_Guide_Exit_var._rotation_absolute, tc1);
  } /* NL1B_Guide_Exit=Arm() AT ROTATED */
  DEBUG_COMPONENT("NL1B_Guide_Exit", _NL1B_Guide_Exit_var._position_absolute, _NL1B_Guide_Exit_var._rotation_absolute);
  instrument->_position_absolute[22] = _NL1B_Guide_Exit_var._position_absolute;
  instrument->_position_relative[22] = _NL1B_Guide_Exit_var._position_relative;
    _NL1B_Guide_Exit_var._position_relative_is_zero =  coords_test_zero(_NL1B_Guide_Exit_var._position_relative);
  instrument->counter_N[22]  = instrument->counter_P[22] = instrument->counter_P2[22] = 0;
  instrument->counter_AbsorbProp[22]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0021_NL1B_Guide_Exit", _NL1B_Guide_Exit_var._position_absolute, _NL1B_Guide_Exit_var._rotation_absolute, "Arm");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _NL1B_Guide_Exit_setpos */

/* component energy_endguide=E_monitor() SETTING, POSITION/ROTATION */
int _energy_endguide_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_energy_endguide_setpos] component energy_endguide=E_monitor() SETTING [E_monitor:0]");
  stracpy(_energy_endguide_var._name, "energy_endguide", 16384);
  stracpy(_energy_endguide_var._type, "E_monitor", 16384);
  _energy_endguide_var._index=23;
  int current_setpos_index = 23;
  _energy_endguide_var._parameters.nE = 200;
  if("energy-endguide.dat" && strlen("energy-endguide.dat"))
    stracpy(_energy_endguide_var._parameters.filename, "energy-endguide.dat" ? "energy-endguide.dat" : "", 16384);
  else 
  _energy_endguide_var._parameters.filename[0]='\0';
  _energy_endguide_var._parameters.xmin = -0.05;
  _energy_endguide_var._parameters.xmax = 0.05;
  _energy_endguide_var._parameters.ymin = -0.05;
  _energy_endguide_var._parameters.ymax = 0.05;
  _energy_endguide_var._parameters.nowritefile = 0;
  _energy_endguide_var._parameters.xwidth = 0.17;
  _energy_endguide_var._parameters.yheight = 0.135;
  _energy_endguide_var._parameters.Emin = e_min;
  _energy_endguide_var._parameters.Emax = e_max;
  _energy_endguide_var._parameters.restore_neutron = 1;


  /* component energy_endguide=E_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _NL1B_Guide_Exit_var._rotation_absolute, _energy_endguide_var._rotation_absolute);
    rot_transpose(_NL1B_Vertical_Guide_var._rotation_absolute, tr1);
    rot_mul(_energy_endguide_var._rotation_absolute, tr1, _energy_endguide_var._rotation_relative);
    _energy_endguide_var._rotation_is_identity =  rot_test_identity(_energy_endguide_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0);
    rot_transpose(_NL1B_Guide_Exit_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _energy_endguide_var._position_absolute = coords_add(_NL1B_Guide_Exit_var._position_absolute, tc2);
    tc1 = coords_sub(_NL1B_Vertical_Guide_var._position_absolute, _energy_endguide_var._position_absolute);
    _energy_endguide_var._position_relative = rot_apply(_energy_endguide_var._rotation_absolute, tc1);
  } /* energy_endguide=E_monitor() AT ROTATED */
  DEBUG_COMPONENT("energy_endguide", _energy_endguide_var._position_absolute, _energy_endguide_var._rotation_absolute);
  instrument->_position_absolute[23] = _energy_endguide_var._position_absolute;
  instrument->_position_relative[23] = _energy_endguide_var._position_relative;
    _energy_endguide_var._position_relative_is_zero =  coords_test_zero(_energy_endguide_var._position_relative);
  instrument->counter_N[23]  = instrument->counter_P[23] = instrument->counter_P2[23] = 0;
  instrument->counter_AbsorbProp[23]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0022_energy_endguide", _energy_endguide_var._position_absolute, _energy_endguide_var._rotation_absolute, "E_monitor");
        mccomp_param_nexus(nxhandle,"0022_energy_endguide", "nE", "20", "200","int");
        mccomp_param_nexus(nxhandle,"0022_energy_endguide", "filename", 0, "energy-endguide.dat", "char*");
        mccomp_param_nexus(nxhandle,"0022_energy_endguide", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0022_energy_endguide", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0022_energy_endguide", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0022_energy_endguide", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0022_energy_endguide", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0022_energy_endguide", "xwidth", "0", "0.17","MCNUM");
        mccomp_param_nexus(nxhandle,"0022_energy_endguide", "yheight", "0", "0.135","MCNUM");
        mccomp_param_nexus(nxhandle,"0022_energy_endguide", "Emin", "NONE", "e_min","MCNUM");
        mccomp_param_nexus(nxhandle,"0022_energy_endguide", "Emax", "NONE", "e_max","MCNUM");
        mccomp_param_nexus(nxhandle,"0022_energy_endguide", "restore_neutron", "0", "1","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _energy_endguide_setpos */

/* component Mono_center=Arm() SETTING, POSITION/ROTATION */
int _Mono_center_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Mono_center_setpos] component Mono_center=Arm() SETTING [Arm:0]");
  stracpy(_Mono_center_var._name, "Mono_center", 16384);
  stracpy(_Mono_center_var._type, "Arm", 16384);
  _Mono_center_var._index=24;
  int current_setpos_index = 24;
  /* component Mono_center=Arm() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (A1)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _NL1B_Guide_Exit_var._rotation_absolute, _Mono_center_var._rotation_absolute);
    rot_transpose(_energy_endguide_var._rotation_absolute, tr1);
    rot_mul(_Mono_center_var._rotation_absolute, tr1, _Mono_center_var._rotation_relative);
    _Mono_center_var._rotation_is_identity =  rot_test_identity(_Mono_center_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0.25);
    rot_transpose(_NL1B_Guide_Exit_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Mono_center_var._position_absolute = coords_add(_NL1B_Guide_Exit_var._position_absolute, tc2);
    tc1 = coords_sub(_energy_endguide_var._position_absolute, _Mono_center_var._position_absolute);
    _Mono_center_var._position_relative = rot_apply(_Mono_center_var._rotation_absolute, tc1);
  } /* Mono_center=Arm() AT ROTATED */
  DEBUG_COMPONENT("Mono_center", _Mono_center_var._position_absolute, _Mono_center_var._rotation_absolute);
  instrument->_position_absolute[24] = _Mono_center_var._position_absolute;
  instrument->_position_relative[24] = _Mono_center_var._position_relative;
    _Mono_center_var._position_relative_is_zero =  coords_test_zero(_Mono_center_var._position_relative);
  instrument->counter_N[24]  = instrument->counter_P[24] = instrument->counter_P2[24] = 0;
  instrument->counter_AbsorbProp[24]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0023_Mono_center", _Mono_center_var._position_absolute, _Mono_center_var._rotation_absolute, "Arm");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Mono_center_setpos */

/* component Monochromator=Monochromator_curved() SETTING, POSITION/ROTATION */
int _Monochromator_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Monochromator_setpos] component Monochromator=Monochromator_curved() SETTING [Monochromator_curved:0]");
  stracpy(_Monochromator_var._name, "Monochromator", 16384);
  stracpy(_Monochromator_var._type, "Monochromator_curved", 16384);
  _Monochromator_var._index=25;
  int current_setpos_index = 25;
  if("NULL" && strlen("NULL"))
    stracpy(_Monochromator_var._parameters.reflect, "NULL" ? "NULL" : "", 16384);
  else 
  _Monochromator_var._parameters.reflect[0]='\0';
  if("NULL" && strlen("NULL"))
    stracpy(_Monochromator_var._parameters.transmit, "NULL" ? "NULL" : "", 16384);
  else 
  _Monochromator_var._parameters.transmit[0]='\0';
  _Monochromator_var._parameters.zwidth = 0.02;
  _Monochromator_var._parameters.yheight = 0.02;
  _Monochromator_var._parameters.gap = 0.0005;
  _Monochromator_var._parameters.NH = 15;
  _Monochromator_var._parameters.NV = 7;
  _Monochromator_var._parameters.mosaich = 40;
  _Monochromator_var._parameters.mosaicv = 40;
  _Monochromator_var._parameters.r0 = 1.0;
  _Monochromator_var._parameters.t0 = 0.00001;
  _Monochromator_var._parameters.Q = 1.8734;
  _Monochromator_var._parameters.RV = RMV;
  _Monochromator_var._parameters.RH = RMH;
  _Monochromator_var._parameters.DM = 0;
  _Monochromator_var._parameters.mosaic = 0;
  _Monochromator_var._parameters.width = 0;
  _Monochromator_var._parameters.height = 0;
  _Monochromator_var._parameters.verbose = 0;
  _Monochromator_var._parameters.order = 0;


  /* component Monochromator=Monochromator_curved() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (0)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _Mono_center_var._rotation_absolute, _Monochromator_var._rotation_absolute);
    rot_transpose(_energy_endguide_var._rotation_absolute, tr1);
    rot_mul(_Monochromator_var._rotation_absolute, tr1, _Monochromator_var._rotation_relative);
    _Monochromator_var._rotation_is_identity =  rot_test_identity(_Monochromator_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0);
    rot_transpose(_Mono_center_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Monochromator_var._position_absolute = coords_add(_Mono_center_var._position_absolute, tc2);
    tc1 = coords_sub(_energy_endguide_var._position_absolute, _Monochromator_var._position_absolute);
    _Monochromator_var._position_relative = rot_apply(_Monochromator_var._rotation_absolute, tc1);
  } /* Monochromator=Monochromator_curved() AT ROTATED */
  DEBUG_COMPONENT("Monochromator", _Monochromator_var._position_absolute, _Monochromator_var._rotation_absolute);
  instrument->_position_absolute[25] = _Monochromator_var._position_absolute;
  instrument->_position_relative[25] = _Monochromator_var._position_relative;
    _Monochromator_var._position_relative_is_zero =  coords_test_zero(_Monochromator_var._position_relative);
  instrument->counter_N[25]  = instrument->counter_P[25] = instrument->counter_P2[25] = 0;
  instrument->counter_AbsorbProp[25]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0024_Monochromator", _Monochromator_var._position_absolute, _Monochromator_var._rotation_absolute, "Monochromator_curved");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "reflect", "NULL", "NULL", "char*");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "transmit", "NULL", "NULL", "char*");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "zwidth", "0.01", "0.02","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "yheight", "0.01", "0.02","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "gap", "0.0005", "0.0005","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "NH", "11", "15","int");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "NV", "11", "7","int");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "mosaich", "30.0", "40","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "mosaicv", "30.0", "40","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "r0", "0.7", "1.0","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "t0", "1.0", "0.00001","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "Q", "1.8734", "1.8734","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "RV", "0", "RMV","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "RH", "0", "RMH","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "DM", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "mosaic", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "width", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "height", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "verbose", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0024_Monochromator", "order", "0", "0","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Monochromator_setpos */

/* component Mono_sample_arm=Arm() SETTING, POSITION/ROTATION */
int _Mono_sample_arm_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Mono_sample_arm_setpos] component Mono_sample_arm=Arm() SETTING [Arm:0]");
  stracpy(_Mono_sample_arm_var._name, "Mono_sample_arm", 16384);
  stracpy(_Mono_sample_arm_var._type, "Arm", 16384);
  _Mono_sample_arm_var._index=26;
  int current_setpos_index = 26;
  /* component Mono_sample_arm=Arm() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (A2)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _NL1B_Guide_Exit_var._rotation_absolute, _Mono_sample_arm_var._rotation_absolute);
    rot_transpose(_Monochromator_var._rotation_absolute, tr1);
    rot_mul(_Mono_sample_arm_var._rotation_absolute, tr1, _Mono_sample_arm_var._rotation_relative);
    _Mono_sample_arm_var._rotation_is_identity =  rot_test_identity(_Mono_sample_arm_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0);
    rot_transpose(_Mono_center_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Mono_sample_arm_var._position_absolute = coords_add(_Mono_center_var._position_absolute, tc2);
    tc1 = coords_sub(_Monochromator_var._position_absolute, _Mono_sample_arm_var._position_absolute);
    _Mono_sample_arm_var._position_relative = rot_apply(_Mono_sample_arm_var._rotation_absolute, tc1);
  } /* Mono_sample_arm=Arm() AT ROTATED */
  DEBUG_COMPONENT("Mono_sample_arm", _Mono_sample_arm_var._position_absolute, _Mono_sample_arm_var._rotation_absolute);
  instrument->_position_absolute[26] = _Mono_sample_arm_var._position_absolute;
  instrument->_position_relative[26] = _Mono_sample_arm_var._position_relative;
    _Mono_sample_arm_var._position_relative_is_zero =  coords_test_zero(_Mono_sample_arm_var._position_relative);
  instrument->counter_N[26]  = instrument->counter_P[26] = instrument->counter_P2[26] = 0;
  instrument->counter_AbsorbProp[26]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0025_Mono_sample_arm", _Mono_sample_arm_var._position_absolute, _Mono_sample_arm_var._rotation_absolute, "Arm");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Mono_sample_arm_setpos */

/* component energy_mono=E_monitor() SETTING, POSITION/ROTATION */
int _energy_mono_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_energy_mono_setpos] component energy_mono=E_monitor() SETTING [E_monitor:0]");
  stracpy(_energy_mono_var._name, "energy_mono", 16384);
  stracpy(_energy_mono_var._type, "E_monitor", 16384);
  _energy_mono_var._index=27;
  int current_setpos_index = 27;
  _energy_mono_var._parameters.nE = 200;
  if("energy-mono.dat" && strlen("energy-mono.dat"))
    stracpy(_energy_mono_var._parameters.filename, "energy-mono.dat" ? "energy-mono.dat" : "", 16384);
  else 
  _energy_mono_var._parameters.filename[0]='\0';
  _energy_mono_var._parameters.xmin = -0.05;
  _energy_mono_var._parameters.xmax = 0.05;
  _energy_mono_var._parameters.ymin = -0.05;
  _energy_mono_var._parameters.ymax = 0.05;
  _energy_mono_var._parameters.nowritefile = 0;
  _energy_mono_var._parameters.xwidth = 0.2;
  _energy_mono_var._parameters.yheight = 0.125;
  _energy_mono_var._parameters.Emin = e_min;
  _energy_mono_var._parameters.Emax = e_max;
  _energy_mono_var._parameters.restore_neutron = 1;


  /* component energy_mono=E_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Mono_sample_arm_var._rotation_absolute, _energy_mono_var._rotation_absolute);
    rot_transpose(_Monochromator_var._rotation_absolute, tr1);
    rot_mul(_energy_mono_var._rotation_absolute, tr1, _energy_mono_var._rotation_relative);
    _energy_mono_var._rotation_is_identity =  rot_test_identity(_energy_mono_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0.15);
    rot_transpose(_Mono_sample_arm_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _energy_mono_var._position_absolute = coords_add(_Mono_sample_arm_var._position_absolute, tc2);
    tc1 = coords_sub(_Monochromator_var._position_absolute, _energy_mono_var._position_absolute);
    _energy_mono_var._position_relative = rot_apply(_energy_mono_var._rotation_absolute, tc1);
  } /* energy_mono=E_monitor() AT ROTATED */
  DEBUG_COMPONENT("energy_mono", _energy_mono_var._position_absolute, _energy_mono_var._rotation_absolute);
  instrument->_position_absolute[27] = _energy_mono_var._position_absolute;
  instrument->_position_relative[27] = _energy_mono_var._position_relative;
    _energy_mono_var._position_relative_is_zero =  coords_test_zero(_energy_mono_var._position_relative);
  instrument->counter_N[27]  = instrument->counter_P[27] = instrument->counter_P2[27] = 0;
  instrument->counter_AbsorbProp[27]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0026_energy_mono", _energy_mono_var._position_absolute, _energy_mono_var._rotation_absolute, "E_monitor");
        mccomp_param_nexus(nxhandle,"0026_energy_mono", "nE", "20", "200","int");
        mccomp_param_nexus(nxhandle,"0026_energy_mono", "filename", 0, "energy-mono.dat", "char*");
        mccomp_param_nexus(nxhandle,"0026_energy_mono", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0026_energy_mono", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0026_energy_mono", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0026_energy_mono", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0026_energy_mono", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0026_energy_mono", "xwidth", "0", "0.2","MCNUM");
        mccomp_param_nexus(nxhandle,"0026_energy_mono", "yheight", "0", "0.125","MCNUM");
        mccomp_param_nexus(nxhandle,"0026_energy_mono", "Emin", "NONE", "e_min","MCNUM");
        mccomp_param_nexus(nxhandle,"0026_energy_mono", "Emax", "NONE", "e_max","MCNUM");
        mccomp_param_nexus(nxhandle,"0026_energy_mono", "restore_neutron", "0", "1","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _energy_mono_setpos */

/* component energy_pre_sample=E_monitor() SETTING, POSITION/ROTATION */
int _energy_pre_sample_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_energy_pre_sample_setpos] component energy_pre_sample=E_monitor() SETTING [E_monitor:0]");
  stracpy(_energy_pre_sample_var._name, "energy_pre_sample", 16384);
  stracpy(_energy_pre_sample_var._type, "E_monitor", 16384);
  _energy_pre_sample_var._index=28;
  int current_setpos_index = 28;
  _energy_pre_sample_var._parameters.nE = 50;
  if("energy-pre-sample.dat" && strlen("energy-pre-sample.dat"))
    stracpy(_energy_pre_sample_var._parameters.filename, "energy-pre-sample.dat" ? "energy-pre-sample.dat" : "", 16384);
  else 
  _energy_pre_sample_var._parameters.filename[0]='\0';
  _energy_pre_sample_var._parameters.xmin = -0.05;
  _energy_pre_sample_var._parameters.xmax = 0.05;
  _energy_pre_sample_var._parameters.ymin = -0.05;
  _energy_pre_sample_var._parameters.ymax = 0.05;
  _energy_pre_sample_var._parameters.nowritefile = 0;
  _energy_pre_sample_var._parameters.xwidth = 0.08;
  _energy_pre_sample_var._parameters.yheight = 0.08;
  _energy_pre_sample_var._parameters.Emin = e_min;
  _energy_pre_sample_var._parameters.Emax = e_max;
  _energy_pre_sample_var._parameters.restore_neutron = 1;


  /* component energy_pre_sample=E_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Mono_sample_arm_var._rotation_absolute, _energy_pre_sample_var._rotation_absolute);
    rot_transpose(_energy_mono_var._rotation_absolute, tr1);
    rot_mul(_energy_pre_sample_var._rotation_absolute, tr1, _energy_pre_sample_var._rotation_relative);
    _energy_pre_sample_var._rotation_is_identity =  rot_test_identity(_energy_pre_sample_var._rotation_relative);
    tc1 = coords_set(
      0, 0, L2 -0.1);
    rot_transpose(_Mono_sample_arm_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _energy_pre_sample_var._position_absolute = coords_add(_Mono_sample_arm_var._position_absolute, tc2);
    tc1 = coords_sub(_energy_mono_var._position_absolute, _energy_pre_sample_var._position_absolute);
    _energy_pre_sample_var._position_relative = rot_apply(_energy_pre_sample_var._rotation_absolute, tc1);
  } /* energy_pre_sample=E_monitor() AT ROTATED */
  DEBUG_COMPONENT("energy_pre_sample", _energy_pre_sample_var._position_absolute, _energy_pre_sample_var._rotation_absolute);
  instrument->_position_absolute[28] = _energy_pre_sample_var._position_absolute;
  instrument->_position_relative[28] = _energy_pre_sample_var._position_relative;
    _energy_pre_sample_var._position_relative_is_zero =  coords_test_zero(_energy_pre_sample_var._position_relative);
  instrument->counter_N[28]  = instrument->counter_P[28] = instrument->counter_P2[28] = 0;
  instrument->counter_AbsorbProp[28]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0027_energy_pre_sample", _energy_pre_sample_var._position_absolute, _energy_pre_sample_var._rotation_absolute, "E_monitor");
        mccomp_param_nexus(nxhandle,"0027_energy_pre_sample", "nE", "20", "50","int");
        mccomp_param_nexus(nxhandle,"0027_energy_pre_sample", "filename", 0, "energy-pre-sample.dat", "char*");
        mccomp_param_nexus(nxhandle,"0027_energy_pre_sample", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0027_energy_pre_sample", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0027_energy_pre_sample", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0027_energy_pre_sample", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0027_energy_pre_sample", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0027_energy_pre_sample", "xwidth", "0", "0.08","MCNUM");
        mccomp_param_nexus(nxhandle,"0027_energy_pre_sample", "yheight", "0", "0.08","MCNUM");
        mccomp_param_nexus(nxhandle,"0027_energy_pre_sample", "Emin", "NONE", "e_min","MCNUM");
        mccomp_param_nexus(nxhandle,"0027_energy_pre_sample", "Emax", "NONE", "e_max","MCNUM");
        mccomp_param_nexus(nxhandle,"0027_energy_pre_sample", "restore_neutron", "0", "1","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _energy_pre_sample_setpos */

/* component Sample_center=Arm() SETTING, POSITION/ROTATION */
int _Sample_center_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Sample_center_setpos] component Sample_center=Arm() SETTING [Arm:0]");
  stracpy(_Sample_center_var._name, "Sample_center", 16384);
  stracpy(_Sample_center_var._type, "Arm", 16384);
  _Sample_center_var._index=29;
  int current_setpos_index = 29;
  /* component Sample_center=Arm() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (_instrument_var._parameters.A3)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _Mono_sample_arm_var._rotation_absolute, _Sample_center_var._rotation_absolute);
    rot_transpose(_energy_pre_sample_var._rotation_absolute, tr1);
    rot_mul(_Sample_center_var._rotation_absolute, tr1, _Sample_center_var._rotation_relative);
    _Sample_center_var._rotation_is_identity =  rot_test_identity(_Sample_center_var._rotation_relative);
    tc1 = coords_set(
      0, 0, L2);
    rot_transpose(_Mono_sample_arm_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Sample_center_var._position_absolute = coords_add(_Mono_sample_arm_var._position_absolute, tc2);
    tc1 = coords_sub(_energy_pre_sample_var._position_absolute, _Sample_center_var._position_absolute);
    _Sample_center_var._position_relative = rot_apply(_Sample_center_var._rotation_absolute, tc1);
  } /* Sample_center=Arm() AT ROTATED */
  DEBUG_COMPONENT("Sample_center", _Sample_center_var._position_absolute, _Sample_center_var._rotation_absolute);
  instrument->_position_absolute[29] = _Sample_center_var._position_absolute;
  instrument->_position_relative[29] = _Sample_center_var._position_relative;
    _Sample_center_var._position_relative_is_zero =  coords_test_zero(_Sample_center_var._position_relative);
  instrument->counter_N[29]  = instrument->counter_P[29] = instrument->counter_P2[29] = 0;
  instrument->counter_AbsorbProp[29]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0028_Sample_center", _Sample_center_var._position_absolute, _Sample_center_var._rotation_absolute, "Arm");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Sample_center_setpos */

/* component Sample_analyser_arm=Arm() SETTING, POSITION/ROTATION */
int _Sample_analyser_arm_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_Sample_analyser_arm_setpos] component Sample_analyser_arm=Arm() SETTING [Arm:0]");
  stracpy(_Sample_analyser_arm_var._name, "Sample_analyser_arm", 16384);
  stracpy(_Sample_analyser_arm_var._type, "Arm", 16384);
  _Sample_analyser_arm_var._index=30;
  int current_setpos_index = 30;
  /* component Sample_analyser_arm=Arm() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (_instrument_var._parameters.A4)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _Mono_sample_arm_var._rotation_absolute, _Sample_analyser_arm_var._rotation_absolute);
    rot_transpose(_energy_pre_sample_var._rotation_absolute, tr1);
    rot_mul(_Sample_analyser_arm_var._rotation_absolute, tr1, _Sample_analyser_arm_var._rotation_relative);
    _Sample_analyser_arm_var._rotation_is_identity =  rot_test_identity(_Sample_analyser_arm_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0);
    rot_transpose(_Sample_center_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _Sample_analyser_arm_var._position_absolute = coords_add(_Sample_center_var._position_absolute, tc2);
    tc1 = coords_sub(_energy_pre_sample_var._position_absolute, _Sample_analyser_arm_var._position_absolute);
    _Sample_analyser_arm_var._position_relative = rot_apply(_Sample_analyser_arm_var._rotation_absolute, tc1);
  } /* Sample_analyser_arm=Arm() AT ROTATED */
  DEBUG_COMPONENT("Sample_analyser_arm", _Sample_analyser_arm_var._position_absolute, _Sample_analyser_arm_var._rotation_absolute);
  instrument->_position_absolute[30] = _Sample_analyser_arm_var._position_absolute;
  instrument->_position_relative[30] = _Sample_analyser_arm_var._position_relative;
    _Sample_analyser_arm_var._position_relative_is_zero =  coords_test_zero(_Sample_analyser_arm_var._position_relative);
  instrument->counter_N[30]  = instrument->counter_P[30] = instrument->counter_P2[30] = 0;
  instrument->counter_AbsorbProp[30]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0029_Sample_analyser_arm", _Sample_analyser_arm_var._position_absolute, _Sample_analyser_arm_var._rotation_absolute, "Arm");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _Sample_analyser_arm_setpos */

/* component div_mono=Divergence_monitor() SETTING, POSITION/ROTATION */
int _div_mono_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_div_mono_setpos] component div_mono=Divergence_monitor() SETTING [Divergence_monitor:0]");
  stracpy(_div_mono_var._name, "div_mono", 16384);
  stracpy(_div_mono_var._type, "Divergence_monitor", 16384);
  _div_mono_var._index=31;
  int current_setpos_index = 31;
  _div_mono_var._parameters.nh = 20;
  _div_mono_var._parameters.nv = 20;
  if("div-sample.dat" && strlen("div-sample.dat"))
    stracpy(_div_mono_var._parameters.filename, "div-sample.dat" ? "div-sample.dat" : "", 16384);
  else 
  _div_mono_var._parameters.filename[0]='\0';
  _div_mono_var._parameters.xmin = -0.05;
  _div_mono_var._parameters.xmax = 0.05;
  _div_mono_var._parameters.ymin = -0.05;
  _div_mono_var._parameters.ymax = 0.05;
  _div_mono_var._parameters.nowritefile = 0;
  _div_mono_var._parameters.xwidth = 0.1;
  _div_mono_var._parameters.yheight = 0.1;
  _div_mono_var._parameters.maxdiv_h = 4;
  _div_mono_var._parameters.maxdiv_v = 4;
  _div_mono_var._parameters.restore_neutron = 1;
  _div_mono_var._parameters.nx = 0;
  _div_mono_var._parameters.ny = 0;
  _div_mono_var._parameters.nz = 1;


  /* component div_mono=Divergence_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Mono_sample_arm_var._rotation_absolute, _div_mono_var._rotation_absolute);
    rot_transpose(_energy_pre_sample_var._rotation_absolute, tr1);
    rot_mul(_div_mono_var._rotation_absolute, tr1, _div_mono_var._rotation_relative);
    _div_mono_var._rotation_is_identity =  rot_test_identity(_div_mono_var._rotation_relative);
    tc1 = coords_set(
      0, 0, L2 -0.05);
    rot_transpose(_Mono_sample_arm_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _div_mono_var._position_absolute = coords_add(_Mono_sample_arm_var._position_absolute, tc2);
    tc1 = coords_sub(_energy_pre_sample_var._position_absolute, _div_mono_var._position_absolute);
    _div_mono_var._position_relative = rot_apply(_div_mono_var._rotation_absolute, tc1);
  } /* div_mono=Divergence_monitor() AT ROTATED */
  DEBUG_COMPONENT("div_mono", _div_mono_var._position_absolute, _div_mono_var._rotation_absolute);
  instrument->_position_absolute[31] = _div_mono_var._position_absolute;
  instrument->_position_relative[31] = _div_mono_var._position_relative;
    _div_mono_var._position_relative_is_zero =  coords_test_zero(_div_mono_var._position_relative);
  instrument->counter_N[31]  = instrument->counter_P[31] = instrument->counter_P2[31] = 0;
  instrument->counter_AbsorbProp[31]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0030_div_mono", _div_mono_var._position_absolute, _div_mono_var._rotation_absolute, "Divergence_monitor");
        mccomp_param_nexus(nxhandle,"0030_div_mono", "nh", "20", "20","int");
        mccomp_param_nexus(nxhandle,"0030_div_mono", "nv", "20", "20","int");
        mccomp_param_nexus(nxhandle,"0030_div_mono", "filename", 0, "div-sample.dat", "char*");
        mccomp_param_nexus(nxhandle,"0030_div_mono", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_div_mono", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_div_mono", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_div_mono", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_div_mono", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0030_div_mono", "xwidth", "0", "0.1","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_div_mono", "yheight", "0", "0.1","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_div_mono", "maxdiv_h", "2", "4","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_div_mono", "maxdiv_v", "2", "4","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_div_mono", "restore_neutron", "0", "1","int");
        mccomp_param_nexus(nxhandle,"0030_div_mono", "nx", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_div_mono", "ny", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0030_div_mono", "nz", "1", "1","MCNUM");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _div_mono_setpos */

/* component div_mono_H=Div1D_monitor() SETTING, POSITION/ROTATION */
int _div_mono_H_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_div_mono_H_setpos] component div_mono_H=Div1D_monitor() SETTING [Div1D_monitor:0]");
  stracpy(_div_mono_H_var._name, "div_mono_H", 16384);
  stracpy(_div_mono_H_var._type, "Div1D_monitor", 16384);
  _div_mono_H_var._index=32;
  int current_setpos_index = 32;
  _div_mono_H_var._parameters.ndiv = 35;
  if("H-div-sample.dat" && strlen("H-div-sample.dat"))
    stracpy(_div_mono_H_var._parameters.filename, "H-div-sample.dat" ? "H-div-sample.dat" : "", 16384);
  else 
  _div_mono_H_var._parameters.filename[0]='\0';
  _div_mono_H_var._parameters.xmin = -0.05;
  _div_mono_H_var._parameters.xmax = 0.05;
  _div_mono_H_var._parameters.ymin = -0.05;
  _div_mono_H_var._parameters.ymax = 0.05;
  _div_mono_H_var._parameters.xwidth = 0.005;
  _div_mono_H_var._parameters.yheight = 0.7;
  _div_mono_H_var._parameters.maxdiv = 4;
  _div_mono_H_var._parameters.restore_neutron = 1;
  _div_mono_H_var._parameters.nowritefile = 0;
  _div_mono_H_var._parameters.vertical = 0;


  /* component div_mono_H=Div1D_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0.0)*DEG2RAD, (0.0)*DEG2RAD, (0.0)*DEG2RAD);
    rot_mul(tr1, _Mono_sample_arm_var._rotation_absolute, _div_mono_H_var._rotation_absolute);
    rot_transpose(_div_mono_var._rotation_absolute, tr1);
    rot_mul(_div_mono_H_var._rotation_absolute, tr1, _div_mono_H_var._rotation_relative);
    _div_mono_H_var._rotation_is_identity =  rot_test_identity(_div_mono_H_var._rotation_relative);
    tc1 = coords_set(
      0, 0, L2 -0.04);
    rot_transpose(_Mono_sample_arm_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _div_mono_H_var._position_absolute = coords_add(_Mono_sample_arm_var._position_absolute, tc2);
    tc1 = coords_sub(_div_mono_var._position_absolute, _div_mono_H_var._position_absolute);
    _div_mono_H_var._position_relative = rot_apply(_div_mono_H_var._rotation_absolute, tc1);
  } /* div_mono_H=Div1D_monitor() AT ROTATED */
  DEBUG_COMPONENT("div_mono_H", _div_mono_H_var._position_absolute, _div_mono_H_var._rotation_absolute);
  instrument->_position_absolute[32] = _div_mono_H_var._position_absolute;
  instrument->_position_relative[32] = _div_mono_H_var._position_relative;
    _div_mono_H_var._position_relative_is_zero =  coords_test_zero(_div_mono_H_var._position_relative);
  instrument->counter_N[32]  = instrument->counter_P[32] = instrument->counter_P2[32] = 0;
  instrument->counter_AbsorbProp[32]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0031_div_mono_H", _div_mono_H_var._position_absolute, _div_mono_H_var._rotation_absolute, "Div1D_monitor");
        mccomp_param_nexus(nxhandle,"0031_div_mono_H", "ndiv", "20", "35","int");
        mccomp_param_nexus(nxhandle,"0031_div_mono_H", "filename", 0, "H-div-sample.dat", "char*");
        mccomp_param_nexus(nxhandle,"0031_div_mono_H", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0031_div_mono_H", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0031_div_mono_H", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0031_div_mono_H", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0031_div_mono_H", "xwidth", "0", "0.005","MCNUM");
        mccomp_param_nexus(nxhandle,"0031_div_mono_H", "yheight", "0", "0.7","MCNUM");
        mccomp_param_nexus(nxhandle,"0031_div_mono_H", "maxdiv", "2", "4","MCNUM");
        mccomp_param_nexus(nxhandle,"0031_div_mono_H", "restore_neutron", "0", "1","int");
        mccomp_param_nexus(nxhandle,"0031_div_mono_H", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0031_div_mono_H", "vertical", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _div_mono_H_setpos */

/* component psd_sam=PSD_monitor() SETTING, POSITION/ROTATION */
int _psd_sam_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_psd_sam_setpos] component psd_sam=PSD_monitor() SETTING [PSD_monitor:0]");
  stracpy(_psd_sam_var._name, "psd_sam", 16384);
  stracpy(_psd_sam_var._type, "PSD_monitor", 16384);
  _psd_sam_var._index=33;
  int current_setpos_index = 33;
  _psd_sam_var._parameters.nx = 60;
  _psd_sam_var._parameters.ny = 60;
  if("psd-sam.dat" && strlen("psd-sam.dat"))
    stracpy(_psd_sam_var._parameters.filename, "psd-sam.dat" ? "psd-sam.dat" : "", 16384);
  else 
  _psd_sam_var._parameters.filename[0]='\0';
  _psd_sam_var._parameters.xmin = -0.05;
  _psd_sam_var._parameters.xmax = 0.05;
  _psd_sam_var._parameters.ymin = -0.05;
  _psd_sam_var._parameters.ymax = 0.05;
  _psd_sam_var._parameters.xwidth = 0.20;
  _psd_sam_var._parameters.yheight = 0.20;
  _psd_sam_var._parameters.restore_neutron = 1;
  _psd_sam_var._parameters.nowritefile = 0;


  /* component psd_sam=PSD_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (0)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _Mono_sample_arm_var._rotation_absolute, _psd_sam_var._rotation_absolute);
    rot_transpose(_div_mono_H_var._rotation_absolute, tr1);
    rot_mul(_psd_sam_var._rotation_absolute, tr1, _psd_sam_var._rotation_relative);
    _psd_sam_var._rotation_is_identity =  rot_test_identity(_psd_sam_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0);
    rot_transpose(_Sample_center_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _psd_sam_var._position_absolute = coords_add(_Sample_center_var._position_absolute, tc2);
    tc1 = coords_sub(_div_mono_H_var._position_absolute, _psd_sam_var._position_absolute);
    _psd_sam_var._position_relative = rot_apply(_psd_sam_var._rotation_absolute, tc1);
  } /* psd_sam=PSD_monitor() AT ROTATED */
  DEBUG_COMPONENT("psd_sam", _psd_sam_var._position_absolute, _psd_sam_var._rotation_absolute);
  instrument->_position_absolute[33] = _psd_sam_var._position_absolute;
  instrument->_position_relative[33] = _psd_sam_var._position_relative;
    _psd_sam_var._position_relative_is_zero =  coords_test_zero(_psd_sam_var._position_relative);
  instrument->counter_N[33]  = instrument->counter_P[33] = instrument->counter_P2[33] = 0;
  instrument->counter_AbsorbProp[33]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0032_psd_sam", _psd_sam_var._position_absolute, _psd_sam_var._rotation_absolute, "PSD_monitor");
        mccomp_param_nexus(nxhandle,"0032_psd_sam", "nx", "90", "60","int");
        mccomp_param_nexus(nxhandle,"0032_psd_sam", "ny", "90", "60","int");
        mccomp_param_nexus(nxhandle,"0032_psd_sam", "filename", 0, "psd-sam.dat", "char*");
        mccomp_param_nexus(nxhandle,"0032_psd_sam", "xmin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0032_psd_sam", "xmax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0032_psd_sam", "ymin", "-0.05", "-0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0032_psd_sam", "ymax", "0.05", "0.05","MCNUM");
        mccomp_param_nexus(nxhandle,"0032_psd_sam", "xwidth", "0", "0.20","MCNUM");
        mccomp_param_nexus(nxhandle,"0032_psd_sam", "yheight", "0", "0.20","MCNUM");
        mccomp_param_nexus(nxhandle,"0032_psd_sam", "restore_neutron", "0", "1","int");
        mccomp_param_nexus(nxhandle,"0032_psd_sam", "nowritefile", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _psd_sam_setpos */

/* component dpsd1=PSDlin_monitor() SETTING, POSITION/ROTATION */
int _dpsd1_setpos(void)
{ /* sets initial component parameters, position and rotation */
  SIG_MESSAGE("[_dpsd1_setpos] component dpsd1=PSDlin_monitor() SETTING [PSDlin_monitor:0]");
  stracpy(_dpsd1_var._name, "dpsd1", 16384);
  stracpy(_dpsd1_var._type, "PSDlin_monitor", 16384);
  _dpsd1_var._index=34;
  int current_setpos_index = 34;
  _dpsd1_var._parameters.nbins = 30;
  if("linpsdsam.dat" && strlen("linpsdsam.dat"))
    stracpy(_dpsd1_var._parameters.filename, "linpsdsam.dat" ? "linpsdsam.dat" : "", 16384);
  else 
  _dpsd1_var._parameters.filename[0]='\0';
  _dpsd1_var._parameters.xmin = -0.1;
  _dpsd1_var._parameters.xmax = 0.1;
  _dpsd1_var._parameters.ymin = -0.1;
  _dpsd1_var._parameters.ymax = 0.1;
  _dpsd1_var._parameters.nowritefile = 0;
  _dpsd1_var._parameters.xwidth = 0;
  _dpsd1_var._parameters.yheight = 0;
  _dpsd1_var._parameters.restore_neutron = 0;
  _dpsd1_var._parameters.vertical = 0;


  /* component dpsd1=PSDlin_monitor() AT ROTATED */
  {
    Coords tc1, tc2;
    tc1 = coords_set(0,0,0);
    tc2 = coords_set(0,0,0);
    Rotation tr1;
    rot_set_rotation(tr1,0,0,0);
    rot_set_rotation(tr1,
      (0)*DEG2RAD, (0)*DEG2RAD, (0)*DEG2RAD);
    rot_mul(tr1, _Mono_sample_arm_var._rotation_absolute, _dpsd1_var._rotation_absolute);
    rot_transpose(_psd_sam_var._rotation_absolute, tr1);
    rot_mul(_dpsd1_var._rotation_absolute, tr1, _dpsd1_var._rotation_relative);
    _dpsd1_var._rotation_is_identity =  rot_test_identity(_dpsd1_var._rotation_relative);
    tc1 = coords_set(
      0, 0, 0);
    rot_transpose(_Sample_center_var._rotation_absolute, tr1);
    tc2 = rot_apply(tr1, tc1);
    _dpsd1_var._position_absolute = coords_add(_Sample_center_var._position_absolute, tc2);
    tc1 = coords_sub(_psd_sam_var._position_absolute, _dpsd1_var._position_absolute);
    _dpsd1_var._position_relative = rot_apply(_dpsd1_var._rotation_absolute, tc1);
  } /* dpsd1=PSDlin_monitor() AT ROTATED */
  DEBUG_COMPONENT("dpsd1", _dpsd1_var._position_absolute, _dpsd1_var._rotation_absolute);
  instrument->_position_absolute[34] = _dpsd1_var._position_absolute;
  instrument->_position_relative[34] = _dpsd1_var._position_relative;
    _dpsd1_var._position_relative_is_zero =  coords_test_zero(_dpsd1_var._position_relative);
  instrument->counter_N[34]  = instrument->counter_P[34] = instrument->counter_P2[34] = 0;
  instrument->counter_AbsorbProp[34]= 0;
  #ifdef USE_NEXUS
  if(nxhandle) {
    if ((!mcdotrace) && mcformat && strcasestr(mcformat, "NeXus")) {
    MPI_MASTER(
        mccomp_placement_type_nexus(nxhandle,"0033_dpsd1", _dpsd1_var._position_absolute, _dpsd1_var._rotation_absolute, "PSDlin_monitor");
        mccomp_param_nexus(nxhandle,"0033_dpsd1", "nbins", "20", "30","int");
        mccomp_param_nexus(nxhandle,"0033_dpsd1", "filename", 0, "linpsdsam.dat", "char*");
        mccomp_param_nexus(nxhandle,"0033_dpsd1", "xmin", "-0.05", "-0.1","MCNUM");
        mccomp_param_nexus(nxhandle,"0033_dpsd1", "xmax", "0.05", "0.1","MCNUM");
        mccomp_param_nexus(nxhandle,"0033_dpsd1", "ymin", "-0.05", "-0.1","MCNUM");
        mccomp_param_nexus(nxhandle,"0033_dpsd1", "ymax", "0.05", "0.1","MCNUM");
        mccomp_param_nexus(nxhandle,"0033_dpsd1", "nowritefile", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0033_dpsd1", "xwidth", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0033_dpsd1", "yheight", "0", "0","MCNUM");
        mccomp_param_nexus(nxhandle,"0033_dpsd1", "restore_neutron", "0", "0","int");
        mccomp_param_nexus(nxhandle,"0033_dpsd1", "vertical", "0", "0","int");
      );
    }
  } else {
    // fprintf(stderr,"NO NEXUS FILE");
  }
  #endif
  return(0);
} /* _dpsd1_setpos */

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

_class_Source_gen *class_Source_gen_init(_class_Source_gen *_comp
) {
  #define flux_file (_comp->_parameters.flux_file)
  #define xdiv_file (_comp->_parameters.xdiv_file)
  #define ydiv_file (_comp->_parameters.ydiv_file)
  #define radius (_comp->_parameters.radius)
  #define dist (_comp->_parameters.dist)
  #define focus_xw (_comp->_parameters.focus_xw)
  #define focus_yh (_comp->_parameters.focus_yh)
  #define focus_aw (_comp->_parameters.focus_aw)
  #define focus_ah (_comp->_parameters.focus_ah)
  #define E0 (_comp->_parameters.E0)
  #define dE (_comp->_parameters.dE)
  #define lambda0 (_comp->_parameters.lambda0)
  #define dlambda (_comp->_parameters.dlambda)
  #define I1 (_comp->_parameters.I1)
  #define yheight (_comp->_parameters.yheight)
  #define xwidth (_comp->_parameters.xwidth)
  #define verbose (_comp->_parameters.verbose)
  #define T1 (_comp->_parameters.T1)
  #define flux_file_perAA (_comp->_parameters.flux_file_perAA)
  #define flux_file_log (_comp->_parameters.flux_file_log)
  #define Lmin (_comp->_parameters.Lmin)
  #define Lmax (_comp->_parameters.Lmax)
  #define Emin (_comp->_parameters.Emin)
  #define Emax (_comp->_parameters.Emax)
  #define T2 (_comp->_parameters.T2)
  #define I2 (_comp->_parameters.I2)
  #define T3 (_comp->_parameters.T3)
  #define I3 (_comp->_parameters.I3)
  #define zdepth (_comp->_parameters.zdepth)
  #define target_index (_comp->_parameters.target_index)
  #define p_in (_comp->_parameters.p_in)
  #define lambda1 (_comp->_parameters.lambda1)
  #define lambda2 (_comp->_parameters.lambda2)
  #define lambda3 (_comp->_parameters.lambda3)
  #define pTable (_comp->_parameters.pTable)
  #define pTable_x (_comp->_parameters.pTable_x)
  #define pTable_y (_comp->_parameters.pTable_y)
  #define pTable_xmin (_comp->_parameters.pTable_xmin)
  #define pTable_xmax (_comp->_parameters.pTable_xmax)
  #define pTable_xsum (_comp->_parameters.pTable_xsum)
  #define pTable_ymin (_comp->_parameters.pTable_ymin)
  #define pTable_ymax (_comp->_parameters.pTable_ymax)
  #define pTable_ysum (_comp->_parameters.pTable_ysum)
  #define pTable_dxmin (_comp->_parameters.pTable_dxmin)
  #define pTable_dxmax (_comp->_parameters.pTable_dxmax)
  #define pTable_dymin (_comp->_parameters.pTable_dymin)
  #define pTable_dymax (_comp->_parameters.pTable_dymax)
  SIG_MESSAGE("[_Gen_Source_init] component Gen_Source=Source_gen() INITIALISE [Source_gen:0]");

  pTable_xsum=0;
  pTable_ysum=0;


  double source_area, k;

  if (target_index && !dist)
  {
    Coords ToTarget;
    double tx,ty,tz;
    ToTarget = coords_sub(POS_A_COMP_INDEX(INDEX_CURRENT_COMP+target_index),POS_A_CURRENT_COMP);
    ToTarget = rot_apply(ROT_A_CURRENT_COMP, ToTarget);
    coords_get(ToTarget, &tx, &ty, &tz);
    dist=sqrt(tx*tx+ty*ty+tz*tz);
  }

  /* spectrum characteristics */
  if (flux_file && strlen(flux_file) && strcmp(flux_file,"NULL") && strcmp(flux_file,"0")) {
    if (Table_Read(&pTable, flux_file, 1) <= 0) /* read 1st block data from file into pTable */
      exit(fprintf(stderr, "Source_gen: %s: can not read flux file %s\n", NAME_CURRENT_COMP, flux_file));
    /* put table in Log scale */
    int i;
    if (pTable.columns < 2) exit(fprintf(stderr, "Source_gen: %s: Flux file %s should contain at least 2 columns [wavelength in Angs,flux].\n", NAME_CURRENT_COMP, flux_file));
    double table_lmin=FLT_MAX, table_lmax=-FLT_MAX;
    double tmin=FLT_MAX, tmax=-FLT_MAX;
    for (i=0; i<pTable.rows; i++) {
      double val = Table_Index(pTable, i,1);
      val = Table_Index(pTable, i,0); /* lambda */
      if (val > tmax) tmax=val;
      if (val < tmin) tmin=val;
    }
    for (i=0; i<pTable.rows; i++) {
      double val = Table_Index(pTable, i,1);
      if (val < 0) fprintf(stderr, "Source_gen: %s: File %s has negative flux at row %i.\n", NAME_CURRENT_COMP, flux_file, i+1);
      if (flux_file_log)
        val = log(val > 0 ? val : tmin/10);
      Table_SetElement(&pTable, i, 1, val);
      val = Table_Index(pTable, i,0); /* lambda */
      if (val > table_lmax) table_lmax=val;
      if (val < table_lmin) table_lmin=val;
    }
    if (!Lmin && !Lmax && !lambda0 && !dlambda && !E0 && !dE && !Emin && !Emax) {
      Lmin = table_lmin; Lmax = table_lmax;
    }
    if (Lmax > table_lmax) {
      if (verbose) fprintf(stderr, "Source_gen: %s: Maximum wavelength %g is beyond table range upper limit %g. Constraining.\n", NAME_CURRENT_COMP, Lmax, table_lmax);
      Lmax = table_lmax;
    }
    if (Lmin < table_lmin) {
      if (verbose) fprintf(stderr, "Source_gen: %s: Minimum wavelength %g is below table range lower limit %g. Constraining.\n", NAME_CURRENT_COMP, Lmin, table_lmin);
      Lmin = table_lmin;
    }
  }  /* end flux file */
  else
  {
    k  = 1.38066e-23; /* k_B */
    if (T1 > 0)
    {
      lambda1  = 1.0e10*sqrt(HBAR*HBAR*4.0*PI*PI/2.0/MNEUTRON/k/T1);
    }
    else
      { lambda1 = lambda0; }

    if (T2 > 0)
    {
      lambda2  = 1.0e10*sqrt(HBAR*HBAR*4.0*PI*PI/2.0/MNEUTRON/k/T2);
    }
    else
      { lambda2 = lambda0; }

    if (T3 > 0)
    {
      lambda3  = 1.0e10*sqrt(HBAR*HBAR*4.0*PI*PI/2.0/MNEUTRON/k/T3);
    }
    else
      { lambda3 = lambda0; }
  }

  /* now read position-divergence files, if any */
  if (xdiv_file && strlen(xdiv_file) && strcmp(xdiv_file,"NULL") && strcmp(xdiv_file,"0")) {
    int i,j;
    if (Table_Read(&pTable_x, xdiv_file, 1) <= 0) /* read 1st block data from file into pTable */
      exit(fprintf(stderr, "Source_gen: %s: can not read XDiv file %s\n", NAME_CURRENT_COMP, xdiv_file));
    pTable_xsum = 0;
    for (i=0; i<pTable_x.rows; i++)
      for (j=0; j<pTable_x.columns; j++)
        pTable_xsum += Table_Index(pTable_x, i,j);

    /* now extract limits */
    char **parsing;
    char xylimits[1024];
    strcpy(xylimits, "");
    parsing = Table_ParseHeader(pTable_x.header,
      "xlimits", "xylimits",
      NULL);

    if (parsing) {
      if (parsing[0])  strcpy(xylimits, str_dup_numeric(parsing[0]));
      if (parsing[1] && !strlen(xylimits))
                       strcpy(xylimits, str_dup_numeric(parsing[1]));
      for (i=0; i<=1; i++) {
        if (parsing[i]) free(parsing[i]);
      }
      free(parsing);
    }
    i = sscanf(xylimits, "%lg %lg %lg %lg",
      &(pTable_xmin),  &(pTable_xmax),
      &(pTable_dxmin), &(pTable_dxmax));
    if (i != 2 && i != 4 && verbose)
      fprintf(stderr, "Source_gen: %s: invalid xylimits '%s' from file %s. extracted %i values\n",
        NAME_CURRENT_COMP, xylimits, xdiv_file, i);

    if (!xwidth) xwidth=pTable_xmax-pTable_xmin;
    if (!focus_xw && !dist) focus_xw=fabs(pTable_dxmax-pTable_dxmin);
  } /* end xdiv file */

  if (ydiv_file && strlen(ydiv_file) && strcmp(ydiv_file,"NULL") && strcmp(ydiv_file,"0")) {
    int i,j;
    if (Table_Read(&pTable_y, ydiv_file, 1) <= 0) /* read 1st block data from file into pTable */
      exit(fprintf(stderr, "Source_gen: %s: can not read YDiv file %s\n", NAME_CURRENT_COMP, ydiv_file));
    pTable_ysum = 0;
    for (i=0; i<pTable_y.rows; i++)
      for (j=0; j<pTable_y.columns; j++)
        pTable_ysum += Table_Index(pTable_y, i,j);

    /* now extract limits */
    char **parsing;
    char xylimits[1024];
    strcpy(xylimits, "");
    parsing = Table_ParseHeader(pTable_y.header,
      "xlimits", "xylimits",
      NULL);

    if (parsing) {
      if (parsing[0])  strcpy(xylimits,str_dup_numeric(parsing[0]));
      if (parsing[1] && !strlen(xylimits))
                       strcpy(xylimits,str_dup_numeric(parsing[1]));
      for (i=0; i<=1; i++) {
        if (parsing[i]) free(parsing[i]);
      }
      free(parsing);
    }
    i = sscanf(xylimits, "%lg %lg %lg %lg",
      &(pTable_ymin),  &(pTable_ymax),
      &(pTable_dymin), &(pTable_dymax));
    if (i != 2 && i != 4 && verbose)
      fprintf(stderr, "Source_gen: %s: invalid xylimits '%s' from file %s. extracted %i values\n",
        NAME_CURRENT_COMP, xylimits, ydiv_file, i);
    if (!yheight)  yheight=pTable_ymax-pTable_ymin;
    if (!focus_yh && !dist) focus_yh=fabs(pTable_dymax-pTable_dymin);
  } /* end ydiv file */

  /* tests for parameter values */
  if (Emin < 0 || Emax < 0 || Lmin < 0 || Lmax < 0 || E0 < 0 || dE < 0 || lambda0 < 0 || dlambda < 0)
  {
    fprintf(stderr,"Source_gen: %s: Error: Negative average\n"
                   "            or range values for wavelength or energy encountered\n",
                   NAME_CURRENT_COMP);
    exit(-1);
  }
  if ((Emin == 0 && Emax > 0) || (dE > 0 && dE >= E0))
  {
    fprintf(stderr,"Source_gen: %s: Error: minimal energy cannot be less or equal zero\n",
      NAME_CURRENT_COMP);
    exit(-1);
  }
  if ((Emax >= Emin) && (Emin > 0))
  { E0 = (Emax+Emin)/2;
    dE = (Emax-Emin)/2;
  }
  if ((E0 > dE) && (dE >= 0))
  {
    Lmin = sqrt(81.81/(E0+dE)); /* Angstroem */
    Lmax = sqrt(81.81/(E0-dE));
  }
  if (Lmax > 0)
  { lambda0 = (Lmax+Lmin)/2;
    dlambda = (Lmax-Lmin)/2;
  }
  if (lambda0 <= 0 || (lambda0 < dlambda) || (dlambda < 0))
  { fprintf(stderr,"Source_gen: %s: Error: Wavelength range %.3f +/- %.3f AA calculated \n",
      NAME_CURRENT_COMP, lambda0, dlambda);
    fprintf(stderr,"- whole wavelength range must be >= 0 \n");
    fprintf(stderr,"- range must be > 0; otherwise intensity gets zero, use other sources in this case \n\n");
    exit(-1);
  }

  radius = fabs(radius); xwidth=fabs(xwidth); yheight=fabs(yheight);  I1=fabs(I1);
  lambda0=fabs(lambda0); dlambda=fabs(dlambda);
  focus_xw = fabs(focus_xw); focus_yh=fabs(focus_yh); dist=fabs(dist);

  if ((!focus_ah && !focus_aw) && (!focus_xw && !focus_yh))
  {
    fprintf(stderr,"Source_gen: %s: Error: No focusing information.\n"
                   "            Specify focus_xw, focus_yh or focus_aw, focus_ah\n",
                   NAME_CURRENT_COMP);
    exit(-1);
  }
  Lmin = lambda0 - dlambda; /* Angstroem */
  Lmax = lambda0 + dlambda;

  /* compute initial weight factor p_in to get [n/s] */
  if ((I1 > 0  && T1 >= 0)
     || (flux_file && strlen(flux_file) && strcmp(flux_file,"NULL") && strcmp(flux_file,"0")))
  { /* the I1,2,3 are usually in [n/s/cm2/st/AA] */
    if (radius)
      source_area = radius*radius*PI*1e4; /* circular cm^2 */
    else
      source_area = yheight*xwidth*1e4; /* square cm^2 */
    p_in  = source_area; /* cm2 */
    p_in *= (Lmax-Lmin); /* AA. 1 bin=AA/n */
    if (flux_file && strlen(flux_file) && strcmp(flux_file,"NULL") && strcmp(flux_file,"0")
      && !flux_file_perAA)  p_in *= pTable.rows/(Lmax-Lmin);
  }
  else
    p_in = 1.0/4/PI; /* Small angle approx. */
  p_in /= mcget_ncount();
  if (!T1 && I1) p_in *= I1;

  if (radius == 0 && yheight == 0 && xwidth == 0)
  {
    fprintf(stderr,"Source_gen: %s: Error: Please specify source geometry (radius, yheight, xwidth)\n",
      NAME_CURRENT_COMP);
    exit(-1);
  }
  if (focus_xw*focus_yh == 0)
  {
    fprintf(stderr,"Source_gen: %s: Error: Please specify source target (focus_xw, focus_yh)\n",
      NAME_CURRENT_COMP);
    exit(-1);
  }
  MPI_MASTER(
  if (verbose)
  {
    printf("Source_gen: component %s ", NAME_CURRENT_COMP);
    if ((yheight == 0) || (xwidth == 0))
      printf("(disk, radius=%g)", radius);
    else
      printf("(square %g x %g)",xwidth,yheight);
    if (dist) printf("\n            focusing distance dist=%g area=%g x %g\n", dist, focus_xw, focus_yh);
    printf("            spectra ");
    printf("%.3f to %.3f AA (%.3f to %.3f meV)", Lmin, Lmax, 81.81/Lmax/Lmax, 81.81/Lmin/Lmin);
    printf("\n");
    if (flux_file && strlen(flux_file) && strcmp(flux_file,"NULL") && strcmp(flux_file,"0"))
    { printf("  File %s for flux distribution used. Flux is dPhi/dlambda in [n/s/AA]. \n", flux_file);
      Table_Info(pTable);
    }
    else if (T1>=0 && I1)
    { if (T1 != 0)
        printf("            T1=%.1f K (%.3f AA)", T1, lambda1);
      if (T2*I2 != 0)
        printf(", T2=%.1f K (%.3f AA)", T2, lambda2);
      if (T3*I3 != 0)
        printf(", T3=%.1f K (%.3f AA)", T3, lambda3);
      if (T1) printf("\n");
      printf("  Flux is dPhi/dlambda in [n/s/cm2].\n");
    }
    else
    { printf("  Flux is Phi in [n/s].\n");
    }
    if (xdiv_file && strlen(xdiv_file) && strcmp(xdiv_file,"NULL") && strcmp(xdiv_file,"0"))
      printf("  File %s x=[%g:%g] [m] xdiv=[%g:%g] [deg] used as horizontal phase space distribution.\n", xdiv_file, pTable_xmin, pTable_xmax, pTable_dxmin, pTable_dxmax);
    if (ydiv_file && strlen(ydiv_file) && strcmp(ydiv_file,"NULL") && strcmp(ydiv_file,"0"))
      printf("  File %s y=[%g:%g] [m] ydiv=[%g:%g] [deg] used as vertical phase space distribution.\n", ydiv_file, pTable_ymin, pTable_ymax, pTable_dymin, pTable_dymax);
  }
  else
    if (verbose == -1)
      printf("Source_gen: component %s inactivated", NAME_CURRENT_COMP);
  );
  #undef flux_file
  #undef xdiv_file
  #undef ydiv_file
  #undef radius
  #undef dist
  #undef focus_xw
  #undef focus_yh
  #undef focus_aw
  #undef focus_ah
  #undef E0
  #undef dE
  #undef lambda0
  #undef dlambda
  #undef I1
  #undef yheight
  #undef xwidth
  #undef verbose
  #undef T1
  #undef flux_file_perAA
  #undef flux_file_log
  #undef Lmin
  #undef Lmax
  #undef Emin
  #undef Emax
  #undef T2
  #undef I2
  #undef T3
  #undef I3
  #undef zdepth
  #undef target_index
  #undef p_in
  #undef lambda1
  #undef lambda2
  #undef lambda3
  #undef pTable
  #undef pTable_x
  #undef pTable_y
  #undef pTable_xmin
  #undef pTable_xmax
  #undef pTable_xsum
  #undef pTable_ymin
  #undef pTable_ymax
  #undef pTable_ysum
  #undef pTable_dxmin
  #undef pTable_dxmax
  #undef pTable_dymin
  #undef pTable_dymax
  return(_comp);
} /* class_Source_gen_init */

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
  SIG_MESSAGE("[_NL1A_NL1B_NL2_NL3_InPile_init] component NL1A_NL1B_NL2_NL3_InPile=Guide() INITIALISE [Guide:0]");

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

_class_Guide_curved *class_Guide_curved_init(_class_Guide_curved *_comp
) {
  #define w1 (_comp->_parameters.w1)
  #define h1 (_comp->_parameters.h1)
  #define l (_comp->_parameters.l)
  #define R0 (_comp->_parameters.R0)
  #define Qc (_comp->_parameters.Qc)
  #define alpha (_comp->_parameters.alpha)
  #define m (_comp->_parameters.m)
  #define W (_comp->_parameters.W)
  #define curvature (_comp->_parameters.curvature)
  SIG_MESSAGE("[_NL1B_Curved_Guide_1_init] component NL1B_Curved_Guide_1=Guide_curved() INITIALISE [Guide_curved:0]");

  if (mcgravitation)
    fprintf (stderr,
             "WARNING: Guide_curved: %s: "
             "This component produces wrong results with gravitation !\n"
             "Use Guide_gravity.\n",
             NAME_CURRENT_COMP);
  #undef w1
  #undef h1
  #undef l
  #undef R0
  #undef Qc
  #undef alpha
  #undef m
  #undef W
  #undef curvature
  return(_comp);
} /* class_Guide_curved_init */

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
  SIG_MESSAGE("[_Before_selec_init] component Before_selec=PSD_monitor() INITIALISE [PSD_monitor:0]");

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

_class_Selector *class_Selector_init(_class_Selector *_comp
) {
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define length (_comp->_parameters.length)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define nslit (_comp->_parameters.nslit)
  #define d (_comp->_parameters.d)
  #define radius (_comp->_parameters.radius)
  #define alpha (_comp->_parameters.alpha)
  #define nu (_comp->_parameters.nu)
  SIG_MESSAGE("[_SELEC_init] component SELEC=Selector() INITIALISE [Selector:0]");

  if (xwidth > 0) {
    xmax = xwidth / 2;
    xmin = -xmax;
  }
  if (yheight > 0) {
    ymax = yheight / 2;
    ymin = -ymax;
  }
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef length
  #undef xwidth
  #undef yheight
  #undef nslit
  #undef d
  #undef radius
  #undef alpha
  #undef nu
  return(_comp);
} /* class_Selector_init */

_class_Guide_tapering *class_Guide_tapering_init(_class_Guide_tapering *_comp
) {
  #define option (_comp->_parameters.option)
  #define w1 (_comp->_parameters.w1)
  #define h1 (_comp->_parameters.h1)
  #define l (_comp->_parameters.l)
  #define linw (_comp->_parameters.linw)
  #define loutw (_comp->_parameters.loutw)
  #define linh (_comp->_parameters.linh)
  #define louth (_comp->_parameters.louth)
  #define R0 (_comp->_parameters.R0)
  #define Qcx (_comp->_parameters.Qcx)
  #define Qcy (_comp->_parameters.Qcy)
  #define alphax (_comp->_parameters.alphax)
  #define alphay (_comp->_parameters.alphay)
  #define W (_comp->_parameters.W)
  #define mx (_comp->_parameters.mx)
  #define my (_comp->_parameters.my)
  #define segno (_comp->_parameters.segno)
  #define curvature (_comp->_parameters.curvature)
  #define curvature_v (_comp->_parameters.curvature_v)
  #define w1c (_comp->_parameters.w1c)
  #define w2c (_comp->_parameters.w2c)
  #define ww (_comp->_parameters.ww)
  #define hh (_comp->_parameters.hh)
  #define whalf (_comp->_parameters.whalf)
  #define hhalf (_comp->_parameters.hhalf)
  #define lwhalf (_comp->_parameters.lwhalf)
  #define lhhalf (_comp->_parameters.lhhalf)
  #define h1_in (_comp->_parameters.h1_in)
  #define h2_out (_comp->_parameters.h2_out)
  #define w1_in (_comp->_parameters.w1_in)
  #define w2_out (_comp->_parameters.w2_out)
  #define l_seg (_comp->_parameters.l_seg)
  #define h12 (_comp->_parameters.h12)
  #define h2 (_comp->_parameters.h2)
  #define w12 (_comp->_parameters.w12)
  #define w2 (_comp->_parameters.w2)
  #define a_ell_q (_comp->_parameters.a_ell_q)
  #define b_ell_q (_comp->_parameters.b_ell_q)
  #define lbw (_comp->_parameters.lbw)
  #define lbh (_comp->_parameters.lbh)
  #define mxi (_comp->_parameters.mxi)
  #define u1 (_comp->_parameters.u1)
  #define u2 (_comp->_parameters.u2)
  #define div1 (_comp->_parameters.div1)
  #define p2_para (_comp->_parameters.p2_para)
  #define test (_comp->_parameters.test)
  #define Div1 (_comp->_parameters.Div1)
  #define seg (_comp->_parameters.seg)
  #define fu (_comp->_parameters.fu)
  #define pos (_comp->_parameters.pos)
  #define file_name (_comp->_parameters.file_name)
  #define ep (_comp->_parameters.ep)
  #define num (_comp->_parameters.num)
  #define rotation_h (_comp->_parameters.rotation_h)
  #define rotation_v (_comp->_parameters.rotation_v)
  SIG_MESSAGE("[_elliptical_piece_init] component elliptical_piece=Guide_tapering() INITIALISE [Guide_tapering:0]");

  int i, ii;

  rotation_h = 0;
  rotation_v = 0;

  // dynamic memory allocation is good
  w1c = (double*)malloc (sizeof (double) * segno);
  w2c = (double*)malloc (sizeof (double) * segno);
  ww = (double*)malloc (sizeof (double) * segno);
  hh = (double*)malloc (sizeof (double) * segno);
  whalf = (double*)malloc (sizeof (double) * segno);
  hhalf = (double*)malloc (sizeof (double) * segno);
  lwhalf = (double*)malloc (sizeof (double) * segno);
  lhhalf = (double*)malloc (sizeof (double) * segno);
  h1_in = (double*)malloc (sizeof (double) * (segno + 1));
  h2_out = (double*)malloc (sizeof (double) * (segno + 1));
  w1_in = (double*)malloc (sizeof (double) * (segno + 1));
  w2_out = (double*)malloc (sizeof (double) * (segno + 1));

  struct para {
    char st[128];
  } segment[800];

  if (W <= 0) {
    fprintf (stderr, "Component: %s (Guide_tapering) W must \n", NAME_CURRENT_COMP);
    fprintf (stderr, "           be positive\n");
    exit (-1);
  }
  if (l <= 0) {
    fprintf (stderr, "Component: %s (Guide_tapering) real guide length \n", NAME_CURRENT_COMP);
    fprintf (stderr, "           is <= ZERO ! \n");
    exit (-1);
  }
  if (mcgravitation)
    fprintf (stderr,
             "WARNING: Guide_tapering: %s: "
             "This component produces wrong results with gravitation !\n"
             "Use Guide_gravity.\n",
             NAME_CURRENT_COMP);
  seg = segno;
  l_seg = l / (seg);
  h12 = h1 / 2.0;
  if (option != NULL) {
    fu = (char*)malloc (sizeof (char) * (strlen (option) + 1));
    strcpy (fu, option);
  } else {
    exit (-1);
  }
  /* handle guide geometry ================================================== */
  if (!strcmp (fu, "elliptical")) {
    /* calculate parameter b of elliptical equestion - vertical mirrors */
    /* (l+linh+louth) -> distance between focal points */
    /*  printf("A1 \n"); */
    lbh = l + linh + louth;
    if (linh == 0 && louth == 0) {
      /* plane mirrors (vertical) */
      b_ell_q = 0;
      h2 = h1;
    } else {
      /* elliptical mirrors */
      u1 = sqrt ((linh * linh) + (h12 * h12));
      u2 = sqrt ((h12 * h12) + ((l + louth) * (l + louth)));
      a_ell_q = ((u1 + u2) / 2.0) * ((u1 + u2) / 2.0);
      b_ell_q = a_ell_q - ((lbh / 2.0) * (lbh / 2.0));
      /* calculate heigth of guide exit (h2) */
      div1 = ((lbh / 2.0 - louth) * (lbh / 2.0 - louth)) / a_ell_q;
      h2 = sqrt (b_ell_q * (1.0 - div1));
      h2 = h2 * 2.0;
    }
  } else if (!strcmp (fu, "parabolical")) {
    if ((linh > 0) && (louth > 0)) {
      fprintf (stderr, "Component: %s (Guide_tapering) Two focal\n", NAME_CURRENT_COMP);
      fprintf (stderr, "            points lout and linh are not allowed! \n");
      free (fu);
      exit (-1);
    }
    if (louth == 0 && linh == 0) {
      /* plane mirrors (vertical) */
      h2 = h1;
    } else {
      /* parabolical mirrors */
      if (linh == 0) {
        Div1 = ((2.0 * louth + 2.0 * l) * (2.0 * louth + 2.0 * l)) / 4.0;
        p2_para = ((sqrt (Div1 + (h12 * h12))) - (louth + l)) * 2.0;
        /* calculate heigth of guide exit (h2) */
        h2 = sqrt (p2_para * (louth + p2_para / 4.0));
        h2 = h2 * 2.0;
      } else {
        /* anti-trompete */
        Div1 = ((2.0 * linh) * (2.0 * linh)) / 4.0;
        p2_para = ((sqrt (Div1 + (h12 * h12))) - linh) * 2.0;
        /* calculate heigth of guide exit (h2) */
        h2 = sqrt (p2_para * (l + linh + p2_para / 4.0));
        h2 = h2 * 2.0;
      }
    }
  } else if (!strncmp (fu, "file", 4)) {
    pos = strtok (fu, "=");
    while ((pos = strtok (0, "="))) {
      strcpy (file_name, pos);
    }
    if ((num = fopen (file_name, "r")) == NULL) {
      fprintf (stderr, "Component: %s (Guide_tapering)\n", NAME_CURRENT_COMP);
      fprintf (stderr, "           File %s not found! \n", file_name);
      free (fu);
      exit (-1);
    } else {
      ii = 0;
      /* Use ret==NULL as termination criterion on while, otherwise
         the reading may on some systems continue until "800" segments */
      char dymmy = 1;
      char* ret = &dymmy;
      while (!feof (num) && ret) {
        ret = fgets (segment[ii].st, 128, num);
        if (ii > 799) {
          fprintf (stderr, "%s: Number of segments is limited to 800 !! \n", NAME_CURRENT_COMP);
          free (fu);
          exit (-1);
        }
        ii++;
      }
      fclose (num);
      ii--;
    }
    seg = ii - 3;
    l_seg = l / seg;
    for (i = 3; i < ii; i++) {
      if (strlen (segment[i].st) < 4) {
        fprintf (stderr, "Component: %s (Guide_tapering)\n", NAME_CURRENT_COMP);
        fprintf (stderr, "           Data Format Error! \n");
        free (fu);
        exit (-1);
      }
      h1_in[i - 3] = strtod (strtok (segment[i].st, " "), &ep);
      h2_out[i - 3] = strtod (strtok (0, " "), &ep);
      w1_in[i - 3] = strtod (strtok (0, " "), &ep);
      w2_out[i - 3] = strtod (strtok (0, " "), &ep);
    }
    h1 = h1_in[0];
    h2 = h2_out[seg - 1];
    w1 = w1_in[0];
    w2 = w2_out[seg - 1];
    for (i = 0; i < seg; i++) {
      fprintf (stderr, "%d: %lf %lf %lf %lf \n", i, h1_in[i], h2_out[i], w1_in[i], w2_out[i]);
    }
  } else if (!strcmp (fu, "straight")) {
    for (i = 0; i < seg; i++) {
      h1_in[i] = h2_out[i] = h2 = h1;
      w1_in[i] = w2_out[i] = w2 = w1;
    }
  } else {
    fprintf (stderr, "Component: %s (Guide_tapering)\n", NAME_CURRENT_COMP);
    fprintf (stderr, "           Unknown KEYWORD: %s \n", fu);
    free (fu);
    exit (-1);
  }
  fprintf (stderr, "Component: %s (Guide_tapering)\n", NAME_CURRENT_COMP);
  fprintf (stderr, "           Height at the guide exit (h2): %lf \n", h2);
  if (h2 <= 0) {
    fprintf (stderr, "Component: %s (Guide_tapering)\n", NAME_CURRENT_COMP);
    fprintf (stderr, "           Height at the guide exit (h2) was calculated\n");
    fprintf (stderr, "           <=0; Please change the parameter h1 and/or\n");
    fprintf (stderr, "           linh and/or louth! \n");
    free (fu);
    exit (-1);
  }
  if (!strcmp (fu, "elliptical")) {
    h1_in[0] = h1;
    for (i = 1; i < seg; i++) {
      if (b_ell_q == 0) {
        h1_in[i] = h1;
      } else {
        mxi = (((lbh / 2.0) - linh) - (l_seg * i));
        h1_in[i] = (sqrt ((1.0 - ((mxi * mxi) / a_ell_q)) * b_ell_q)) * 2.0;
      }
      h2_out[i - 1] = h1_in[i];
    }
    h2_out[seg - 1] = h2;
  } else if (!strcmp (fu, "parabolical")) {
    h1_in[0] = h1;
    ii = seg - 1;
    if (louth == 0 && linh == 0) {
      for (i = 1; i < (seg + 1); i++) {
        h1_in[i] = h1;
        ii = ii - 1;
        h2_out[i - 1] = h1_in[i];
      }
    } else {
      if ((linh == 0) && (louth > 0)) {
        for (i = 1; i < (seg + 1); i++) {
          h1_in[i] = (sqrt ((p2_para / 4.0 + louth + (l_seg * ii)) * p2_para)) * 2.0;
          ii = ii - 1;
          h2_out[i - 1] = h1_in[i];
        }
      } else {
        for (i = 1; i < (seg + 1); i++) {
          h1_in[i] = (sqrt ((p2_para / 4.0 + linh + (l_seg * i)) * p2_para)) * 2.0;
          h2_out[i - 1] = h1_in[i];
        }
      }
    }
  }
  /* compute each value for horizontal mirrors */
  w12 = w1 / 2.0;
  if (!strcmp (fu, "elliptical")) {
    /* calculate lbw the distance between focal points of horizontal mirrors */
    lbw = l + linw + loutw;
    /* calculate parameter b of elliptical equestion - horizontal mirrors */
    if (linw == 0 && loutw == 0) {
      /* plane mirrors (horizontal) */
      b_ell_q = 0;
      w2 = w1;
    } else {
      /* elliptical mirrors */
      u1 = sqrt ((linw * linw) + (w12 * w12));
      u2 = sqrt ((w12 * w12) + ((l + loutw) * (l + loutw)));
      a_ell_q = ((u1 + u2) / 2.0) * ((u1 + u2) / 2.0);
      b_ell_q = a_ell_q - ((lbw / 2.0) * (lbw / 2.0));
      /* calculate weigth of guide exit (w2) */
      div1 = ((lbw / 2.0 - loutw) * (lbw / 2.0 - loutw)) / a_ell_q;
      w2 = sqrt (b_ell_q * (1.0 - div1));
      w2 = w2 * 2.0;
    }
  } else if (!strcmp (fu, "parabolical")) {
    if ((linw > 0) && (loutw > 0)) {
      fprintf (stderr, "Component: %s (Guide_tapering) Two focal\n", NAME_CURRENT_COMP);
      fprintf (stderr, "           points linw and loutw are not allowed! \n");
      free (fu);
      exit (-1);
    }
    if (loutw == 0 && linw == 0) {
      /* plane mirrors (horizontal) */
      w2 = w1;
    } else {
      if (linw == 0) {
        /* parabolical mirrors */
        Div1 = ((2.0 * loutw + 2.0 * l) * (2.0 * loutw + 2.0 * l)) / 4.0;
        p2_para = ((sqrt (Div1 + (w12 * w12))) - (loutw + l)) * 2.0;
        /* calculate weigth of guide exit (w2) */
        w2 = sqrt (p2_para * (loutw + p2_para / 4.0));
        w2 = w2 * 2.0;
      } else {
        /* anti-trompete */
        Div1 = ((2.0 * linw) * (2.0 * linw)) / 4.0;
        p2_para = ((sqrt (Div1 + (w12 * w12))) - linw) * 2.0;
        /* calculate heigth of guide exit (w2) */
        w2 = sqrt (p2_para * (l + linw + p2_para / 4.0));
        w2 = w2 * 2.0;
      }
    }
  }
  fprintf (stderr, "Component: %s (Guide_tapering)\n", NAME_CURRENT_COMP);
  fprintf (stderr, "           Width at the guide exit (w2): %lf \n", w2);
  if (w2 <= 0) {
    fprintf (stderr, "Component: %s (Guide_tapering)\n", NAME_CURRENT_COMP);
    fprintf (stderr, "           Width at the guide exit (w2) was calculated\n");
    fprintf (stderr, "           <=0; Please change the parameter w1 and/or\n");
    fprintf (stderr, "           l! \n");
    free (fu);
    exit (-1);
  }
  if (!strcmp (fu, "elliptical")) {
    w1_in[0] = w1;
    for (i = 1; i < seg; i++) {
      if (b_ell_q == 0) {
        w1_in[i] = w1;
      } else {
        mxi = (((lbw / 2.0) - linw) - (l_seg * i));
        w1_in[i] = (sqrt ((1.0 - ((mxi * mxi) / a_ell_q)) * b_ell_q)) * 2.0;
      }
      w2_out[i - 1] = w1_in[i];
    }
    w2_out[seg - 1] = w2;
  } else if (!strcmp (fu, "parabolical")) {
    w1_in[0] = w1;
    ii = seg - 1;
    if (loutw == 0 && linw == 0) {
      for (i = 1; i < (seg + 1); i++) {
        w1_in[i] = w1;
        ii = ii - 1;
        w2_out[i - 1] = w1_in[i];
      }
    } else {
      if ((linw == 0) && (loutw > 0)) {
        for (i = 1; i < (seg + 1); i++) {
          w1_in[i] = (sqrt ((p2_para / 4 + loutw + (l_seg * ii)) * p2_para)) * 2;
          ii = ii - 1;
          w2_out[i - 1] = w1_in[i];
        }
      } else {
        for (i = 1; i < (seg + 1); i++) {
          w1_in[i] = (sqrt ((p2_para / 4 + linw + (l_seg * i)) * p2_para)) * 2;
          w2_out[i - 1] = w1_in[i];
        }
      }
    }
  }
  free (fu);
  for (i = 0; i < seg; i++) {
    w1c[i] = w1_in[i];
    w2c[i] = w2_out[i];
    ww[i] = .5 * (w2c[i] - w1c[i]);
    hh[i] = .5 * (h2_out[i] - h1_in[i]);
    whalf[i] = .5 * w1c[i];
    hhalf[i] = .5 * h1_in[i];
    lwhalf[i] = l_seg * whalf[i];
    lhhalf[i] = l_seg * hhalf[i];
  }
  /* guide curvature: rotation angle [rad] between each guide segment */
  if (curvature && l && segno)
    rotation_h = l / curvature / segno;
  if (curvature_v && l && segno)
    rotation_v = l / curvature_v / segno;
  #undef option
  #undef w1
  #undef h1
  #undef l
  #undef linw
  #undef loutw
  #undef linh
  #undef louth
  #undef R0
  #undef Qcx
  #undef Qcy
  #undef alphax
  #undef alphay
  #undef W
  #undef mx
  #undef my
  #undef segno
  #undef curvature
  #undef curvature_v
  #undef w1c
  #undef w2c
  #undef ww
  #undef hh
  #undef whalf
  #undef hhalf
  #undef lwhalf
  #undef lhhalf
  #undef h1_in
  #undef h2_out
  #undef w1_in
  #undef w2_out
  #undef l_seg
  #undef h12
  #undef h2
  #undef w12
  #undef w2
  #undef a_ell_q
  #undef b_ell_q
  #undef lbw
  #undef lbh
  #undef mxi
  #undef u1
  #undef u2
  #undef div1
  #undef p2_para
  #undef test
  #undef Div1
  #undef seg
  #undef fu
  #undef pos
  #undef file_name
  #undef ep
  #undef num
  #undef rotation_h
  #undef rotation_v
  return(_comp);
} /* class_Guide_tapering_init */

_class_Slit *class_Slit_init(_class_Slit *_comp
) {
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define radius (_comp->_parameters.radius)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define isradial (_comp->_parameters.isradial)
  SIG_MESSAGE("[_Virtual_source_init] component Virtual_source=Slit() INITIALISE [Slit:0]");

  if (is_unset (radius)) {
    isradial = 0;
    if (all_set (3, xwidth, xmin, xmax)) {
      slit_error_if (xwidth != xmax - xmin, "specifying xwidth, xmin and xmax requires consistent parameters", NAME_CURRENT_COMP);
    } else {
      slit_error_if (is_unset (xwidth) && any_unset (2, xmin, xmax), "specify either xwidth or xmin & xmax", NAME_CURRENT_COMP);
    }
    if (all_set (3, yheight, ymin, ymax)) {
      slit_error_if (yheight != ymax - ymin, "specifying yheight, ymin and ymax requires consistent parameters", NAME_CURRENT_COMP);
    } else {
      slit_error_if (is_unset (yheight) && any_unset (2, ymin, ymax), "specify either yheight or ymin & ymax", NAME_CURRENT_COMP);
    }
    if (is_unset (xmin)) { // xmax also unset but xwidth *is* set
      xmax = xwidth / 2;
      xmin = -xmax;
    }
    if (is_unset (ymin)) { // ymax also unset but yheight *is* set
      ymax = yheight / 2;
      ymin = -ymax;
    }
    slit_warning_if (xmin == xmax || ymin == ymax, "Running with CLOSED rectangular slit - is this intentional?", NAME_CURRENT_COMP);
  } else {
    isradial = 1;
    slit_error_if (any_set (6, xwidth, xmin, xmax, yheight, ymin, ymax), "specify radius OR width and height parameters", NAME_CURRENT_COMP);
    slit_warning_if (radius == 0., "Running with CLOSED radial slit - is this intentional?", NAME_CURRENT_COMP);
  }
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef radius
  #undef xwidth
  #undef yheight
  #undef isradial
  return(_comp);
} /* class_Slit_init */

_class_Guide_channeled *class_Guide_channeled_init(_class_Guide_channeled *_comp
) {
  #define w1 (_comp->_parameters.w1)
  #define h1 (_comp->_parameters.h1)
  #define w2 (_comp->_parameters.w2)
  #define h2 (_comp->_parameters.h2)
  #define l (_comp->_parameters.l)
  #define R0 (_comp->_parameters.R0)
  #define Qc (_comp->_parameters.Qc)
  #define alpha (_comp->_parameters.alpha)
  #define m (_comp->_parameters.m)
  #define nslit (_comp->_parameters.nslit)
  #define d (_comp->_parameters.d)
  #define Qcx (_comp->_parameters.Qcx)
  #define Qcy (_comp->_parameters.Qcy)
  #define alphax (_comp->_parameters.alphax)
  #define alphay (_comp->_parameters.alphay)
  #define W (_comp->_parameters.W)
  #define mx (_comp->_parameters.mx)
  #define my (_comp->_parameters.my)
  #define nu (_comp->_parameters.nu)
  #define phase (_comp->_parameters.phase)
  #define w1c (_comp->_parameters.w1c)
  #define w2c (_comp->_parameters.w2c)
  #define ww (_comp->_parameters.ww)
  #define hh (_comp->_parameters.hh)
  #define whalf (_comp->_parameters.whalf)
  #define hhalf (_comp->_parameters.hhalf)
  #define lwhalf (_comp->_parameters.lwhalf)
  #define lhhalf (_comp->_parameters.lhhalf)
  SIG_MESSAGE("[_NL1B_Vertical_Guide_init] component NL1B_Vertical_Guide=Guide_channeled() INITIALISE [Guide_channeled:0]");

  if (!w2)
    w2 = w1;
  if (!h2)
    h2 = h1;
  if (nslit <= 0 || W <= 0) {
    fprintf (stderr, "Guide_channeled: %s: nslit and W must be positive\n", NAME_CURRENT_COMP);
    exit (-1);
  }
  w1c = (w1 + d) / (double)nslit;
  w2c = (w2 + d) / (double)nslit;
  ww = .5 * (w2c - w1c);
  hh = .5 * (h2 - h1);
  whalf = .5 * (w1c - d);
  hhalf = .5 * h1;
  lwhalf = l * whalf;
  lhhalf = l * hhalf;

  if (m) {
    mx = my = m;
  }
  if (Qc) {
    Qcx = Qcy = Qc;
  }
  if (alpha) {
    alphax = alphay = alpha;
  }

  if ((nslit > 1) && (w1 != w2)) {
    fprintf (stderr,
             "WARNING: Guide_channeled: %s:"
             "This component does not work with multichannel focusing guide\n"
             "Use Guide_gravity for that.\n",
             NAME_CURRENT_COMP);
    exit (-1);
  }

  if (d * nslit > w1)
    exit (fprintf (stderr, "Guide_channeled: %s: absorbing walls fill input window. No space left for transmission (d*nslit > w1).\n", NAME_CURRENT_COMP));

  if (mcgravitation)
    fprintf (stderr,
             "WARNING: Guide_channeled: %s: "
             "This component produces wrong results with gravitation !\n"
             "Use Guide_gravity.\n",
             NAME_CURRENT_COMP);
  if (nu != 0 || phase != 0) {
    if (w1 != w2 || h1 != h2)
      exit (fprintf (stderr, "Guide_channeled: %s: rotating slit pack must be straight (w1=w2 and h1=h2).\n", NAME_CURRENT_COMP));
    printf ("Guide_channeled: %s: Fermi Chopper mode: frequency=%g [Hz] phase=%g [deg]\n", NAME_CURRENT_COMP, nu, phase);
  }
  #undef w1
  #undef h1
  #undef w2
  #undef h2
  #undef l
  #undef R0
  #undef Qc
  #undef alpha
  #undef m
  #undef nslit
  #undef d
  #undef Qcx
  #undef Qcy
  #undef alphax
  #undef alphay
  #undef W
  #undef mx
  #undef my
  #undef nu
  #undef phase
  #undef w1c
  #undef w2c
  #undef ww
  #undef hh
  #undef whalf
  #undef hhalf
  #undef lwhalf
  #undef lhhalf
  return(_comp);
} /* class_Guide_channeled_init */

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
  SIG_MESSAGE("[_energy_endguide_init] component energy_endguide=E_monitor() INITIALISE [E_monitor:0]");

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

_class_Monochromator_curved *class_Monochromator_curved_init(_class_Monochromator_curved *_comp
) {
  #define reflect (_comp->_parameters.reflect)
  #define transmit (_comp->_parameters.transmit)
  #define zwidth (_comp->_parameters.zwidth)
  #define yheight (_comp->_parameters.yheight)
  #define gap (_comp->_parameters.gap)
  #define NH (_comp->_parameters.NH)
  #define NV (_comp->_parameters.NV)
  #define mosaich (_comp->_parameters.mosaich)
  #define mosaicv (_comp->_parameters.mosaicv)
  #define r0 (_comp->_parameters.r0)
  #define t0 (_comp->_parameters.t0)
  #define Q (_comp->_parameters.Q)
  #define RV (_comp->_parameters.RV)
  #define RH (_comp->_parameters.RH)
  #define DM (_comp->_parameters.DM)
  #define mosaic (_comp->_parameters.mosaic)
  #define width (_comp->_parameters.width)
  #define height (_comp->_parameters.height)
  #define verbose (_comp->_parameters.verbose)
  #define order (_comp->_parameters.order)
  #define mos_rms_y (_comp->_parameters.mos_rms_y)
  #define mos_rms_z (_comp->_parameters.mos_rms_z)
  #define mos_rms_max (_comp->_parameters.mos_rms_max)
  #define mono_Q (_comp->_parameters.mono_Q)
  #define SlabWidth (_comp->_parameters.SlabWidth)
  #define SlabHeight (_comp->_parameters.SlabHeight)
  #define rTable (_comp->_parameters.rTable)
  #define tTable (_comp->_parameters.tTable)
  #define rTableFlag (_comp->_parameters.rTableFlag)
  #define tTableFlag (_comp->_parameters.tTableFlag)
  #define tiltH (_comp->_parameters.tiltH)
  #define tiltV (_comp->_parameters.tiltV)
  #define ncol_var (_comp->_parameters.ncol_var)
  #define nrow_var (_comp->_parameters.nrow_var)
  SIG_MESSAGE("[_Monochromator_init] component Monochromator=Monochromator_curved() INITIALISE [Monochromator_curved:0]");

  int i;

  if (mosaic != 0) {
    mos_rms_y = MIN2RAD * mosaic / sqrt (8 * log (2));
    mos_rms_z = mos_rms_y;
  } else {
    mos_rms_y = MIN2RAD * mosaich / sqrt (8 * log (2));
    mos_rms_z = MIN2RAD * mosaicv / sqrt (8 * log (2));
  }
  mos_rms_max = mos_rms_y > mos_rms_z ? mos_rms_y : mos_rms_z;

  mono_Q = Q;
  if (DM != 0)
    mono_Q = 2 * PI / DM;

  if (mono_Q <= 0) {
    fprintf (stderr, "Monochromator_curved: %s: Error scattering vector Q = 0\n", NAME_CURRENT_COMP);
    exit (-1);
  }
  if (r0 < 0) {
    fprintf (stderr, "Monochromator_curved: %s: Error reflectivity r0 is negative\n", NAME_CURRENT_COMP);
    exit (-1);
  }
  if (r0 == 0) {
    fprintf (stderr, "Monochromator_curved: %s: Reflectivity r0 is null. Ignoring component.\n", NAME_CURRENT_COMP);
  }
  if (NH * NV == 0) {
    fprintf (stderr, "Monochromator_curved: %s: no slabs ??? (NH or NV=0)\n", NAME_CURRENT_COMP);
    exit (-1);
  }

  if (verbose && r0) {
    printf ("Monochromator_curved: component %s Q=%.3g Angs-1 (DM=%.4g Angs)\n", NAME_CURRENT_COMP, mono_Q, 2 * PI / mono_Q);
    if (NH * NV == 1)
      printf ("            flat.\n");
    else {
      if (NH > 1) {
        printf ("            horizontal: %i blades", (int)NH);
        if (RH != 0)
          printf (" focusing with RH=%.3g [m]", RH);
        printf ("\n");
      }
      if (NV > 1) {
        printf ("            vertical:   %i blades", (int)NV);
        if (RV != 0)
          printf (" focusing with RV=%.3g [m]", RV);
        printf ("\n");
      }
    }
  }

  if (reflect != NULL && r0 && strlen (reflect) && strcmp (reflect, "NULL") && strcmp (reflect, "0")) {
    if (verbose)
      fprintf (stdout, "Monochromator_curved: %s: Reflectivity data (k, R) from %s\n", NAME_CURRENT_COMP, reflect);
    Table_Read (&rTable, reflect, 1); /* read 1st block data from file into rTable */
    Table_Rebin (&rTable);            /* rebin as evenly, increasing array */
    if (rTable.rows < 2)
      Table_Free (&rTable);
    if (verbose)
      Table_Info (rTable);
    rTableFlag = 1;
  } else {
    rTableFlag = 0;
  }
  if (transmit != NULL && strlen (transmit) && strcmp (transmit, "NULL") && strcmp (transmit, "0")) {
    if (verbose)
      fprintf (stdout, "Monochromator_curved: %s: Transmission data (k, T) from %s\n", NAME_CURRENT_COMP, transmit);
    Table_Read (&tTable, transmit, 1); /* read 1st block data from file into rTable */
    Table_Rebin (&tTable);             /* rebin as evenly, increasing array */
    if (tTable.rows < 2)
      Table_Free (&tTable);
    if (verbose)
      Table_Info (tTable);
    tTableFlag = 1;
  } else {
    tTableFlag = 0;
  }

  if (width == 0)
    SlabWidth = zwidth;
  else
    SlabWidth = (width + gap) / NH - gap;
  if (height == 0)
    SlabHeight = yheight;
  else
    SlabHeight = (height + gap) / NV - gap;

  tiltH = calloc ((int)2 * (NH + 1), sizeof (double));
  tiltV = calloc ((int)2 * (NV + 1), sizeof (double));

  if (!tiltH)
    printf ("Monochromator_curved: %s: Warning: not enough memory to allocate tilts (NH=%i).\n", NAME_CURRENT_COMP, NH);
  else if (RH) { /* pre-compute tilts */
    for (i = 0; i <= NH; i++) {
      tiltH[i] = asin ((i - (NH + 1) / 2.0) * (SlabWidth + gap) / RH);
    }
  }
  if (!tiltV)
    printf ("Monochromator_curved: %s: Warning: not enough memory to allocate tilts (NV=%i).\n", NAME_CURRENT_COMP, NV);
  else if (RV) {
    for (i = 0; i <= NV; i++) {
      tiltV[i] = -asin ((i - (NV + 1) / 2.0) * (SlabHeight + gap) / RV);
    }
  }
  sprintf (ncol_var, "ncol_%ld", _comp->_index);
  sprintf (nrow_var, "nrow_%ld", _comp->_index);
  #undef reflect
  #undef transmit
  #undef zwidth
  #undef yheight
  #undef gap
  #undef NH
  #undef NV
  #undef mosaich
  #undef mosaicv
  #undef r0
  #undef t0
  #undef Q
  #undef RV
  #undef RH
  #undef DM
  #undef mosaic
  #undef width
  #undef height
  #undef verbose
  #undef order
  #undef mos_rms_y
  #undef mos_rms_z
  #undef mos_rms_max
  #undef mono_Q
  #undef SlabWidth
  #undef SlabHeight
  #undef rTable
  #undef tTable
  #undef rTableFlag
  #undef tTableFlag
  #undef tiltH
  #undef tiltV
  #undef ncol_var
  #undef nrow_var
  return(_comp);
} /* class_Monochromator_curved_init */

_class_Divergence_monitor *class_Divergence_monitor_init(_class_Divergence_monitor *_comp
) {
  #define nh (_comp->_parameters.nh)
  #define nv (_comp->_parameters.nv)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define maxdiv_h (_comp->_parameters.maxdiv_h)
  #define maxdiv_v (_comp->_parameters.maxdiv_v)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nx (_comp->_parameters.nx)
  #define ny (_comp->_parameters.ny)
  #define nz (_comp->_parameters.nz)
  #define Div_N (_comp->_parameters.Div_N)
  #define Div_p (_comp->_parameters.Div_p)
  #define Div_p2 (_comp->_parameters.Div_p2)
  SIG_MESSAGE("[_div_mono_init] component div_mono=Divergence_monitor() INITIALISE [Divergence_monitor:0]");

  if (xwidth > 0) {
    xmax = xwidth / 2;
    xmin = -xmax;
  }
  if (yheight > 0) {
    ymax = yheight / 2;
    ymin = -ymax;
  }

  if ((xmin >= xmax) || (ymin >= ymax)) {
    printf ("Divergence_monitor: %s: Null detection area !\n"
            "ERROR               (xwidth,yheight,xmin,xmax,ymin,ymax). Exiting",
            NAME_CURRENT_COMP);
    exit (0);
  }

  Div_N = create_darr2d (nh, nv);
  Div_p = create_darr2d (nh, nv);
  Div_p2 = create_darr2d (nh, nv);

  NORM (nx, ny, nz);

  // Use instance name for monitor output if no input was given
  if (!strcmp (filename, "\0"))
    sprintf (filename, "%s", NAME_CURRENT_COMP);
  #undef nh
  #undef nv
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef nowritefile
  #undef xwidth
  #undef yheight
  #undef maxdiv_h
  #undef maxdiv_v
  #undef restore_neutron
  #undef nx
  #undef ny
  #undef nz
  #undef Div_N
  #undef Div_p
  #undef Div_p2
  return(_comp);
} /* class_Divergence_monitor_init */

_class_Div1D_monitor *class_Div1D_monitor_init(_class_Div1D_monitor *_comp
) {
  #define ndiv (_comp->_parameters.ndiv)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define maxdiv (_comp->_parameters.maxdiv)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define vertical (_comp->_parameters.vertical)
  #define Div_N (_comp->_parameters.Div_N)
  #define Div_p (_comp->_parameters.Div_p)
  #define Div_p2 (_comp->_parameters.Div_p2)
  SIG_MESSAGE("[_div_mono_H_init] component div_mono_H=Div1D_monitor() INITIALISE [Div1D_monitor:0]");

  int i;

  if (xwidth > 0) {
    xmax = xwidth / 2;
    xmin = -xmax;
  }
  if (yheight > 0) {
    ymax = yheight / 2;
    ymin = -ymax;
  }

  if ((xmin >= xmax) || (ymin >= ymax)) {
    printf ("Error(%s): Div1D_monitor: Null detection area !\n"
            "(xwidth,yheight,xmin,xmax,ymin,ymax). Exiting",
            NAME_CURRENT_COMP);
    exit (-1);
  }

  Div_N = create_darr1d (ndiv);
  Div_p = create_darr1d (ndiv);
  Div_p2 = create_darr1d (ndiv);
  for (i = 0; i < ndiv; i++) {
    Div_N[i] = 0;
    Div_p[i] = 0;
    Div_p2[i] = 0;
  }

  // Use instance name for monitor output if no input was given
  if (!strcmp (filename, "\0"))
    sprintf (filename, "%s", NAME_CURRENT_COMP);
  #undef ndiv
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef maxdiv
  #undef restore_neutron
  #undef nowritefile
  #undef vertical
  #undef Div_N
  #undef Div_p
  #undef Div_p2
  return(_comp);
} /* class_Div1D_monitor_init */

_class_PSDlin_monitor *class_PSDlin_monitor_init(_class_PSDlin_monitor *_comp
) {
  #define nbins (_comp->_parameters.nbins)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define vertical (_comp->_parameters.vertical)
  #define PSDlin_N (_comp->_parameters.PSDlin_N)
  #define PSDlin_p (_comp->_parameters.PSDlin_p)
  #define PSDlin_p2 (_comp->_parameters.PSDlin_p2)
  SIG_MESSAGE("[_dpsd1_init] component dpsd1=PSDlin_monitor() INITIALISE [PSDlin_monitor:0]");

  if (xwidth > 0) {
    xmax = xwidth / 2;
    xmin = -xmax;
  }
  if (yheight > 0) {
    ymax = yheight / 2;
    ymin = -ymax;
  }

  if ((xmin >= xmax) || (ymin >= ymax)) {
    printf ("PSDlin_monitor: %s: Null detection area !\n"
            "ERROR           (xwidth,yheight,xmin,xmax,ymin,ymax). Exiting",
            NAME_CURRENT_COMP);
    exit (0);
  }

  PSDlin_N = create_darr1d (nbins);
  PSDlin_p = create_darr1d (nbins);
  PSDlin_p2 = create_darr1d (nbins);

  // Use instance name for monitor output if no input was given
  if (!strcmp (filename, "\0"))
    sprintf (filename, "%s", NAME_CURRENT_COMP);
  #undef nbins
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef nowritefile
  #undef xwidth
  #undef yheight
  #undef restore_neutron
  #undef vertical
  #undef PSDlin_N
  #undef PSDlin_p
  #undef PSDlin_p2
  return(_comp);
} /* class_PSDlin_monitor_init */



int init(void) { /* called by mccode_main for HZB_FLEX:INITIALISE */
  DEBUG_INSTR();
  // Initialise rng
  srandom(_hash(mcseed-1));

  /* code_main/parseoptions/readparams sets instrument parameters value */
  stracpy(instrument->_name, "HZB_FLEX", 256);

  /* Instrument 'HZB_FLEX' INITIALISE */
  SIG_MESSAGE("[HZB_FLEX] INITIALISE [(null):-1]");
  #define kI (instrument->_parameters.kI)
  #define kF (instrument->_parameters.kF)
  #define wVS (instrument->_parameters.wVS)
  #define tilt (instrument->_parameters.tilt)
  #define SA (instrument->_parameters.SA)
  #define A3 (instrument->_parameters.A3)
  #define A4 (instrument->_parameters.A4)
  #define L3 (instrument->_parameters.L3)
  #define L4 (instrument->_parameters.L4)
  #define Mono_flatswitch (instrument->_parameters.Mono_flatswitch)
{

int    SM=-1;      // monochromator scattering sense is always negative
int    SS=+1;
// int    SA=-1;
double DM = 3.355; // PG 002 monochromator d-spacing in inv AA
double DA = 3.355; // PG 002 analyser d-spacing in inv AA

/* -------calculate spectrometer angles and monochromator/analyser curvatures-------*/
// ----- monochromator
  A1 = SM*RAD2DEG*asin(PI/(DM*kI));
  A2 = 2*A1;

//  RMH = L2/sin(A1*DEG2RAD);
  RMH = (L1+L2)/(2*sin(A1*DEG2RAD)); //additional factor done by RTP
//  rho_MH=1/RMH;
if (Mono_flatswitch == 1) RMH = 0.0;




//  RMV = 2*L2*sin(A1*DEG2RAD);
  RMV = (L1+L2)*sin(A1*DEG2RAD); //Facotr intropduced by RTP
//  rho_MV=1/RMV;

  printf("monochromator angles are:\n A1=%7.3f, A2=%7.3f [deg]\n",A1,A2);
  printf("radius of curvature RMH=%7.3f [m]\nradius of curvature RMV=%7.3f [m] \n",RMH,RMV);

//  printf("curvature rhoMH=%7.3f [m^-1]\ncurvature rhoMV=%7.3f [m] \n",rho_MH,rho_MV);

// ----- sample
//  A3 = 0;
//  A4 = 70;

printf("sample angles are:\n A3=%7.3f, A4=%7.3f [deg]\n",A3,A4);

// ----- analyser
  A5 = SA*RAD2DEG*asin(PI/(DA*kF));
  A6 = 2*A5;
  RAH = (L3+L4)/(2*sin(A5*DEG2RAD)); //Additional factor multiplied by RTP
  RAV = SA*1/rho_AV;
//  RAV should be (L3+L4)*sin(A5*DEG2RAD) but above equation gives better focusing for these specific conditions of kf=1.55 and the divergence...
  printf("analyser angles are:\n A5=%7.3f, A6=%7.3f [deg]\n",A5,A6);
  printf("radius of curvature RAH=%7.3f [m]\nradius of curvature RAV=%7.3f [m] \n",RAH,RAV);

/* ------------------------------calculate EI, lambdaI and so on ------------------*/
  lambdaI=2*PI/kI;  // in AA
  EI=SI_to_meV*hbar*hbar*kI*kI*1e20/2/m_n;

  SelFreq = 3956.06/(lambdaI*(360/(19.7+(2.1*tilt)))*0.25);
//  empirical formulas for source emmitance to save time on simulations!
//  1st attempt: linear
//  dlambdaI=0.01*(7-lambdaI)*lambdaI;
//  2nd attempt, 1/x, better one:
//  dlambdaI=((0.082/(lambdaI+0.4))-0.002)*lambdaI;
//  3rd attempt, 6th order polynomial, even better (with enough parameters we can model an elephant):
 dlambdaI = (1.75004E-05*pow(lambdaI,6) - 5.13903E-04*pow(lambdaI,5) + 6.04404E-03*pow(lambdaI,4) - 3.66989E-02*pow(lambdaI,3) + 1.22399E-01*pow(lambdaI,2) - 2.19626E-01*lambdaI + 2.0E-01)*lambdaI;

  l_min=lambdaI-dlambdaI;
  l_max=lambdaI+dlambdaI;
  e_min=81.81/l_max/l_max;
  e_max=81.81/l_min/l_min;

  printf("velocity selector frequency is %7.3f [Hz]\n",SelFreq);
  printf("lambda=%7.3f [A]\nkI=%7.3f [A^-1] \nEI=%7.3f [meV]\n",lambdaI,kI,EI);
  printf("Maxwellian source forced to emit only between l_min=%7.3f and l_max=%7.3f [A] in order to save simulation time\n",l_min,l_max);
  printf("correspondingly, the energy monitors at sample position are detecting between %7.3f and %7.3f [meV]\n",e_min,e_max);


// guide properties
  beta_guide_1=LGUIDE_1/2800*180/PI;
  beta_guide_2=LGUIDE_2/2800*180/PI;
  s_guide_1 = 2800-sqrt(2800*2800-LGUIDE_1*LGUIDE_1);
  s_guide_2 = 2800-sqrt(2800*2800-LGUIDE_2*LGUIDE_2);
}
  #undef kI
  #undef kF
  #undef wVS
  #undef tilt
  #undef SA
  #undef A3
  #undef A4
  #undef L3
  #undef L4
  #undef Mono_flatswitch
  _Origin_setpos(); /* type Progress_bar */
  _Gen_Source_setpos(); /* type Source_gen */
  _NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_setpos(); /* type Arm */
  _NL1A_NL1B_NL2_NL3_InPile_setpos(); /* type Guide */
  _NL1B_Straight1_Entrance_Window_setpos(); /* type Arm */
  _NL1B_Straight1_setpos(); /* type Guide */
  _NL1B_Curved_Entrance_Window_setpos(); /* type Arm */
  _NL1B_Curved_Guide_1_setpos(); /* type Guide_curved */
  _NL1B_Velocity_Selector_Gap_Entrance_Window_setpos(); /* type Arm */
  _Before_selec_setpos(); /* type PSD_monitor */
  _SELEC_setpos(); /* type Selector */
  _After_selec_setpos(); /* type PSD_monitor */
  _NL1B_Curved_2_Entrance_Window_setpos(); /* type Arm */
  _NL1B_Curved_Guide_2_setpos(); /* type Guide_curved */
  _NL1B_Straight2_Entrance_Window_setpos(); /* type Arm */
  _NL1B_Straight2_setpos(); /* type Guide */
  _NL1B_Elliptical_Entrance_Window_setpos(); /* type Arm */
  _elliptical_piece_setpos(); /* type Guide_tapering */
  _Virtual_source_setpos(); /* type Slit */
  _NL1B_Vertical_Guide_Entrance_Window_setpos(); /* type Arm */
  _NL1B_Vertical_Guide_setpos(); /* type Guide_channeled */
  _NL1B_Guide_Exit_setpos(); /* type Arm */
  _energy_endguide_setpos(); /* type E_monitor */
  _Mono_center_setpos(); /* type Arm */
  _Monochromator_setpos(); /* type Monochromator_curved */
  _Mono_sample_arm_setpos(); /* type Arm */
  _energy_mono_setpos(); /* type E_monitor */
  _energy_pre_sample_setpos(); /* type E_monitor */
  _Sample_center_setpos(); /* type Arm */
  _Sample_analyser_arm_setpos(); /* type Arm */
  _div_mono_setpos(); /* type Divergence_monitor */
  _div_mono_H_setpos(); /* type Div1D_monitor */
  _psd_sam_setpos(); /* type PSD_monitor */
  _dpsd1_setpos(); /* type PSDlin_monitor */

  /* call iteratively all components INITIALISE */
  class_Progress_bar_init(&_Origin_var);

  class_Source_gen_init(&_Gen_Source_var);


  class_Guide_init(&_NL1A_NL1B_NL2_NL3_InPile_var);


  class_Guide_init(&_NL1B_Straight1_var);


  class_Guide_curved_init(&_NL1B_Curved_Guide_1_var);


  class_PSD_monitor_init(&_Before_selec_var);

  class_Selector_init(&_SELEC_var);

  class_PSD_monitor_init(&_After_selec_var);


  class_Guide_curved_init(&_NL1B_Curved_Guide_2_var);


  class_Guide_init(&_NL1B_Straight2_var);


  class_Guide_tapering_init(&_elliptical_piece_var);

  class_Slit_init(&_Virtual_source_var);


  class_Guide_channeled_init(&_NL1B_Vertical_Guide_var);


  class_E_monitor_init(&_energy_endguide_var);


  class_Monochromator_curved_init(&_Monochromator_var);


  class_E_monitor_init(&_energy_mono_var);

  class_E_monitor_init(&_energy_pre_sample_var);



  class_Divergence_monitor_init(&_div_mono_var);

  class_Div1D_monitor_init(&_div_mono_H_var);

  class_PSD_monitor_init(&_psd_sam_var);

  class_PSDlin_monitor_init(&_dpsd1_var);

  if (mcdotrace) display();
  DEBUG_INSTR_END();

#ifdef OPENACC
#include <openacc.h>
#pragma acc update device(_Origin_var)
#pragma acc update device(_Gen_Source_var)
#pragma acc update device(_NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var)
#pragma acc update device(_NL1A_NL1B_NL2_NL3_InPile_var)
#pragma acc update device(_NL1B_Straight1_Entrance_Window_var)
#pragma acc update device(_NL1B_Straight1_var)
#pragma acc update device(_NL1B_Curved_Entrance_Window_var)
#pragma acc update device(_NL1B_Curved_Guide_1_var)
#pragma acc update device(_NL1B_Velocity_Selector_Gap_Entrance_Window_var)
#pragma acc update device(_Before_selec_var)
#pragma acc update device(_SELEC_var)
#pragma acc update device(_After_selec_var)
#pragma acc update device(_NL1B_Curved_2_Entrance_Window_var)
#pragma acc update device(_NL1B_Curved_Guide_2_var)
#pragma acc update device(_NL1B_Straight2_Entrance_Window_var)
#pragma acc update device(_NL1B_Straight2_var)
#pragma acc update device(_NL1B_Elliptical_Entrance_Window_var)
#pragma acc update device(_elliptical_piece_var)
#pragma acc update device(_Virtual_source_var)
#pragma acc update device(_NL1B_Vertical_Guide_Entrance_Window_var)
#pragma acc update device(_NL1B_Vertical_Guide_var)
#pragma acc update device(_NL1B_Guide_Exit_var)
#pragma acc update device(_energy_endguide_var)
#pragma acc update device(_Mono_center_var)
#pragma acc update device(_Monochromator_var)
#pragma acc update device(_Mono_sample_arm_var)
#pragma acc update device(_energy_mono_var)
#pragma acc update device(_energy_pre_sample_var)
#pragma acc update device(_Sample_center_var)
#pragma acc update device(_Sample_analyser_arm_var)
#pragma acc update device(_div_mono_var)
#pragma acc update device(_div_mono_H_var)
#pragma acc update device(_psd_sam_var)
#pragma acc update device(_dpsd1_var)
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
void class_Source_gen_trace(_class_Source_gen *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define flux_file (_comp->_parameters.flux_file)
  #define xdiv_file (_comp->_parameters.xdiv_file)
  #define ydiv_file (_comp->_parameters.ydiv_file)
  #define radius (_comp->_parameters.radius)
  #define dist (_comp->_parameters.dist)
  #define focus_xw (_comp->_parameters.focus_xw)
  #define focus_yh (_comp->_parameters.focus_yh)
  #define focus_aw (_comp->_parameters.focus_aw)
  #define focus_ah (_comp->_parameters.focus_ah)
  #define E0 (_comp->_parameters.E0)
  #define dE (_comp->_parameters.dE)
  #define lambda0 (_comp->_parameters.lambda0)
  #define dlambda (_comp->_parameters.dlambda)
  #define I1 (_comp->_parameters.I1)
  #define yheight (_comp->_parameters.yheight)
  #define xwidth (_comp->_parameters.xwidth)
  #define verbose (_comp->_parameters.verbose)
  #define T1 (_comp->_parameters.T1)
  #define flux_file_perAA (_comp->_parameters.flux_file_perAA)
  #define flux_file_log (_comp->_parameters.flux_file_log)
  #define Lmin (_comp->_parameters.Lmin)
  #define Lmax (_comp->_parameters.Lmax)
  #define Emin (_comp->_parameters.Emin)
  #define Emax (_comp->_parameters.Emax)
  #define T2 (_comp->_parameters.T2)
  #define I2 (_comp->_parameters.I2)
  #define T3 (_comp->_parameters.T3)
  #define I3 (_comp->_parameters.I3)
  #define zdepth (_comp->_parameters.zdepth)
  #define target_index (_comp->_parameters.target_index)
  #define p_in (_comp->_parameters.p_in)
  #define lambda1 (_comp->_parameters.lambda1)
  #define lambda2 (_comp->_parameters.lambda2)
  #define lambda3 (_comp->_parameters.lambda3)
  #define pTable (_comp->_parameters.pTable)
  #define pTable_x (_comp->_parameters.pTable_x)
  #define pTable_y (_comp->_parameters.pTable_y)
  #define pTable_xmin (_comp->_parameters.pTable_xmin)
  #define pTable_xmax (_comp->_parameters.pTable_xmax)
  #define pTable_xsum (_comp->_parameters.pTable_xsum)
  #define pTable_ymin (_comp->_parameters.pTable_ymin)
  #define pTable_ymax (_comp->_parameters.pTable_ymax)
  #define pTable_ysum (_comp->_parameters.pTable_ysum)
  #define pTable_dxmin (_comp->_parameters.pTable_dxmin)
  #define pTable_dxmax (_comp->_parameters.pTable_dxmax)
  #define pTable_dymin (_comp->_parameters.pTable_dymin)
  #define pTable_dymax (_comp->_parameters.pTable_dymax)
  SIG_MESSAGE("[_Gen_Source_trace] component Gen_Source=Source_gen() TRACE [Source_gen:0]");

  double dx=0,dy=0,xf,yf,rf,pdir,chi,v,r, lambda;
  double Maxwell;

  if (verbose >= 0)
  {

    z=0;

    if (radius)
    {
      chi=2*PI*rand01();                          /* Choose point on source */
      r=sqrt(rand01())*radius;                    /* with uniform distribution. */
      x=r*cos(chi);
      y=r*sin(chi);
    }
    else
    {
      x = xwidth*randpm1()/2;   /* select point on source (uniform) */
      y = yheight*randpm1()/2;
    }
    if (zdepth != 0)
      z = zdepth*randpm1()/2;
  /* Assume linear wavelength distribution */
    lambda = lambda0+dlambda*randpm1();
    if (lambda <= 0) ABSORB;
    v = K2V*(2*PI/lambda);

    if (!focus_ah && !focus_aw) {
      randvec_target_rect_real(&xf, &yf, &rf, &pdir,
       0, 0, dist, focus_xw, focus_yh, ROT_A_CURRENT_COMP, x, y, z, 2);

      dx = xf-x;
      dy = yf-y;
      rf = sqrt(dx*dx+dy*dy+dist*dist);

      vz=v*dist/rf;
      vy=v*dy/rf;
      vx=v*dx/rf;
    } else {

      randvec_target_rect_angular(&vx, &vy, &vz, &pdir,
          0, 0, 1, focus_aw*DEG2RAD, focus_ah*DEG2RAD, ROT_A_CURRENT_COMP);
      dx = vx; dy = vy; /* from unit vector */
      vx *= v; vy *= v; vz *= v;
    }
    p = p_in*pdir;

    /* spectral dependency from files or Maxwellians */

    if (flux_file && strlen(flux_file) && strcmp(flux_file,"NULL") && strcmp(flux_file,"0"))
    {
       double binwidth=Table_Value(pTable, lambda, 1);
       if (flux_file_log) binwidth=exp(binwidth);
       p *= binwidth;
    }
    else 

if (T1 > 0 && I1 > 0)
    {
      Maxwell = I1 * SG_Maxwell(lambda, T1);;  /* 1/AA */

      if ((T2 > 0) && (I2 > 0))
      {
        Maxwell += I2 * SG_Maxwell(lambda, T2);
      }
      if ((T3 > 0) && (I3 > 0))
      {
        Maxwell += I3 * SG_Maxwell(lambda, T3);;
      }
      p *= Maxwell;
    }

    /* optional x-xdiv and y-ydiv weightening: position=along columns, div=along rows */
    if (xdiv_file && strlen(xdiv_file)
      && strcmp(xdiv_file,"NULL") && strcmp(xdiv_file,"0") && pTable_xsum > 0) {
      double i,j;
      j = (x-            pTable_xmin) /(pTable_xmax -pTable_xmin) *pTable_x.columns;
      i = (atan2(dx,rf)*RAD2DEG-pTable_dxmin)/(pTable_dxmax-pTable_dxmin)*pTable_x.rows;
      r = Table_Value2d(pTable_x, i,j); /* row, column */
      p *= r/pTable_xsum;
    }
    if (ydiv_file && strlen(ydiv_file)
       && strcmp(ydiv_file,"NULL") && strcmp(ydiv_file,"0") && pTable_ysum > 0) {
      double i,j;
      j = (y-            pTable_ymin) /(pTable_ymax -pTable_ymin) *pTable_y.columns;
      i = (atan2(dy,rf)*RAD2DEG-  pTable_dymin)/(pTable_dymax-pTable_dymin)*pTable_y.rows;
      r = Table_Value2d(pTable_y, i,j);
      p *= r/pTable_ysum;
    }

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
  #undef flux_file
  #undef xdiv_file
  #undef ydiv_file
  #undef radius
  #undef dist
  #undef focus_xw
  #undef focus_yh
  #undef focus_aw
  #undef focus_ah
  #undef E0
  #undef dE
  #undef lambda0
  #undef dlambda
  #undef I1
  #undef yheight
  #undef xwidth
  #undef verbose
  #undef T1
  #undef flux_file_perAA
  #undef flux_file_log
  #undef Lmin
  #undef Lmax
  #undef Emin
  #undef Emax
  #undef T2
  #undef I2
  #undef T3
  #undef I3
  #undef zdepth
  #undef target_index
  #undef p_in
  #undef lambda1
  #undef lambda2
  #undef lambda3
  #undef pTable
  #undef pTable_x
  #undef pTable_y
  #undef pTable_xmin
  #undef pTable_xmax
  #undef pTable_xsum
  #undef pTable_ymin
  #undef pTable_ymax
  #undef pTable_ysum
  #undef pTable_dxmin
  #undef pTable_dxmax
  #undef pTable_dymin
  #undef pTable_dymax
  return;
} /* class_Source_gen_trace */

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
  SIG_MESSAGE("[_NL1A_NL1B_NL2_NL3_InPile_trace] component NL1A_NL1B_NL2_NL3_InPile=Guide() TRACE [Guide:0]");

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
void class_Guide_curved_trace(_class_Guide_curved *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define w1 (_comp->_parameters.w1)
  #define h1 (_comp->_parameters.h1)
  #define l (_comp->_parameters.l)
  #define R0 (_comp->_parameters.R0)
  #define Qc (_comp->_parameters.Qc)
  #define alpha (_comp->_parameters.alpha)
  #define m (_comp->_parameters.m)
  #define W (_comp->_parameters.W)
  #define curvature (_comp->_parameters.curvature)
  SIG_MESSAGE("[_NL1B_Curved_Guide_1_trace] component NL1B_Curved_Guide_1=Guide_curved() TRACE [Guide_curved:0]");

  double t11, t12, t21, t22, theta, alphaAng, endtime, phi;
  double time, time1, time2, q, R;
  int ii, i_bounce;

  double whalf = 0.5 * w1, hhalf = 0.5 * h1;       /* half width and height of guide */
  double z_off = curvature * sin (l / curvature);  /* z-component of total guide length */
  double R1 = curvature - whalf;                   /* radius of curvature of inside mirror */
  double R2 = curvature + whalf;                   /* radius of curvature of outside mirror */
  double vel = sqrt (vx * vx + vy * vy + vz * vz); /* neutron velocity */
  double vel_xz = sqrt (vx * vx + vz * vz);        /* in plane velocity */
  double K = V2K * vel;                            /* neutron wavevector */
  double lambda = 2.0 * PI / K;                    /* neutron wavelength */

  /* Propagate neutron to guide entrance. */

  PROP_Z0;
  if (x <= -whalf || x >= whalf || y <= -hhalf || y >= hhalf)
    ABSORB;
  SCATTER;
  for (;;) {
    double par[] = { R0, Qc, alpha, m, W };
    /* Find itersection points of neutron with inside and outside guide walls */
    ii = cylinder_intersect (&t11, &t12, x - curvature, y, z, vx, vy, vz, R1, h1);
    ii = cylinder_intersect (&t21, &t22, x - curvature, y, z, vx, vy, vz, R2, h1);

    /* Choose appropriate reflection time */
    time1 = (t11 < 1e-7) ? t12 : t11;
    time2 = (t21 < 1e-7) ? t22 : t21;
    time = (time1 < 1e-7 || time2 < time1) ? time2 : time1;

    /* Has neutron left the guide? */
    endtime = (z_off - z) / vz;
    if (time > endtime || time <= 1e-7)
      break;

    PROP_DT (time);

    /* Find reflection surface */
    R = (time == time1) ? R1 : R2;
    i_bounce = (fabs (y - hhalf) < 1e-7 || fabs (y + hhalf) < 1e-7) ? 2 : 1;
    switch (i_bounce) {
    case 1:                          /* Inside or Outside wall */
      phi = atan (vx / vz);          /* angle of neutron trajectory */
      alphaAng = asin (z / R);       /* angle of guide wall */
      theta = fabs (phi - alphaAng); /* angle of reflection */
      vz = vel_xz * cos (2.0 * alphaAng - phi);
      vx = vel_xz * sin (2.0 * alphaAng - phi);
      break;
    case 2: /* Top or Bottom wall */
      theta = fabs (atan (vy / vz));
      vy = -vy;
      break;
    }
    /* Now compute reflectivity. */
    if (m == 0 || !R0)
      ABSORB;

    q = 4.0 * PI * sin (theta) / lambda;
    StdReflecFunc (q, par, &R);
    if (R >= 0)
      p *= R;
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
  #undef w1
  #undef h1
  #undef l
  #undef R0
  #undef Qc
  #undef alpha
  #undef m
  #undef W
  #undef curvature
  return;
} /* class_Guide_curved_trace */

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
  SIG_MESSAGE("[_Before_selec_trace] component Before_selec=PSD_monitor() TRACE [PSD_monitor:0]");

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
void class_Selector_trace(_class_Selector *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define length (_comp->_parameters.length)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define nslit (_comp->_parameters.nslit)
  #define d (_comp->_parameters.d)
  #define radius (_comp->_parameters.radius)
  #define alpha (_comp->_parameters.alpha)
  #define nu (_comp->_parameters.nu)
  SIG_MESSAGE("[_SELEC_trace] component SELEC=Selector() TRACE [Selector:0]");

  double E;
  double dt;
  double open_angle, closed_angle, n_angle_in, n_angle_out;
  double sel_phase, act_radius; // distance between neutron and selector axle

  PROP_Z0;
  E = VS2E * (vx * vx + vy * vy + vz * vz);
  if (x < xmin || x > xmax || y < ymin || y > ymax)
    ABSORB; /* because outside frame */
  dt = length / vz;
  /* get phase angle of selector rotor as MonteCarlo choice
   only the free space between two neighboring spokes is taken
   p is adjusted to transmission for parallel beam
  */
  n_angle_in = atan2 (x, y + radius) * RAD2DEG;
  act_radius = sqrt (x * x + pow (radius + y, 2));
  closed_angle = d / act_radius * RAD2DEG;
  open_angle = 360 / nslit - closed_angle;
  sel_phase = open_angle * rand01 ();
  p *= (open_angle / (closed_angle + open_angle));

  PROP_DT (dt);

  if (x < xmin || x > xmax || y < ymin || y > ymax)
    ABSORB; /* because outside frame */
  /* now let's look whether the neutron is still
  between the same two spokes or absorbed meanwhile */

  n_angle_out = atan2 (x, y + radius) * RAD2DEG; /* neutron beam might be divergent */

  sel_phase = sel_phase + nu * dt * 360 - alpha; /* rotor turned, but spokes are torsaded */

  if (n_angle_out < (n_angle_in - sel_phase) || n_angle_out > (n_angle_in - sel_phase + open_angle))
    ABSORB; /* because must have passed absorber */
  else
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
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef length
  #undef xwidth
  #undef yheight
  #undef nslit
  #undef d
  #undef radius
  #undef alpha
  #undef nu
  return;
} /* class_Selector_trace */

#pragma acc routine
void class_Guide_tapering_trace(_class_Guide_tapering *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define option (_comp->_parameters.option)
  #define w1 (_comp->_parameters.w1)
  #define h1 (_comp->_parameters.h1)
  #define l (_comp->_parameters.l)
  #define linw (_comp->_parameters.linw)
  #define loutw (_comp->_parameters.loutw)
  #define linh (_comp->_parameters.linh)
  #define louth (_comp->_parameters.louth)
  #define R0 (_comp->_parameters.R0)
  #define Qcx (_comp->_parameters.Qcx)
  #define Qcy (_comp->_parameters.Qcy)
  #define alphax (_comp->_parameters.alphax)
  #define alphay (_comp->_parameters.alphay)
  #define W (_comp->_parameters.W)
  #define mx (_comp->_parameters.mx)
  #define my (_comp->_parameters.my)
  #define segno (_comp->_parameters.segno)
  #define curvature (_comp->_parameters.curvature)
  #define curvature_v (_comp->_parameters.curvature_v)
  #define w1c (_comp->_parameters.w1c)
  #define w2c (_comp->_parameters.w2c)
  #define ww (_comp->_parameters.ww)
  #define hh (_comp->_parameters.hh)
  #define whalf (_comp->_parameters.whalf)
  #define hhalf (_comp->_parameters.hhalf)
  #define lwhalf (_comp->_parameters.lwhalf)
  #define lhhalf (_comp->_parameters.lhhalf)
  #define h1_in (_comp->_parameters.h1_in)
  #define h2_out (_comp->_parameters.h2_out)
  #define w1_in (_comp->_parameters.w1_in)
  #define w2_out (_comp->_parameters.w2_out)
  #define l_seg (_comp->_parameters.l_seg)
  #define h12 (_comp->_parameters.h12)
  #define h2 (_comp->_parameters.h2)
  #define w12 (_comp->_parameters.w12)
  #define w2 (_comp->_parameters.w2)
  #define a_ell_q (_comp->_parameters.a_ell_q)
  #define b_ell_q (_comp->_parameters.b_ell_q)
  #define lbw (_comp->_parameters.lbw)
  #define lbh (_comp->_parameters.lbh)
  #define mxi (_comp->_parameters.mxi)
  #define u1 (_comp->_parameters.u1)
  #define u2 (_comp->_parameters.u2)
  #define div1 (_comp->_parameters.div1)
  #define p2_para (_comp->_parameters.p2_para)
  #define test (_comp->_parameters.test)
  #define Div1 (_comp->_parameters.Div1)
  #define seg (_comp->_parameters.seg)
  #define fu (_comp->_parameters.fu)
  #define pos (_comp->_parameters.pos)
  #define file_name (_comp->_parameters.file_name)
  #define ep (_comp->_parameters.ep)
  #define num (_comp->_parameters.num)
  #define rotation_h (_comp->_parameters.rotation_h)
  #define rotation_v (_comp->_parameters.rotation_v)
  SIG_MESSAGE("[_elliptical_piece_trace] component elliptical_piece=Guide_tapering() TRACE [Guide_tapering:0]");

  double t1, t2, ts, zr;                         /* Intersection times. */
  double av, ah, bv, bh, cv1, cv2, ch1, ch2, dd; /* Intermediate values */
  double vdotn_v1, vdotn_v2, vdotn_h1, vdotn_h2; /* Dot products. */
  int i;                                         /* Which mirror hit? */
  double q;                                      /* Q [1/AA] of reflection */
  double vlen2, nlen2;                           /* Vector lengths squared */
  double edge;
  double hadj; /* Channel displacement */
  int ii;

  /* Propagate neutron to guide entrance. */
  PROP_Z0;
  for (ii = 0; ii < seg; ii++) {
    zr = ii * l_seg;
    /* Propagate neutron to segment entrance. */
    ts = (zr - z) / vz;
    PROP_DT (ts);
    if (x <= w1_in[ii] / -2.0 || x >= w1_in[ii] / 2.0 || y <= -hhalf[ii] || y >= hhalf[ii])
      ABSORB;
    /* Shift origin to center of channel hit (absorb if hit dividing walls) */
    x += w1_in[ii] / 2.0;
    edge = floor (x / w1c[ii]) * w1c[ii];
    if (x - edge > w1c[ii]) {
      x -= w1_in[ii] / 2.0; /* Re-adjust origin */
      ABSORB;
    }
    x -= (edge + (w1c[ii] / 2.0));
    hadj = edge + (w1c[ii] / 2.0) - w1_in[ii] / 2.0;
    for (;;) {
      /* Compute the dot products of v and n for the four mirrors. */
      ts = (zr - z) / vz;
      av = l_seg * vx;
      bv = ww[ii] * vz;
      ah = l_seg * vy;
      bh = hh[ii] * vz;
      vdotn_v1 = bv + av; /* Left vertical */
      vdotn_v2 = bv - av; /* Right vertical */
      vdotn_h1 = bh + ah; /* Lower horizontal */
      vdotn_h2 = bh - ah; /* Upper horizontal */
      /* Compute the dot products of (O - r) and n as c1+c2 and c1-c2 */
      cv1 = -whalf[ii] * l_seg - (z - zr) * ww[ii];
      cv2 = x * l_seg;
      ch1 = -hhalf[ii] * l_seg - (z - zr) * hh[ii];
      ch2 = y * l_seg;
      /* Compute intersection times. */
      t1 = (zr + l_seg - z) / vz;
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
      if (i == 0) {
        break; /* Neutron left guide. */
      }
      PROP_DT (t1);
      switch (i) {
      case 1: /* Left vertical mirror */
        nlen2 = l_seg * l_seg + ww[ii] * ww[ii];
        q = V2Q * (-2) * vdotn_v1 / sqrt (nlen2);
        dd = 2 * vdotn_v1 / nlen2;
        vx = vx - dd * l_seg;
        vz = vz - dd * ww[ii];
        break;
      case 2: /* Right vertical mirror */
        nlen2 = l_seg * l_seg + ww[ii] * ww[ii];
        q = V2Q * (-2) * vdotn_v2 / sqrt (nlen2);
        dd = 2 * vdotn_v2 / nlen2;
        vx = vx + dd * l_seg;
        vz = vz - dd * ww[ii];
        break;
      case 3: /* Lower horizontal mirror */
        nlen2 = l_seg * l_seg + hh[ii] * hh[ii];
        q = V2Q * (-2) * vdotn_h1 / sqrt (nlen2);
        dd = 2 * vdotn_h1 / nlen2;
        vy = vy - dd * l_seg;
        vz = vz - dd * hh[ii];
        break;
      case 4: /* Upper horizontal mirror */
        nlen2 = l_seg * l_seg + hh[ii] * hh[ii];
        q = V2Q * (-2) * vdotn_h2 / sqrt (nlen2);
        dd = 2 * vdotn_h2 / nlen2;
        vy = vy + dd * l_seg;
        vz = vz - dd * hh[ii];
        break;
      }
      /* Now compute reflectivity. */
      if ((i <= 2 && mx == 0) || (i > 2 && my == 0)) {
        x += hadj; /* Re-adjust origin */
        ABSORB;
      } else {
        double ref = 1;
        if (i <= 2) {
          double m = (mx > 0 ? mx : fabs (mx * w1 / w1_in[ii]));
          double par[] = { R0, Qcx, alphax, m, W };
          StdReflecFunc (q, par, &ref);
          if (ref > 0)
            p *= ref;
          else {
            x += hadj; /* Re-adjust origin */
            ABSORB;    /* Cutoff ~ 1E-10 */
          }
        } else {
          double m = (my > 0 ? my : fabs (my * h1 / h1_in[ii]));
          double par[] = { R0, Qcy, alphay, m, W };
          StdReflecFunc (q, par, &ref);
          if (ref > 0)
            p *= ref;
          else {
            x += hadj; /* Re-adjust origin */
            ABSORB;    /* Cutoff ~ 1E-10 */
          }
        }
      }
      x += hadj;
      SCATTER;
      x -= hadj;
    } /* loop on reflections inside segment */
    x += hadj; /* Re-adjust origin */

    /* rotate neutron according to actual guide curvature */
    if (rotation_h) {
      double nvx, nvy, nvz;
      rotate (nvx, nvy, nvz, vx, vy, vz, -rotation_h, 0, 1, 0);
      vx = nvx;
      vy = nvy;
      vz = nvz;
    }
    if (rotation_v) {
      double nvx, nvy, nvz;
      rotate (nvx, nvy, nvz, vx, vy, vz, -rotation_v, 1, 0, 0);
      vx = nvx;
      vy = nvy;
      vz = nvz;
    }
  } /* loop on segments */
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
  #undef option
  #undef w1
  #undef h1
  #undef l
  #undef linw
  #undef loutw
  #undef linh
  #undef louth
  #undef R0
  #undef Qcx
  #undef Qcy
  #undef alphax
  #undef alphay
  #undef W
  #undef mx
  #undef my
  #undef segno
  #undef curvature
  #undef curvature_v
  #undef w1c
  #undef w2c
  #undef ww
  #undef hh
  #undef whalf
  #undef hhalf
  #undef lwhalf
  #undef lhhalf
  #undef h1_in
  #undef h2_out
  #undef w1_in
  #undef w2_out
  #undef l_seg
  #undef h12
  #undef h2
  #undef w12
  #undef w2
  #undef a_ell_q
  #undef b_ell_q
  #undef lbw
  #undef lbh
  #undef mxi
  #undef u1
  #undef u2
  #undef div1
  #undef p2_para
  #undef test
  #undef Div1
  #undef seg
  #undef fu
  #undef pos
  #undef file_name
  #undef ep
  #undef num
  #undef rotation_h
  #undef rotation_v
  return;
} /* class_Guide_tapering_trace */

#pragma acc routine
void class_Slit_trace(_class_Slit *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define radius (_comp->_parameters.radius)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define isradial (_comp->_parameters.isradial)
  SIG_MESSAGE("[_Virtual_source_trace] component Virtual_source=Slit() TRACE [Slit:0]");

  PROP_Z0;
  if (!isradial ? (x < xmin || x > xmax || y < ymin || y > ymax) : (x * x + y * y > radius * radius))
    ABSORB;
  else
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
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef radius
  #undef xwidth
  #undef yheight
  #undef isradial
  return;
} /* class_Slit_trace */

#pragma acc routine
void class_Guide_channeled_trace(_class_Guide_channeled *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define w1 (_comp->_parameters.w1)
  #define h1 (_comp->_parameters.h1)
  #define w2 (_comp->_parameters.w2)
  #define h2 (_comp->_parameters.h2)
  #define l (_comp->_parameters.l)
  #define R0 (_comp->_parameters.R0)
  #define Qc (_comp->_parameters.Qc)
  #define alpha (_comp->_parameters.alpha)
  #define m (_comp->_parameters.m)
  #define nslit (_comp->_parameters.nslit)
  #define d (_comp->_parameters.d)
  #define Qcx (_comp->_parameters.Qcx)
  #define Qcy (_comp->_parameters.Qcy)
  #define alphax (_comp->_parameters.alphax)
  #define alphay (_comp->_parameters.alphay)
  #define W (_comp->_parameters.W)
  #define mx (_comp->_parameters.mx)
  #define my (_comp->_parameters.my)
  #define nu (_comp->_parameters.nu)
  #define phase (_comp->_parameters.phase)
  #define w1c (_comp->_parameters.w1c)
  #define w2c (_comp->_parameters.w2c)
  #define ww (_comp->_parameters.ww)
  #define hh (_comp->_parameters.hh)
  #define whalf (_comp->_parameters.whalf)
  #define hhalf (_comp->_parameters.hhalf)
  #define lwhalf (_comp->_parameters.lwhalf)
  #define lhhalf (_comp->_parameters.lhhalf)
  SIG_MESSAGE("[_NL1B_Vertical_Guide_trace] component NL1B_Vertical_Guide=Guide_channeled() TRACE [Guide_channeled:0]");

  double t1, t2;                                 /* Intersection times. */
  double av, ah, bv, bh, cv1, cv2, ch1, ch2, dd; /* Intermediate values */
  double vdotn_v1, vdotn_v2, vdotn_h1, vdotn_h2; /* Dot products. */
  int i;                                         /* Which mirror hit? */
  double q;                                      /* Q [1/AA] of reflection */
  double nlen2;                                  /* Vector lengths squared */
  double edge;
  double hadj; /* Channel displacement */
  double angle = 0;

  if (nu != 0 || phase != 0) { /* rotate neutron w/r to guide element */
    /* approximation of rotating straight Fermi Chopper */
    Coords X = coords_set (x, y, z - l / 2); /* current coordinates of neutron in centered static frame */
    Rotation R;
    double dt = (-z + l / 2) / vz;                   /* time shift to each center of slit package */
    angle = fmod (360 * nu * (t + dt) + phase, 360); /* in deg */
    /* modify angle so that Z0 guide side is always in front of incoming neutron */
    if (angle > 90 && angle < 270) {
      angle -= 180;
    }
    angle *= DEG2RAD;
    rot_set_rotation (R, 0, -angle, 0); /* will rotate neutron instead of comp: negative side */
    /* apply rotation to centered coordinates */
    Coords RX = rot_apply (R, X);
    coords_get (RX, &x, &y, &z);
    z = z + l / 2;
    /* rotate speed */
    X = coords_set (vx, vy, vz);
    RX = rot_apply (R, X);
    coords_get (RX, &vx, &vy, &vz);
  }

  /* Propagate neutron to guide entrance. */
  PROP_Z0;
  /* Scatter here to ensure that fully transmitted neutrons will not be
     absorbed in a GROUP construction, e.g. all neutrons - even the
     later absorbed ones are scattered at the guide entry. */
  SCATTER;
  if (x <= w1 / -2.0 || x >= w1 / 2.0 || y <= -hhalf || y >= hhalf)
    ABSORB;
  /* Shift origin to center of channel hit (absorb if hit dividing walls) */
  x += w1 / 2.0;
  edge = floor (x / w1c) * w1c;
  if (x - edge > w1c - d) {
    x -= w1 / 2.0; /* Re-adjust origin */
    ABSORB;
  }
  x -= (edge + (w1c - d) / 2.0);
  hadj = edge + (w1c - d) / 2.0 - w1 / 2.0;
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
      dd = 2 * vdotn_v1 / nlen2;
      vx = vx - dd * l;
      vz = vz - dd * ww;
      break;
    case 2: /* Right vertical mirror */
      nlen2 = l * l + ww * ww;
      q = V2Q * (-2) * vdotn_v2 / sqrt (nlen2);
      dd = 2 * vdotn_v2 / nlen2;
      vx = vx + dd * l;
      vz = vz - dd * ww;
      break;
    case 3: /* Lower horizontal mirror */
      nlen2 = l * l + hh * hh;
      q = V2Q * (-2) * vdotn_h1 / sqrt (nlen2);
      dd = 2 * vdotn_h1 / nlen2;
      vy = vy - dd * l;
      vz = vz - dd * hh;
      break;
    case 4: /* Upper horizontal mirror */
      nlen2 = l * l + hh * hh;
      q = V2Q * (-2) * vdotn_h2 / sqrt (nlen2);
      dd = 2 * vdotn_h2 / nlen2;
      vy = vy + dd * l;
      vz = vz - dd * hh;
      break;
    }
    /* Now compute reflectivity. */
    if ((i <= 2 && mx == 0) || (i > 2 && my == 0)) {
      x += hadj; /* Re-adjust origin */
      ABSORB;
    } else {
      double ref = 1;
      if (i <= 2) {
        double par[] = { R0, Qcx, alphax, mx, W };
        StdReflecFunc (q, par, &ref);
        if (ref > 0)
          p *= ref;
        else {
          x += hadj; /* Re-adjust origin */
          ABSORB;    /* Cutoff ~ 1E-10 */
        }
      } else {
        double par[] = { R0, Qcy, alphay, my, W };
        StdReflecFunc (q, par, &ref);
        if (ref > 0)
          p *= ref;
        else {
          x += hadj; /* Re-adjust origin */
          ABSORB;    /* Cutoff ~ 1E-10 */
        }
      }
    }
    x += hadj;
    SCATTER;
    x -= hadj;
  } /* end for */
  x += hadj;                   /* Re-adjust origin */
  if (nu != 0 || phase != 0) { /* rotate back neutron w/r to guide element */
    /* approximation of rotating straight Fermi Chopper */
    Coords X = coords_set (x, y, z - l / 2); /* current coordinates of neutron in centered static frame */
    Rotation R;
    rot_set_rotation (R, 0, angle, 0); /* will rotate back neutron: positive side */
    /* apply rotation to centered coordinates */
    Coords RX = rot_apply (R, X);
    coords_get (RX, &x, &y, &z);
    z = z + l / 2;
    /* rotate speed */
    X = coords_set (vx, vy, vz);
    RX = rot_apply (R, X);
    coords_get (RX, &vx, &vy, &vz);
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
  #undef w1
  #undef h1
  #undef w2
  #undef h2
  #undef l
  #undef R0
  #undef Qc
  #undef alpha
  #undef m
  #undef nslit
  #undef d
  #undef Qcx
  #undef Qcy
  #undef alphax
  #undef alphay
  #undef W
  #undef mx
  #undef my
  #undef nu
  #undef phase
  #undef w1c
  #undef w2c
  #undef ww
  #undef hh
  #undef whalf
  #undef hhalf
  #undef lwhalf
  #undef lhhalf
  return;
} /* class_Guide_channeled_trace */

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
  SIG_MESSAGE("[_energy_endguide_trace] component energy_endguide=E_monitor() TRACE [E_monitor:0]");

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
void class_Monochromator_curved_trace(_class_Monochromator_curved *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define reflect (_comp->_parameters.reflect)
  #define transmit (_comp->_parameters.transmit)
  #define zwidth (_comp->_parameters.zwidth)
  #define yheight (_comp->_parameters.yheight)
  #define gap (_comp->_parameters.gap)
  #define NH (_comp->_parameters.NH)
  #define NV (_comp->_parameters.NV)
  #define mosaich (_comp->_parameters.mosaich)
  #define mosaicv (_comp->_parameters.mosaicv)
  #define r0 (_comp->_parameters.r0)
  #define t0 (_comp->_parameters.t0)
  #define Q (_comp->_parameters.Q)
  #define RV (_comp->_parameters.RV)
  #define RH (_comp->_parameters.RH)
  #define DM (_comp->_parameters.DM)
  #define mosaic (_comp->_parameters.mosaic)
  #define width (_comp->_parameters.width)
  #define height (_comp->_parameters.height)
  #define verbose (_comp->_parameters.verbose)
  #define order (_comp->_parameters.order)
  #define mos_rms_y (_comp->_parameters.mos_rms_y)
  #define mos_rms_z (_comp->_parameters.mos_rms_z)
  #define mos_rms_max (_comp->_parameters.mos_rms_max)
  #define mono_Q (_comp->_parameters.mono_Q)
  #define SlabWidth (_comp->_parameters.SlabWidth)
  #define SlabHeight (_comp->_parameters.SlabHeight)
  #define rTable (_comp->_parameters.rTable)
  #define tTable (_comp->_parameters.tTable)
  #define rTableFlag (_comp->_parameters.rTableFlag)
  #define tTableFlag (_comp->_parameters.tTableFlag)
  #define tiltH (_comp->_parameters.tiltH)
  #define tiltV (_comp->_parameters.tiltV)
  #define ncol_var (_comp->_parameters.ncol_var)
  #define nrow_var (_comp->_parameters.nrow_var)
  SIG_MESSAGE("[_Monochromator_trace] component Monochromator=Monochromator_curved() TRACE [Monochromator_curved:0]");

  double dt;
  double Gauss_X[]
      = { -0.987992518020485, -0.937273392400706, -0.848206583410427, -0.724417731360170, -0.570972172608539, -0.394151347077563, -0.201194093997435, 0,
          0.201194093997435,  0.394151347077563,  0.570972172608539,  0.724417731360170,  0.848206583410427,  0.937273392400706,  0.987992518020485 };
  double Gauss_W[]
      = { 0.030753241996117, 0.070366047488108, 0.107159220467172, 0.139570677926154, 0.166269205816994, 0.186161000115562, 0.198431485327111, 0.202578241925561,
          0.198431485327111, 0.186161000115562, 0.166269205816994, 0.139570677926154, 0.107159220467172, 0.070366047488108, 0.030753241996117 };

  if (vx != 0.0 && (dt = -x / vx) >= 0.0 && r0) { /* Moving towards crystal? */
    double zmin, zmax, ymin, ymax;
    double yy, zz;

    zmax = ((NH * (SlabWidth + gap)) - gap) / 2;
    zmin = -zmax;
    ymax = ((NV * (SlabHeight + gap)) - gap) / 2;
    ymin = -ymax;

    /* Test-propagate to crystal plane */
    zz = z + vz * dt;
    yy = y + vy * dt;
    if (zz > zmin && zz < zmax && yy > ymin && yy < ymax) { /* Intersect the crystal? */
      double tilth, tiltv;                                  /* used to calculate tilt angle of slab */
      double ratio, Q_order, k, kux, kuy, kuz;
      double kix, kiy, kiz;
      int do_transmit = 0;
      int row, col;

      col = ceil ((zz - zmin) / (SlabWidth + gap)); /* which slab hit ? */
      row = ceil ((yy - ymin) / (SlabHeight + gap));

      particle_setvar_void (_particle, ncol_var, &col);
      particle_setvar_void (_particle, nrow_var, &row);

      if (RH != 0) {
        tilth = tiltH ? tiltH[(int)col] : asin ((col - (NH + 1) / 2.0) * (SlabWidth + gap) / RH);
      } else {
        tilth = 0;
      }
      if (RV != 0) {
        tiltv = tiltV ? tiltV[(int)row] : -asin ((row - (NV + 1) / 2.0) * (SlabHeight + gap) / RV);
      } else {
        tiltv = 0;
      }

      /* rotate with tilth (around Y) and tiltv (around Z), center on plate */
      double center_z = zmin + (col - 0.5) * (SlabWidth + gap) - gap / 2;
      double center_y = ymin + (row - 0.5) * (SlabHeight + gap) - gap / 2;

      Rotation T;
      rot_set_rotation (T, 0, tilth, tiltv);
      /* now make the coordinate system change */
      mccoordschange_polarisation (T, &vx, &vy, &vz);
      y = y - center_y;
      z = z - center_z;
      coords_get (rot_apply (T, coords_set (x, y, z)), &x, &y, &z);

      /* this is where polarisation should be handled, plus further down */
      /* mccoordschange_polarisation(t, &sx, &sy, &sz); */

      /* now propagate to slab plane */
      PROP_X0;

      /* Hit a slab or a gap ?*/
      int inside = inside_rectangle (z, y, SlabWidth, SlabHeight);
      if (inside) {     /* not in gap ? */
        kix = V2K * vx; /* Initial wave vector */
        kiy = V2K * vy;
        kiz = V2K * vz;
        /* Get reflection order and corresponding nominal scattering vector q0
          of correct length and direction. Only the order with the closest
          scattering vector is considered */
        ratio = -2 * kix / mono_Q;
        Q_order = floor (ratio + .5);
        if (Q_order == 0.0)
          Q_order = ratio < 0 ? -1 : 1;
        /* Order will be negative when the neutron enters from the back, in
          which case the direction of Q0 is flipped. */
        if (Q_order < 0)
          Q_order = -Q_order;
        /* Make sure the order is small enough to allow Bragg scattering at the
          given neutron wavelength */
        k = sqrt (kix * kix + kiy * kiy + kiz * kiz);
        kux = kix / k; /* Unit vector along ki */
        kuy = kiy / k;
        kuz = kiz / k;
        if (Q_order > 2 * k / mono_Q)
          Q_order--;
        if ((!order && Q_order > 0) || (Q_order == fabs (order) && order)) { /* Bragg scattering possible? */
          double q0, q0x, theta, delta, p_reflect, my_r0;

          q0 = Q_order * mono_Q;
          q0x = ratio < 0 ? -q0 : q0;
          theta = asin (q0 / (2 * k)); /* Actual bragg angle */
          /* Make MC choice: reflect or transmit? */
          delta = asin (fabs (kux)) - theta;

          if (rTableFlag) {
            my_r0 = r0 * Table_Value (rTable, k, 1); /* 2nd column */
          } else
            my_r0 = r0;
          if (my_r0 > 1) {
            if (my_r0 > 1.01 && verbose)
              fprintf (stdout, "Warning: Monochromator_curved : lowered reflectivity from %f to 1 (k=%f)\n", my_r0, k);
            my_r0 = 0.999;
          }
          if (my_r0 < 0) {
            if (verbose)
              fprintf (stdout, "Warning: Monochromator_curved : raised reflectivity from %f to 0 (k=%f)\n", my_r0, k);
            my_r0 = 0;
          }

          p_reflect = fabs (my_r0) * exp (-kiz * kiz / (kiy * kiy + kiz * kiz) * (delta * delta) / (2 * mos_rms_y * mos_rms_y))
                      * exp (-kiy * kiy / (kiy * kiy + kiz * kiz) * (delta * delta) / (2 * mos_rms_z * mos_rms_z));

          double rr = rand01 ();
          if (rr <= p_reflect) { /* Reflect */
            double bx, by, bz, ax, ay, az, phi;
            double cos_2theta, k_sin_2theta, cos_phi, sin_phi, q_x, q_y, q_z;
            double total, c1x, c1y, c1z, w, mos_sample;
            int i = 0;

            cos_2theta = cos (2 * theta);
            k_sin_2theta = k * sin (2 * theta);
            /* Get unit normal to plane containing ki and most probable kf */
            vec_prod (bx, by, bz, kix, kiy, kiz, q0x, 0, 0);
            NORM (bx, by, bz);
            bx = bx * k_sin_2theta;
            by = by * k_sin_2theta;
            bz = bz * k_sin_2theta;
            /* Get unit vector normal to ki and b */
            vec_prod (ax, ay, az, bx, by, bz, kux, kuy, kuz);
            /* Compute the total scattering probability at this ki */
            total = 0;
            /* Choose width of Gaussian distribution to sample the angle
             * phi on the Debye-Scherrer cone for the scattered neutron.
             * The radius of the Debye-Scherrer cone is smaller by a
             * factor 1/cos(theta) than the radius of the (partial) sphere
             * describing the possible orientations of Q due to mosaicity, so we
             * start with a width 1/cos(theta) greater than the largest of
             * the two mosaics. */
            mos_sample = mos_rms_max / cos (theta);
            c1x = kix * (cos_2theta - 1);
            c1y = kiy * (cos_2theta - 1);
            c1z = kiz * (cos_2theta - 1);
            /* Loop, repeatedly reducing the sample width until it is small
             * enough to avoid sampling scattering directions with
             * ridiculously low scattering probability.
             * Use a cut-off at 5 times the gauss width for considering
             * scattering probability as well as for integration limits
             * when integrating the sampled distribution below. */
            for (i = 0; i < 100; i++) {
              w = 5 * mos_sample;
              cos_phi = cos (w);
              sin_phi = sin (w);
              q_x = c1x + cos_phi * ax + sin_phi * bx;
              q_y = (c1y + cos_phi * ay + sin_phi * by) / mos_rms_z;
              q_z = (c1z + cos_phi * az + sin_phi * bz) / mos_rms_y;
              /* Stop when we get near a factor of 25=5^2. */
              if (q_z * q_z + q_y * q_y < (25 / (2.0 / 3.0)) * (q_x * q_x))
                break;
              mos_sample *= (2.0 / 3.0);
            }
            /* Now integrate the chosen sampling distribution, using a
             * cut-off at five times sigma. */
            for (i = 0; i < (sizeof (Gauss_X) / sizeof (double)); i++) {
              phi = w * Gauss_X[i];
              cos_phi = cos (phi);
              sin_phi = sin (phi);
              q_x = c1x + cos_phi * ax + sin_phi * bx;
              q_y = c1y + cos_phi * ay + sin_phi * by;
              q_z = c1z + cos_phi * az + sin_phi * bz;
              p_reflect = GAUSS_monocurved ((q_z / q_x), 0, mos_rms_y) * GAUSS_monocurved ((q_y / q_x), 0, mos_rms_z);
              total += Gauss_W[i] * p_reflect;
            }
            total *= w;
            /* Choose point on Debye-Scherrer cone. Sample from a Gaussian of
             * width 1/cos(theta) greater than the mosaic and correct for any
             * error by adjusting the neutron weight later. */
            phi = mos_sample * randnorm ();
            /* Compute final wave vector kf and scattering vector q = ki - kf */
            cos_phi = cos (phi);
            sin_phi = sin (phi);
            q_x = c1x + cos_phi * ax + sin_phi * bx;
            q_y = c1y + cos_phi * ay + sin_phi * by;
            q_z = c1z + cos_phi * az + sin_phi * bz;
            p_reflect = GAUSS_monocurved ((q_z / q_x), 0, mos_rms_y) * GAUSS_monocurved ((q_y / q_x), 0, mos_rms_z);

            vx = K2V * (kix + q_x);
            vy = K2V * (kiy + q_y);
            vz = K2V * (kiz + q_z);
            p_reflect /= total * GAUSS_monocurved (phi, 0, mos_sample);
            if (p_reflect <= 0)
              ABSORB;
            if (p_reflect > 1)
              p_reflect = 1;
            p = p * p_reflect;

          } /* End MC choice to reflect or transmit neutron (if tmp<p_reflect) */
          else {
            do_transmit = 1;
          }
          /* else transmit neutron */
        } /* End bragg scattering possible (if order) */
        else {
          do_transmit = 1;
        }
        if (do_transmit) {
          double my_t0;
          if (tTableFlag != 0) {
            my_t0 = t0 * Table_Value (tTable, k, 1); /* 2nd column */
          } else
            my_t0 = t0;
          /* do not SCATTER, else GROUP does not work */
          if (my_t0 > 1) {
            if (my_t0 > 1.01 && verbose)
              fprintf (stdout, "Warning: Monochromator_curved : lowered transmission from %f to 1 (k=%f)\n", my_t0, k);
            my_t0 = 0.999;
          }
          if (my_t0 > 0)
            p = p * my_t0;
          else
            ABSORB;
        }
      } /* end if not in gap */
      /* rotate back in component frame */
      Rotation TT;
      rot_transpose (T, TT);
      /* now make the coordinate system change */
      mccoordschange_polarisation (TT, &vx, &vy, &vz);
      coords_get (rot_apply (TT, coords_set (x, y, z)), &x, &y, &z);
      y = y + center_y;
      z = z + center_z;
      /* Visualise scattering point in proper, component frame
         - but only if the neutron is reflected, that is none of:
         * transmitted
         * falling outside the slab material */
      if (!do_transmit)
        SCATTER;

      /* mccoordschange_polarisation(tt, &sx, &sy, &sz); */
    } /* End intersect the crystal (if z) */
    else {
      /* restore neutron state when no interaction */
      RESTORE_NEUTRON (INDEX_CURRENT_COMP, x, y, z, vx, vy, vz, t, sx, sy, sz, p);
    }
  } /* End neutron moving towards crystal (if vx)*/
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
  #undef transmit
  #undef zwidth
  #undef yheight
  #undef gap
  #undef NH
  #undef NV
  #undef mosaich
  #undef mosaicv
  #undef r0
  #undef t0
  #undef Q
  #undef RV
  #undef RH
  #undef DM
  #undef mosaic
  #undef width
  #undef height
  #undef verbose
  #undef order
  #undef mos_rms_y
  #undef mos_rms_z
  #undef mos_rms_max
  #undef mono_Q
  #undef SlabWidth
  #undef SlabHeight
  #undef rTable
  #undef tTable
  #undef rTableFlag
  #undef tTableFlag
  #undef tiltH
  #undef tiltV
  #undef ncol_var
  #undef nrow_var
  return;
} /* class_Monochromator_curved_trace */

#pragma acc routine
void class_Divergence_monitor_trace(_class_Divergence_monitor *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define nh (_comp->_parameters.nh)
  #define nv (_comp->_parameters.nv)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define maxdiv_h (_comp->_parameters.maxdiv_h)
  #define maxdiv_v (_comp->_parameters.maxdiv_v)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nx (_comp->_parameters.nx)
  #define ny (_comp->_parameters.ny)
  #define nz (_comp->_parameters.nz)
  #define Div_N (_comp->_parameters.Div_N)
  #define Div_p (_comp->_parameters.Div_p)
  #define Div_p2 (_comp->_parameters.Div_p2)
  SIG_MESSAGE("[_div_mono_trace] component div_mono=Divergence_monitor() TRACE [Divergence_monitor:0]");

  int i, j;
  double h_div, v_div;
  double v, vn;

  PROP_Z0;
  if (x > xmin && x < xmax && y > ymin && y < ymax) {
    /* Find length of projection onto the [nx ny nz] axis */
    vn = scalar_prod (vx, vy, vz, nx, ny, nz);
    h_div = RAD2DEG * atan2 (vx, vn);
    v_div = RAD2DEG * atan2 (vy, vn);
    if (h_div < maxdiv_h && h_div > -maxdiv_h && v_div < maxdiv_v && v_div > -maxdiv_v) {
      i = floor ((h_div + maxdiv_h) * nh / (2.0 * maxdiv_h));
      j = floor ((v_div + maxdiv_v) * nv / (2.0 * maxdiv_v));
      double p2 = p * p;
      #pragma acc atomic
      Div_N[i][j] = Div_N[i][j] + 1;
      #pragma acc atomic
      Div_p[i][j] = Div_p[i][j] + p;
      #pragma acc atomic
      Div_p2[i][j] = Div_p2[i][j] + p2;
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
  #undef nh
  #undef nv
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef nowritefile
  #undef xwidth
  #undef yheight
  #undef maxdiv_h
  #undef maxdiv_v
  #undef restore_neutron
  #undef nx
  #undef ny
  #undef nz
  #undef Div_N
  #undef Div_p
  #undef Div_p2
  return;
} /* class_Divergence_monitor_trace */

#pragma acc routine
void class_Div1D_monitor_trace(_class_Div1D_monitor *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define ndiv (_comp->_parameters.ndiv)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define maxdiv (_comp->_parameters.maxdiv)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define vertical (_comp->_parameters.vertical)
  #define Div_N (_comp->_parameters.Div_N)
  #define Div_p (_comp->_parameters.Div_p)
  #define Div_p2 (_comp->_parameters.Div_p2)
  SIG_MESSAGE("[_div_mono_H_trace] component div_mono_H=Div1D_monitor() TRACE [Div1D_monitor:0]");

  int i;
  double div;

  PROP_Z0;
  if (x > xmin && x < xmax && y > ymin && y < ymax) {
    if (!vertical) {
      div = RAD2DEG * atan2 (vx, vz);
    } else {
      div = RAD2DEG * atan2 (vy, vz);
    }
    if (div < (double)maxdiv && div > -(double)maxdiv) {
      i = floor ((div + (double)maxdiv) * ndiv / (2.0 * (double)maxdiv));

      double p2 = p * p;
      #pragma acc atomic
      Div_N[i] = Div_N[i] + 1;

      #pragma acc atomic
      Div_p[i] = Div_p[i] + p;

      #pragma acc atomic
      Div_p2[i] = Div_p2[i] + p2;

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
  #undef ndiv
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef maxdiv
  #undef restore_neutron
  #undef nowritefile
  #undef vertical
  #undef Div_N
  #undef Div_p
  #undef Div_p2
  return;
} /* class_Div1D_monitor_trace */

#pragma acc routine
void class_PSDlin_monitor_trace(_class_PSDlin_monitor *_comp
  , _class_particle *_particle) {
  ABSORBED=SCATTERED=RESTORE=0;
  #define nbins (_comp->_parameters.nbins)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define vertical (_comp->_parameters.vertical)
  #define PSDlin_N (_comp->_parameters.PSDlin_N)
  #define PSDlin_p (_comp->_parameters.PSDlin_p)
  #define PSDlin_p2 (_comp->_parameters.PSDlin_p2)
  SIG_MESSAGE("[_dpsd1_trace] component dpsd1=PSDlin_monitor() TRACE [PSDlin_monitor:0]");

  int i;

  PROP_Z0;
  if (x > xmin && x < xmax && y > ymin && y < ymax) {
    /* Bin number */
    if (!vertical) {
      i = floor (nbins * (x - xmin) / (xmax - xmin));
    } else {
      i = floor (nbins * (y - ymin) / (ymax - ymin));
    }
    if ((i >= nbins) || (i < 0)) {
      printf ("ERROR: (%s) wrong positioning in linear PSD. i= %i \n", NAME_CURRENT_COMP, i);
      exit (1);
    }
    double p2 = p * p;
    #pragma acc atomic
    PSDlin_N[i] = PSDlin_N[i] + 1;
    #pragma acc atomic
    PSDlin_p[i] = PSDlin_p[i] + p;
    #pragma acc atomic
    PSDlin_p2[i] = PSDlin_p2[i] + p2;
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
  #undef nbins
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef nowritefile
  #undef xwidth
  #undef yheight
  #undef restore_neutron
  #undef vertical
  #undef PSDlin_N
  #undef PSDlin_p
  #undef PSDlin_p2
  return;
} /* class_PSDlin_monitor_trace */

#define ncol_25 (_particle->ncol_25)
#define nrow_25 (_particle->nrow_25)
/* *****************************************************************************
* instrument 'HZB_FLEX' TRACE
***************************************************************************** */

#ifndef FUNNEL
#pragma acc routine
int raytrace(_class_particle* _particle) { /* single event propagation, called by mccode_main for HZB_FLEX:TRACE */

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
    /* begin component Origin=Progress_bar() [1] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Origin_var._rotation_is_identity) {
        if(!_Origin_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Origin_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Origin_var._position_relative, _Origin_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 1) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Origin_var._name);
      DEBUG_STATE();
      class_Progress_bar_trace(&_Origin_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Origin [1] */
    /* begin component Gen_Source=Source_gen() [2] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Gen_Source_var._rotation_is_identity) {
        if(!_Gen_Source_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Gen_Source_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Gen_Source_var._position_relative, _Gen_Source_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 2) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Gen_Source_var._name);
      DEBUG_STATE();
      class_Source_gen_trace(&_Gen_Source_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Gen_Source [2] */
    /* begin component NL1A_NL1B_NL2_NL3_InPile_Entrance_Window=Arm() [3] */
    if (!ABSORBED && _particle->_index == 3) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle->_index++;
    } /* end component NL1A_NL1B_NL2_NL3_InPile_Entrance_Window [3] */
    /* begin component NL1A_NL1B_NL2_NL3_InPile=Guide() [4] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_NL1A_NL1B_NL2_NL3_InPile_var._rotation_is_identity) {
        if(!_NL1A_NL1B_NL2_NL3_InPile_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _NL1A_NL1B_NL2_NL3_InPile_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_NL1A_NL1B_NL2_NL3_InPile_var._position_relative, _NL1A_NL1B_NL2_NL3_InPile_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 4) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_NL1A_NL1B_NL2_NL3_InPile_var._name);
      DEBUG_STATE();
      class_Guide_trace(&_NL1A_NL1B_NL2_NL3_InPile_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component NL1A_NL1B_NL2_NL3_InPile [4] */
    /* begin component NL1B_Straight1_Entrance_Window=Arm() [5] */
    if (!ABSORBED && _particle->_index == 5) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle->_index++;
    } /* end component NL1B_Straight1_Entrance_Window [5] */
    /* begin component NL1B_Straight1=Guide() [6] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_NL1B_Straight1_var._rotation_is_identity) {
        if(!_NL1B_Straight1_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _NL1B_Straight1_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_NL1B_Straight1_var._position_relative, _NL1B_Straight1_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 6) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_NL1B_Straight1_var._name);
      DEBUG_STATE();
      class_Guide_trace(&_NL1B_Straight1_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component NL1B_Straight1 [6] */
    /* begin component NL1B_Curved_Entrance_Window=Arm() [7] */
    if (!ABSORBED && _particle->_index == 7) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle->_index++;
    } /* end component NL1B_Curved_Entrance_Window [7] */
    /* begin component NL1B_Curved_Guide_1=Guide_curved() [8] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_NL1B_Curved_Guide_1_var._rotation_is_identity) {
        if(!_NL1B_Curved_Guide_1_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _NL1B_Curved_Guide_1_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_NL1B_Curved_Guide_1_var._position_relative, _NL1B_Curved_Guide_1_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 8) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_NL1B_Curved_Guide_1_var._name);
      DEBUG_STATE();
      class_Guide_curved_trace(&_NL1B_Curved_Guide_1_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component NL1B_Curved_Guide_1 [8] */
    /* begin component NL1B_Velocity_Selector_Gap_Entrance_Window=Arm() [9] */
    if (!ABSORBED && _particle->_index == 9) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle->_index++;
    } /* end component NL1B_Velocity_Selector_Gap_Entrance_Window [9] */
    /* begin component Before_selec=PSD_monitor() [10] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Before_selec_var._rotation_is_identity) {
        if(!_Before_selec_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Before_selec_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Before_selec_var._position_relative, _Before_selec_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 10) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Before_selec_var._name);
      DEBUG_STATE();
      class_PSD_monitor_trace(&_Before_selec_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Before_selec [10] */
    /* begin component SELEC=Selector() [11] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_SELEC_var._rotation_is_identity) {
        if(!_SELEC_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _SELEC_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_SELEC_var._position_relative, _SELEC_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 11) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_SELEC_var._name);
      DEBUG_STATE();
      class_Selector_trace(&_SELEC_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component SELEC [11] */
    /* begin component After_selec=PSD_monitor() [12] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_After_selec_var._rotation_is_identity) {
        if(!_After_selec_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _After_selec_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_After_selec_var._position_relative, _After_selec_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 12) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_After_selec_var._name);
      DEBUG_STATE();
      class_PSD_monitor_trace(&_After_selec_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component After_selec [12] */
    /* begin component NL1B_Curved_2_Entrance_Window=Arm() [13] */
    if (!ABSORBED && _particle->_index == 13) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle->_index++;
    } /* end component NL1B_Curved_2_Entrance_Window [13] */
    /* begin component NL1B_Curved_Guide_2=Guide_curved() [14] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_NL1B_Curved_Guide_2_var._rotation_is_identity) {
        if(!_NL1B_Curved_Guide_2_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _NL1B_Curved_Guide_2_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_NL1B_Curved_Guide_2_var._position_relative, _NL1B_Curved_Guide_2_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 14) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_NL1B_Curved_Guide_2_var._name);
      DEBUG_STATE();
      class_Guide_curved_trace(&_NL1B_Curved_Guide_2_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component NL1B_Curved_Guide_2 [14] */
    /* begin component NL1B_Straight2_Entrance_Window=Arm() [15] */
    if (!ABSORBED && _particle->_index == 15) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle->_index++;
    } /* end component NL1B_Straight2_Entrance_Window [15] */
    /* begin component NL1B_Straight2=Guide() [16] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_NL1B_Straight2_var._rotation_is_identity) {
        if(!_NL1B_Straight2_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _NL1B_Straight2_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_NL1B_Straight2_var._position_relative, _NL1B_Straight2_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 16) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_NL1B_Straight2_var._name);
      DEBUG_STATE();
      class_Guide_trace(&_NL1B_Straight2_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component NL1B_Straight2 [16] */
    /* begin component NL1B_Elliptical_Entrance_Window=Arm() [17] */
    if (!ABSORBED && _particle->_index == 17) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle->_index++;
    } /* end component NL1B_Elliptical_Entrance_Window [17] */
    /* begin component elliptical_piece=Guide_tapering() [18] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_elliptical_piece_var._rotation_is_identity) {
        if(!_elliptical_piece_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _elliptical_piece_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_elliptical_piece_var._position_relative, _elliptical_piece_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 18) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_elliptical_piece_var._name);
      DEBUG_STATE();
      class_Guide_tapering_trace(&_elliptical_piece_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component elliptical_piece [18] */
    /* begin component Virtual_source=Slit() [19] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Virtual_source_var._rotation_is_identity) {
        if(!_Virtual_source_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Virtual_source_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Virtual_source_var._position_relative, _Virtual_source_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 19) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Virtual_source_var._name);
      DEBUG_STATE();
      class_Slit_trace(&_Virtual_source_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Virtual_source [19] */
    /* begin component NL1B_Vertical_Guide_Entrance_Window=Arm() [20] */
    if (!ABSORBED && _particle->_index == 20) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle->_index++;
    } /* end component NL1B_Vertical_Guide_Entrance_Window [20] */
    /* begin component NL1B_Vertical_Guide=Guide_channeled() [21] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_NL1B_Vertical_Guide_var._rotation_is_identity) {
        if(!_NL1B_Vertical_Guide_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _NL1B_Vertical_Guide_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_NL1B_Vertical_Guide_var._position_relative, _NL1B_Vertical_Guide_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 21) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_NL1B_Vertical_Guide_var._name);
      DEBUG_STATE();
      class_Guide_channeled_trace(&_NL1B_Vertical_Guide_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component NL1B_Vertical_Guide [21] */
    /* begin component NL1B_Guide_Exit=Arm() [22] */
    if (!ABSORBED && _particle->_index == 22) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle->_index++;
    } /* end component NL1B_Guide_Exit [22] */
    /* begin component energy_endguide=E_monitor() [23] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_energy_endguide_var._rotation_is_identity) {
        if(!_energy_endguide_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _energy_endguide_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_energy_endguide_var._position_relative, _energy_endguide_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 23) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_energy_endguide_var._name);
      DEBUG_STATE();
      class_E_monitor_trace(&_energy_endguide_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component energy_endguide [23] */
    /* begin component Mono_center=Arm() [24] */
    if (!ABSORBED && _particle->_index == 24) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle->_index++;
    } /* end component Mono_center [24] */
    /* begin component Monochromator=Monochromator_curved() [25] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_Monochromator_var._rotation_is_identity) {
        if(!_Monochromator_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _Monochromator_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_Monochromator_var._position_relative, _Monochromator_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 25) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_Monochromator_var._name);
      DEBUG_STATE();
      class_Monochromator_curved_trace(&_Monochromator_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component Monochromator [25] */
    /* begin component Mono_sample_arm=Arm() [26] */
    if (!ABSORBED && _particle->_index == 26) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle->_index++;
    } /* end component Mono_sample_arm [26] */
    /* begin component energy_mono=E_monitor() [27] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_energy_mono_var._rotation_is_identity) {
        if(!_energy_mono_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _energy_mono_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_energy_mono_var._position_relative, _energy_mono_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 27) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_energy_mono_var._name);
      DEBUG_STATE();
      class_E_monitor_trace(&_energy_mono_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component energy_mono [27] */
    /* begin component energy_pre_sample=E_monitor() [28] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_energy_pre_sample_var._rotation_is_identity) {
        if(!_energy_pre_sample_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _energy_pre_sample_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_energy_pre_sample_var._position_relative, _energy_pre_sample_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 28) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_energy_pre_sample_var._name);
      DEBUG_STATE();
      class_E_monitor_trace(&_energy_pre_sample_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component energy_pre_sample [28] */
    /* begin component Sample_center=Arm() [29] */
    if (!ABSORBED && _particle->_index == 29) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle->_index++;
    } /* end component Sample_center [29] */
    /* begin component Sample_analyser_arm=Arm() [30] */
    if (!ABSORBED && _particle->_index == 30) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle->_index++;
    } /* end component Sample_analyser_arm [30] */
    /* begin component div_mono=Divergence_monitor() [31] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_div_mono_var._rotation_is_identity) {
        if(!_div_mono_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _div_mono_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_div_mono_var._position_relative, _div_mono_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 31) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_div_mono_var._name);
      DEBUG_STATE();
      class_Divergence_monitor_trace(&_div_mono_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component div_mono [31] */
    /* begin component div_mono_H=Div1D_monitor() [32] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_div_mono_H_var._rotation_is_identity) {
        if(!_div_mono_H_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _div_mono_H_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_div_mono_H_var._position_relative, _div_mono_H_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 32) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_div_mono_H_var._name);
      DEBUG_STATE();
      class_Div1D_monitor_trace(&_div_mono_H_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component div_mono_H [32] */
    /* begin component psd_sam=PSD_monitor() [33] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_psd_sam_var._rotation_is_identity) {
        if(!_psd_sam_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _psd_sam_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_psd_sam_var._position_relative, _psd_sam_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 33) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_psd_sam_var._name);
      DEBUG_STATE();
      class_PSD_monitor_trace(&_psd_sam_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component psd_sam [33] */
    /* begin component dpsd1=PSDlin_monitor() [34] */
    if (!_particle->flag_nocoordschange) { // flag activated by JUMP to pass coords change
      if (_dpsd1_var._rotation_is_identity) {
        if(!_dpsd1_var._position_relative_is_zero) {
          coords_get(coords_add(coords_set(x,y,z), _dpsd1_var._position_relative),&x, &y, &z);
        }
      } else {
          mccoordschange(_dpsd1_var._position_relative, _dpsd1_var._rotation_relative, _particle);
      }
    }
    if (!ABSORBED && _particle->_index == 34) {
      _particle->flag_nocoordschange=0; /* Reset if we came here from a JUMP */
      _particle_save = *_particle;
      DEBUG_COMP(_dpsd1_var._name);
      DEBUG_STATE();
      class_PSDlin_monitor_trace(&_dpsd1_var, _particle);
      if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
      _particle->_index++;
      if (!ABSORBED) { DEBUG_STATE(); }
    } /* end component dpsd1 [34] */
    if (_particle->_index > 34)
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

      // Origin
    if (!ABSORBED && _particle->_index == 1) {
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

      // Gen_Source
    if (!ABSORBED && _particle->_index == 2) {
#ifndef MULTICORE
        if (_Gen_Source_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Gen_Source_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Gen_Source_var._position_relative, _Gen_Source_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Source_gen_trace(&_Gen_Source_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // NL1A_NL1B_NL2_NL3_InPile_Entrance_Window
    if (!ABSORBED && _particle->_index == 3) {
        _particle->_index++;
      }

      // NL1A_NL1B_NL2_NL3_InPile
    if (!ABSORBED && _particle->_index == 4) {
#ifndef MULTICORE
        if (_NL1A_NL1B_NL2_NL3_InPile_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _NL1A_NL1B_NL2_NL3_InPile_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_NL1A_NL1B_NL2_NL3_InPile_var._position_relative, _NL1A_NL1B_NL2_NL3_InPile_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Guide_trace(&_NL1A_NL1B_NL2_NL3_InPile_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // NL1B_Straight1_Entrance_Window
    if (!ABSORBED && _particle->_index == 5) {
        _particle->_index++;
      }

      // NL1B_Straight1
    if (!ABSORBED && _particle->_index == 6) {
#ifndef MULTICORE
        if (_NL1B_Straight1_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _NL1B_Straight1_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_NL1B_Straight1_var._position_relative, _NL1B_Straight1_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Guide_trace(&_NL1B_Straight1_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // NL1B_Curved_Entrance_Window
    if (!ABSORBED && _particle->_index == 7) {
        _particle->_index++;
      }

      // NL1B_Curved_Guide_1
    if (!ABSORBED && _particle->_index == 8) {
#ifndef MULTICORE
        if (_NL1B_Curved_Guide_1_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _NL1B_Curved_Guide_1_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_NL1B_Curved_Guide_1_var._position_relative, _NL1B_Curved_Guide_1_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Guide_curved_trace(&_NL1B_Curved_Guide_1_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // NL1B_Velocity_Selector_Gap_Entrance_Window
    if (!ABSORBED && _particle->_index == 9) {
        _particle->_index++;
      }

      // Before_selec
    if (!ABSORBED && _particle->_index == 10) {
#ifndef MULTICORE
        if (_Before_selec_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Before_selec_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Before_selec_var._position_relative, _Before_selec_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_PSD_monitor_trace(&_Before_selec_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // SELEC
    if (!ABSORBED && _particle->_index == 11) {
#ifndef MULTICORE
        if (_SELEC_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _SELEC_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_SELEC_var._position_relative, _SELEC_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Selector_trace(&_SELEC_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // After_selec
    if (!ABSORBED && _particle->_index == 12) {
#ifndef MULTICORE
        if (_After_selec_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _After_selec_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_After_selec_var._position_relative, _After_selec_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_PSD_monitor_trace(&_After_selec_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // NL1B_Curved_2_Entrance_Window
    if (!ABSORBED && _particle->_index == 13) {
        _particle->_index++;
      }

      // NL1B_Curved_Guide_2
    if (!ABSORBED && _particle->_index == 14) {
#ifndef MULTICORE
        if (_NL1B_Curved_Guide_2_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _NL1B_Curved_Guide_2_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_NL1B_Curved_Guide_2_var._position_relative, _NL1B_Curved_Guide_2_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Guide_curved_trace(&_NL1B_Curved_Guide_2_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // NL1B_Straight2_Entrance_Window
    if (!ABSORBED && _particle->_index == 15) {
        _particle->_index++;
      }

      // NL1B_Straight2
    if (!ABSORBED && _particle->_index == 16) {
#ifndef MULTICORE
        if (_NL1B_Straight2_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _NL1B_Straight2_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_NL1B_Straight2_var._position_relative, _NL1B_Straight2_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Guide_trace(&_NL1B_Straight2_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // NL1B_Elliptical_Entrance_Window
    if (!ABSORBED && _particle->_index == 17) {
        _particle->_index++;
      }

      // elliptical_piece
    if (!ABSORBED && _particle->_index == 18) {
#ifndef MULTICORE
        if (_elliptical_piece_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _elliptical_piece_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_elliptical_piece_var._position_relative, _elliptical_piece_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Guide_tapering_trace(&_elliptical_piece_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Virtual_source
    if (!ABSORBED && _particle->_index == 19) {
#ifndef MULTICORE
        if (_Virtual_source_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Virtual_source_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Virtual_source_var._position_relative, _Virtual_source_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Slit_trace(&_Virtual_source_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // NL1B_Vertical_Guide_Entrance_Window
    if (!ABSORBED && _particle->_index == 20) {
        _particle->_index++;
      }

      // NL1B_Vertical_Guide
    if (!ABSORBED && _particle->_index == 21) {
#ifndef MULTICORE
        if (_NL1B_Vertical_Guide_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _NL1B_Vertical_Guide_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_NL1B_Vertical_Guide_var._position_relative, _NL1B_Vertical_Guide_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Guide_channeled_trace(&_NL1B_Vertical_Guide_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // NL1B_Guide_Exit
    if (!ABSORBED && _particle->_index == 22) {
        _particle->_index++;
      }

      // energy_endguide
    if (!ABSORBED && _particle->_index == 23) {
#ifndef MULTICORE
        if (_energy_endguide_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _energy_endguide_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_energy_endguide_var._position_relative, _energy_endguide_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_E_monitor_trace(&_energy_endguide_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Mono_center
    if (!ABSORBED && _particle->_index == 24) {
        _particle->_index++;
      }

      // Monochromator
    if (!ABSORBED && _particle->_index == 25) {
#ifndef MULTICORE
        if (_Monochromator_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _Monochromator_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_Monochromator_var._position_relative, _Monochromator_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Monochromator_curved_trace(&_Monochromator_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Mono_sample_arm
    if (!ABSORBED && _particle->_index == 26) {
        _particle->_index++;
      }

      // energy_mono
    if (!ABSORBED && _particle->_index == 27) {
#ifndef MULTICORE
        if (_energy_mono_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _energy_mono_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_energy_mono_var._position_relative, _energy_mono_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_E_monitor_trace(&_energy_mono_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // energy_pre_sample
    if (!ABSORBED && _particle->_index == 28) {
#ifndef MULTICORE
        if (_energy_pre_sample_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _energy_pre_sample_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_energy_pre_sample_var._position_relative, _energy_pre_sample_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_E_monitor_trace(&_energy_pre_sample_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // Sample_center
    if (!ABSORBED && _particle->_index == 29) {
        _particle->_index++;
      }

      // Sample_analyser_arm
    if (!ABSORBED && _particle->_index == 30) {
        _particle->_index++;
      }

      // div_mono
    if (!ABSORBED && _particle->_index == 31) {
#ifndef MULTICORE
        if (_div_mono_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _div_mono_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_div_mono_var._position_relative, _div_mono_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Divergence_monitor_trace(&_div_mono_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // div_mono_H
    if (!ABSORBED && _particle->_index == 32) {
#ifndef MULTICORE
        if (_div_mono_H_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _div_mono_H_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_div_mono_H_var._position_relative, _div_mono_H_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_Div1D_monitor_trace(&_div_mono_H_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // psd_sam
    if (!ABSORBED && _particle->_index == 33) {
#ifndef MULTICORE
        if (_psd_sam_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _psd_sam_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_psd_sam_var._position_relative, _psd_sam_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_PSD_monitor_trace(&_psd_sam_var, _particle);
        if (_particle->_restore)
        particle_restore(_particle, &_particle_save);
        _particle->_index++;
      }

      // dpsd1
    if (!ABSORBED && _particle->_index == 34) {
#ifndef MULTICORE
        if (_dpsd1_var._rotation_is_identity)
          coords_get(coords_add(coords_set(x,y,z), _dpsd1_var._position_relative),&x, &y, &z);
        else
#endif
          mccoordschange(_dpsd1_var._position_relative, _dpsd1_var._rotation_relative, _particle);
        _particle_save = *_particle;
        class_PSDlin_monitor_trace(&_dpsd1_var, _particle);
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

#undef ncol_25
#undef nrow_25
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
* instrument 'HZB_FLEX' and components SAVE
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
  SIG_MESSAGE("[_Before_selec_save] component Before_selec=PSD_monitor() SAVE [PSD_monitor:0]");

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
  SIG_MESSAGE("[_energy_endguide_save] component energy_endguide=E_monitor() SAVE [E_monitor:0]");

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

_class_Divergence_monitor *class_Divergence_monitor_save(_class_Divergence_monitor *_comp
) {
  #define nh (_comp->_parameters.nh)
  #define nv (_comp->_parameters.nv)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define maxdiv_h (_comp->_parameters.maxdiv_h)
  #define maxdiv_v (_comp->_parameters.maxdiv_v)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nx (_comp->_parameters.nx)
  #define ny (_comp->_parameters.ny)
  #define nz (_comp->_parameters.nz)
  #define Div_N (_comp->_parameters.Div_N)
  #define Div_p (_comp->_parameters.Div_p)
  #define Div_p2 (_comp->_parameters.Div_p2)
  SIG_MESSAGE("[_div_mono_save] component div_mono=Divergence_monitor() SAVE [Divergence_monitor:0]");

  if (!nowritefile) {
    DETECTOR_OUT_2D ("Divergence monitor", "X divergence [deg]", "Y divergence [deg]", -maxdiv_h, maxdiv_h, -maxdiv_v, maxdiv_v, nh, nv, &Div_N[0][0],
                     &Div_p[0][0], &Div_p2[0][0], filename);
  }
  #undef nh
  #undef nv
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef nowritefile
  #undef xwidth
  #undef yheight
  #undef maxdiv_h
  #undef maxdiv_v
  #undef restore_neutron
  #undef nx
  #undef ny
  #undef nz
  #undef Div_N
  #undef Div_p
  #undef Div_p2
  return(_comp);
} /* class_Divergence_monitor_save */

_class_Div1D_monitor *class_Div1D_monitor_save(_class_Div1D_monitor *_comp
) {
  #define ndiv (_comp->_parameters.ndiv)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define maxdiv (_comp->_parameters.maxdiv)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define vertical (_comp->_parameters.vertical)
  #define Div_N (_comp->_parameters.Div_N)
  #define Div_p (_comp->_parameters.Div_p)
  #define Div_p2 (_comp->_parameters.Div_p2)
  SIG_MESSAGE("[_div_mono_H_save] component div_mono_H=Div1D_monitor() SAVE [Div1D_monitor:0]");

  if (!nowritefile) {
    if (!vertical) {
      DETECTOR_OUT_1D ("Horizontal divergence monitor", "horizontal divergence [deg]", "Intensity", "divergence", -maxdiv, maxdiv, ndiv, &Div_N[0], &Div_p[0],
                       &Div_p2[0], filename);
    } else {
      DETECTOR_OUT_1D ("Vertical divergence monitor", "vertical divergence [deg]", "Intensity", "divergence", -maxdiv, maxdiv, ndiv, &Div_N[0], &Div_p[0],
                       &Div_p2[0], filename);
    }
  }
  #undef ndiv
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef maxdiv
  #undef restore_neutron
  #undef nowritefile
  #undef vertical
  #undef Div_N
  #undef Div_p
  #undef Div_p2
  return(_comp);
} /* class_Div1D_monitor_save */

_class_PSDlin_monitor *class_PSDlin_monitor_save(_class_PSDlin_monitor *_comp
) {
  #define nbins (_comp->_parameters.nbins)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define vertical (_comp->_parameters.vertical)
  #define PSDlin_N (_comp->_parameters.PSDlin_N)
  #define PSDlin_p (_comp->_parameters.PSDlin_p)
  #define PSDlin_p2 (_comp->_parameters.PSDlin_p2)
  SIG_MESSAGE("[_dpsd1_save] component dpsd1=PSDlin_monitor() SAVE [PSDlin_monitor:0]");

  if (!nowritefile) {
    if (!vertical) {
      DETECTOR_OUT_1D ("Linear PSD monitor", "x-Position [m]", "Intensity", "x", xmin, xmax, nbins, &PSDlin_N[0], &PSDlin_p[0], &PSDlin_p2[0], filename);
    } else {
      DETECTOR_OUT_1D ("Linear PSD monitor", "y-Position [m]", "Intensity", "y", ymin, ymax, nbins, &PSDlin_N[0], &PSDlin_p[0], &PSDlin_p2[0], filename);
    }
  }
  #undef nbins
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef nowritefile
  #undef xwidth
  #undef yheight
  #undef restore_neutron
  #undef vertical
  #undef PSDlin_N
  #undef PSDlin_p
  #undef PSDlin_p2
  return(_comp);
} /* class_PSDlin_monitor_save */



int save(FILE *handle) { /* called by mccode_main for HZB_FLEX:SAVE */
  if (!handle) siminfo_init(NULL);

  /* call iteratively all components SAVE */
  class_Progress_bar_save(&_Origin_var);









  class_PSD_monitor_save(&_Before_selec_var);


  class_PSD_monitor_save(&_After_selec_var);











  class_E_monitor_save(&_energy_endguide_var);




  class_E_monitor_save(&_energy_mono_var);

  class_E_monitor_save(&_energy_pre_sample_var);



  class_Divergence_monitor_save(&_div_mono_var);

  class_Div1D_monitor_save(&_div_mono_H_var);

  class_PSD_monitor_save(&_psd_sam_var);

  class_PSDlin_monitor_save(&_dpsd1_var);

  if (!handle) siminfo_close(); 

  return(0);
} /* save */

/* *****************************************************************************
* instrument 'HZB_FLEX' and components FINALLY
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

_class_Source_gen *class_Source_gen_finally(_class_Source_gen *_comp
) {
  #define flux_file (_comp->_parameters.flux_file)
  #define xdiv_file (_comp->_parameters.xdiv_file)
  #define ydiv_file (_comp->_parameters.ydiv_file)
  #define radius (_comp->_parameters.radius)
  #define dist (_comp->_parameters.dist)
  #define focus_xw (_comp->_parameters.focus_xw)
  #define focus_yh (_comp->_parameters.focus_yh)
  #define focus_aw (_comp->_parameters.focus_aw)
  #define focus_ah (_comp->_parameters.focus_ah)
  #define E0 (_comp->_parameters.E0)
  #define dE (_comp->_parameters.dE)
  #define lambda0 (_comp->_parameters.lambda0)
  #define dlambda (_comp->_parameters.dlambda)
  #define I1 (_comp->_parameters.I1)
  #define yheight (_comp->_parameters.yheight)
  #define xwidth (_comp->_parameters.xwidth)
  #define verbose (_comp->_parameters.verbose)
  #define T1 (_comp->_parameters.T1)
  #define flux_file_perAA (_comp->_parameters.flux_file_perAA)
  #define flux_file_log (_comp->_parameters.flux_file_log)
  #define Lmin (_comp->_parameters.Lmin)
  #define Lmax (_comp->_parameters.Lmax)
  #define Emin (_comp->_parameters.Emin)
  #define Emax (_comp->_parameters.Emax)
  #define T2 (_comp->_parameters.T2)
  #define I2 (_comp->_parameters.I2)
  #define T3 (_comp->_parameters.T3)
  #define I3 (_comp->_parameters.I3)
  #define zdepth (_comp->_parameters.zdepth)
  #define target_index (_comp->_parameters.target_index)
  #define p_in (_comp->_parameters.p_in)
  #define lambda1 (_comp->_parameters.lambda1)
  #define lambda2 (_comp->_parameters.lambda2)
  #define lambda3 (_comp->_parameters.lambda3)
  #define pTable (_comp->_parameters.pTable)
  #define pTable_x (_comp->_parameters.pTable_x)
  #define pTable_y (_comp->_parameters.pTable_y)
  #define pTable_xmin (_comp->_parameters.pTable_xmin)
  #define pTable_xmax (_comp->_parameters.pTable_xmax)
  #define pTable_xsum (_comp->_parameters.pTable_xsum)
  #define pTable_ymin (_comp->_parameters.pTable_ymin)
  #define pTable_ymax (_comp->_parameters.pTable_ymax)
  #define pTable_ysum (_comp->_parameters.pTable_ysum)
  #define pTable_dxmin (_comp->_parameters.pTable_dxmin)
  #define pTable_dxmax (_comp->_parameters.pTable_dxmax)
  #define pTable_dymin (_comp->_parameters.pTable_dymin)
  #define pTable_dymax (_comp->_parameters.pTable_dymax)
  SIG_MESSAGE("[_Gen_Source_finally] component Gen_Source=Source_gen() FINALLY [Source_gen:0]");

  Table_Free(&pTable);
  Table_Free(&pTable_x);
  Table_Free(&pTable_y);
  #undef flux_file
  #undef xdiv_file
  #undef ydiv_file
  #undef radius
  #undef dist
  #undef focus_xw
  #undef focus_yh
  #undef focus_aw
  #undef focus_ah
  #undef E0
  #undef dE
  #undef lambda0
  #undef dlambda
  #undef I1
  #undef yheight
  #undef xwidth
  #undef verbose
  #undef T1
  #undef flux_file_perAA
  #undef flux_file_log
  #undef Lmin
  #undef Lmax
  #undef Emin
  #undef Emax
  #undef T2
  #undef I2
  #undef T3
  #undef I3
  #undef zdepth
  #undef target_index
  #undef p_in
  #undef lambda1
  #undef lambda2
  #undef lambda3
  #undef pTable
  #undef pTable_x
  #undef pTable_y
  #undef pTable_xmin
  #undef pTable_xmax
  #undef pTable_xsum
  #undef pTable_ymin
  #undef pTable_ymax
  #undef pTable_ysum
  #undef pTable_dxmin
  #undef pTable_dxmax
  #undef pTable_dymin
  #undef pTable_dymax
  return(_comp);
} /* class_Source_gen_finally */

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
  SIG_MESSAGE("[_Before_selec_finally] component Before_selec=PSD_monitor() FINALLY [PSD_monitor:0]");

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

_class_Guide_tapering *class_Guide_tapering_finally(_class_Guide_tapering *_comp
) {
  #define option (_comp->_parameters.option)
  #define w1 (_comp->_parameters.w1)
  #define h1 (_comp->_parameters.h1)
  #define l (_comp->_parameters.l)
  #define linw (_comp->_parameters.linw)
  #define loutw (_comp->_parameters.loutw)
  #define linh (_comp->_parameters.linh)
  #define louth (_comp->_parameters.louth)
  #define R0 (_comp->_parameters.R0)
  #define Qcx (_comp->_parameters.Qcx)
  #define Qcy (_comp->_parameters.Qcy)
  #define alphax (_comp->_parameters.alphax)
  #define alphay (_comp->_parameters.alphay)
  #define W (_comp->_parameters.W)
  #define mx (_comp->_parameters.mx)
  #define my (_comp->_parameters.my)
  #define segno (_comp->_parameters.segno)
  #define curvature (_comp->_parameters.curvature)
  #define curvature_v (_comp->_parameters.curvature_v)
  #define w1c (_comp->_parameters.w1c)
  #define w2c (_comp->_parameters.w2c)
  #define ww (_comp->_parameters.ww)
  #define hh (_comp->_parameters.hh)
  #define whalf (_comp->_parameters.whalf)
  #define hhalf (_comp->_parameters.hhalf)
  #define lwhalf (_comp->_parameters.lwhalf)
  #define lhhalf (_comp->_parameters.lhhalf)
  #define h1_in (_comp->_parameters.h1_in)
  #define h2_out (_comp->_parameters.h2_out)
  #define w1_in (_comp->_parameters.w1_in)
  #define w2_out (_comp->_parameters.w2_out)
  #define l_seg (_comp->_parameters.l_seg)
  #define h12 (_comp->_parameters.h12)
  #define h2 (_comp->_parameters.h2)
  #define w12 (_comp->_parameters.w12)
  #define w2 (_comp->_parameters.w2)
  #define a_ell_q (_comp->_parameters.a_ell_q)
  #define b_ell_q (_comp->_parameters.b_ell_q)
  #define lbw (_comp->_parameters.lbw)
  #define lbh (_comp->_parameters.lbh)
  #define mxi (_comp->_parameters.mxi)
  #define u1 (_comp->_parameters.u1)
  #define u2 (_comp->_parameters.u2)
  #define div1 (_comp->_parameters.div1)
  #define p2_para (_comp->_parameters.p2_para)
  #define test (_comp->_parameters.test)
  #define Div1 (_comp->_parameters.Div1)
  #define seg (_comp->_parameters.seg)
  #define fu (_comp->_parameters.fu)
  #define pos (_comp->_parameters.pos)
  #define file_name (_comp->_parameters.file_name)
  #define ep (_comp->_parameters.ep)
  #define num (_comp->_parameters.num)
  #define rotation_h (_comp->_parameters.rotation_h)
  #define rotation_v (_comp->_parameters.rotation_v)
  SIG_MESSAGE("[_elliptical_piece_finally] component elliptical_piece=Guide_tapering() FINALLY [Guide_tapering:0]");

  free (w1c);
  free (w2c);
  free (ww);
  free (hh);
  free (whalf);
  free (hhalf);
  free (lwhalf);
  free (lhhalf);
  free (h1_in);
  free (h2_out);
  free (w1_in);
  free (w2_out);
  #undef option
  #undef w1
  #undef h1
  #undef l
  #undef linw
  #undef loutw
  #undef linh
  #undef louth
  #undef R0
  #undef Qcx
  #undef Qcy
  #undef alphax
  #undef alphay
  #undef W
  #undef mx
  #undef my
  #undef segno
  #undef curvature
  #undef curvature_v
  #undef w1c
  #undef w2c
  #undef ww
  #undef hh
  #undef whalf
  #undef hhalf
  #undef lwhalf
  #undef lhhalf
  #undef h1_in
  #undef h2_out
  #undef w1_in
  #undef w2_out
  #undef l_seg
  #undef h12
  #undef h2
  #undef w12
  #undef w2
  #undef a_ell_q
  #undef b_ell_q
  #undef lbw
  #undef lbh
  #undef mxi
  #undef u1
  #undef u2
  #undef div1
  #undef p2_para
  #undef test
  #undef Div1
  #undef seg
  #undef fu
  #undef pos
  #undef file_name
  #undef ep
  #undef num
  #undef rotation_h
  #undef rotation_v
  return(_comp);
} /* class_Guide_tapering_finally */

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
  SIG_MESSAGE("[_energy_endguide_finally] component energy_endguide=E_monitor() FINALLY [E_monitor:0]");

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

_class_Monochromator_curved *class_Monochromator_curved_finally(_class_Monochromator_curved *_comp
) {
  #define reflect (_comp->_parameters.reflect)
  #define transmit (_comp->_parameters.transmit)
  #define zwidth (_comp->_parameters.zwidth)
  #define yheight (_comp->_parameters.yheight)
  #define gap (_comp->_parameters.gap)
  #define NH (_comp->_parameters.NH)
  #define NV (_comp->_parameters.NV)
  #define mosaich (_comp->_parameters.mosaich)
  #define mosaicv (_comp->_parameters.mosaicv)
  #define r0 (_comp->_parameters.r0)
  #define t0 (_comp->_parameters.t0)
  #define Q (_comp->_parameters.Q)
  #define RV (_comp->_parameters.RV)
  #define RH (_comp->_parameters.RH)
  #define DM (_comp->_parameters.DM)
  #define mosaic (_comp->_parameters.mosaic)
  #define width (_comp->_parameters.width)
  #define height (_comp->_parameters.height)
  #define verbose (_comp->_parameters.verbose)
  #define order (_comp->_parameters.order)
  #define mos_rms_y (_comp->_parameters.mos_rms_y)
  #define mos_rms_z (_comp->_parameters.mos_rms_z)
  #define mos_rms_max (_comp->_parameters.mos_rms_max)
  #define mono_Q (_comp->_parameters.mono_Q)
  #define SlabWidth (_comp->_parameters.SlabWidth)
  #define SlabHeight (_comp->_parameters.SlabHeight)
  #define rTable (_comp->_parameters.rTable)
  #define tTable (_comp->_parameters.tTable)
  #define rTableFlag (_comp->_parameters.rTableFlag)
  #define tTableFlag (_comp->_parameters.tTableFlag)
  #define tiltH (_comp->_parameters.tiltH)
  #define tiltV (_comp->_parameters.tiltV)
  #define ncol_var (_comp->_parameters.ncol_var)
  #define nrow_var (_comp->_parameters.nrow_var)
  SIG_MESSAGE("[_Monochromator_finally] component Monochromator=Monochromator_curved() FINALLY [Monochromator_curved:0]");

  if (rTableFlag) {
    Table_Free (&rTable);
  }
  if (tTableFlag) {
    Table_Free (&tTable);
  }
  if (tiltH)
    free (tiltH);
  if (tiltV)
    free (tiltV);
  #undef reflect
  #undef transmit
  #undef zwidth
  #undef yheight
  #undef gap
  #undef NH
  #undef NV
  #undef mosaich
  #undef mosaicv
  #undef r0
  #undef t0
  #undef Q
  #undef RV
  #undef RH
  #undef DM
  #undef mosaic
  #undef width
  #undef height
  #undef verbose
  #undef order
  #undef mos_rms_y
  #undef mos_rms_z
  #undef mos_rms_max
  #undef mono_Q
  #undef SlabWidth
  #undef SlabHeight
  #undef rTable
  #undef tTable
  #undef rTableFlag
  #undef tTableFlag
  #undef tiltH
  #undef tiltV
  #undef ncol_var
  #undef nrow_var
  return(_comp);
} /* class_Monochromator_curved_finally */

_class_Divergence_monitor *class_Divergence_monitor_finally(_class_Divergence_monitor *_comp
) {
  #define nh (_comp->_parameters.nh)
  #define nv (_comp->_parameters.nv)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define maxdiv_h (_comp->_parameters.maxdiv_h)
  #define maxdiv_v (_comp->_parameters.maxdiv_v)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nx (_comp->_parameters.nx)
  #define ny (_comp->_parameters.ny)
  #define nz (_comp->_parameters.nz)
  #define Div_N (_comp->_parameters.Div_N)
  #define Div_p (_comp->_parameters.Div_p)
  #define Div_p2 (_comp->_parameters.Div_p2)
  SIG_MESSAGE("[_div_mono_finally] component div_mono=Divergence_monitor() FINALLY [Divergence_monitor:0]");

  destroy_darr2d (Div_N);
  destroy_darr2d (Div_p);
  destroy_darr2d (Div_p2);
  #undef nh
  #undef nv
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef nowritefile
  #undef xwidth
  #undef yheight
  #undef maxdiv_h
  #undef maxdiv_v
  #undef restore_neutron
  #undef nx
  #undef ny
  #undef nz
  #undef Div_N
  #undef Div_p
  #undef Div_p2
  return(_comp);
} /* class_Divergence_monitor_finally */

_class_Div1D_monitor *class_Div1D_monitor_finally(_class_Div1D_monitor *_comp
) {
  #define ndiv (_comp->_parameters.ndiv)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define maxdiv (_comp->_parameters.maxdiv)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define vertical (_comp->_parameters.vertical)
  #define Div_N (_comp->_parameters.Div_N)
  #define Div_p (_comp->_parameters.Div_p)
  #define Div_p2 (_comp->_parameters.Div_p2)
  SIG_MESSAGE("[_div_mono_H_finally] component div_mono_H=Div1D_monitor() FINALLY [Div1D_monitor:0]");

  destroy_darr1d (Div_N);
  destroy_darr1d (Div_p);
  destroy_darr1d (Div_p2);
  #undef ndiv
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef maxdiv
  #undef restore_neutron
  #undef nowritefile
  #undef vertical
  #undef Div_N
  #undef Div_p
  #undef Div_p2
  return(_comp);
} /* class_Div1D_monitor_finally */

_class_PSDlin_monitor *class_PSDlin_monitor_finally(_class_PSDlin_monitor *_comp
) {
  #define nbins (_comp->_parameters.nbins)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define vertical (_comp->_parameters.vertical)
  #define PSDlin_N (_comp->_parameters.PSDlin_N)
  #define PSDlin_p (_comp->_parameters.PSDlin_p)
  #define PSDlin_p2 (_comp->_parameters.PSDlin_p2)
  SIG_MESSAGE("[_dpsd1_finally] component dpsd1=PSDlin_monitor() FINALLY [PSDlin_monitor:0]");

  destroy_darr1d (PSDlin_N);
  destroy_darr1d (PSDlin_p);
  destroy_darr1d (PSDlin_p2);
  #undef nbins
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef nowritefile
  #undef xwidth
  #undef yheight
  #undef restore_neutron
  #undef vertical
  #undef PSDlin_N
  #undef PSDlin_p
  #undef PSDlin_p2
  return(_comp);
} /* class_PSDlin_monitor_finally */



int finally(void) { /* called by mccode_main for HZB_FLEX:FINALLY */
#pragma acc update host(_Origin_var)
#pragma acc update host(_Gen_Source_var)
#pragma acc update host(_NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var)
#pragma acc update host(_NL1A_NL1B_NL2_NL3_InPile_var)
#pragma acc update host(_NL1B_Straight1_Entrance_Window_var)
#pragma acc update host(_NL1B_Straight1_var)
#pragma acc update host(_NL1B_Curved_Entrance_Window_var)
#pragma acc update host(_NL1B_Curved_Guide_1_var)
#pragma acc update host(_NL1B_Velocity_Selector_Gap_Entrance_Window_var)
#pragma acc update host(_Before_selec_var)
#pragma acc update host(_SELEC_var)
#pragma acc update host(_After_selec_var)
#pragma acc update host(_NL1B_Curved_2_Entrance_Window_var)
#pragma acc update host(_NL1B_Curved_Guide_2_var)
#pragma acc update host(_NL1B_Straight2_Entrance_Window_var)
#pragma acc update host(_NL1B_Straight2_var)
#pragma acc update host(_NL1B_Elliptical_Entrance_Window_var)
#pragma acc update host(_elliptical_piece_var)
#pragma acc update host(_Virtual_source_var)
#pragma acc update host(_NL1B_Vertical_Guide_Entrance_Window_var)
#pragma acc update host(_NL1B_Vertical_Guide_var)
#pragma acc update host(_NL1B_Guide_Exit_var)
#pragma acc update host(_energy_endguide_var)
#pragma acc update host(_Mono_center_var)
#pragma acc update host(_Monochromator_var)
#pragma acc update host(_Mono_sample_arm_var)
#pragma acc update host(_energy_mono_var)
#pragma acc update host(_energy_pre_sample_var)
#pragma acc update host(_Sample_center_var)
#pragma acc update host(_Sample_analyser_arm_var)
#pragma acc update host(_div_mono_var)
#pragma acc update host(_div_mono_H_var)
#pragma acc update host(_psd_sam_var)
#pragma acc update host(_dpsd1_var)
#pragma acc update host(_instrument_var)

  siminfo_init(NULL);
  save(siminfo_file); /* save data when simulation ends */

  /* call iteratively all components FINALLY */
  class_Progress_bar_finally(&_Origin_var);

  class_Source_gen_finally(&_Gen_Source_var);








  class_PSD_monitor_finally(&_Before_selec_var);


  class_PSD_monitor_finally(&_After_selec_var);






  class_Guide_tapering_finally(&_elliptical_piece_var);





  class_E_monitor_finally(&_energy_endguide_var);


  class_Monochromator_curved_finally(&_Monochromator_var);


  class_E_monitor_finally(&_energy_mono_var);

  class_E_monitor_finally(&_energy_pre_sample_var);



  class_Divergence_monitor_finally(&_div_mono_var);

  class_Div1D_monitor_finally(&_div_mono_H_var);

  class_PSD_monitor_finally(&_psd_sam_var);

  class_PSDlin_monitor_finally(&_dpsd1_var);

  siminfo_close(); 

  return(0);
} /* finally */

/* *****************************************************************************
* instrument 'HZB_FLEX' and components DISPLAY
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

_class_Source_gen *class_Source_gen_display(_class_Source_gen *_comp
) {
  #define flux_file (_comp->_parameters.flux_file)
  #define xdiv_file (_comp->_parameters.xdiv_file)
  #define ydiv_file (_comp->_parameters.ydiv_file)
  #define radius (_comp->_parameters.radius)
  #define dist (_comp->_parameters.dist)
  #define focus_xw (_comp->_parameters.focus_xw)
  #define focus_yh (_comp->_parameters.focus_yh)
  #define focus_aw (_comp->_parameters.focus_aw)
  #define focus_ah (_comp->_parameters.focus_ah)
  #define E0 (_comp->_parameters.E0)
  #define dE (_comp->_parameters.dE)
  #define lambda0 (_comp->_parameters.lambda0)
  #define dlambda (_comp->_parameters.dlambda)
  #define I1 (_comp->_parameters.I1)
  #define yheight (_comp->_parameters.yheight)
  #define xwidth (_comp->_parameters.xwidth)
  #define verbose (_comp->_parameters.verbose)
  #define T1 (_comp->_parameters.T1)
  #define flux_file_perAA (_comp->_parameters.flux_file_perAA)
  #define flux_file_log (_comp->_parameters.flux_file_log)
  #define Lmin (_comp->_parameters.Lmin)
  #define Lmax (_comp->_parameters.Lmax)
  #define Emin (_comp->_parameters.Emin)
  #define Emax (_comp->_parameters.Emax)
  #define T2 (_comp->_parameters.T2)
  #define I2 (_comp->_parameters.I2)
  #define T3 (_comp->_parameters.T3)
  #define I3 (_comp->_parameters.I3)
  #define zdepth (_comp->_parameters.zdepth)
  #define target_index (_comp->_parameters.target_index)
  #define p_in (_comp->_parameters.p_in)
  #define lambda1 (_comp->_parameters.lambda1)
  #define lambda2 (_comp->_parameters.lambda2)
  #define lambda3 (_comp->_parameters.lambda3)
  #define pTable (_comp->_parameters.pTable)
  #define pTable_x (_comp->_parameters.pTable_x)
  #define pTable_y (_comp->_parameters.pTable_y)
  #define pTable_xmin (_comp->_parameters.pTable_xmin)
  #define pTable_xmax (_comp->_parameters.pTable_xmax)
  #define pTable_xsum (_comp->_parameters.pTable_xsum)
  #define pTable_ymin (_comp->_parameters.pTable_ymin)
  #define pTable_ymax (_comp->_parameters.pTable_ymax)
  #define pTable_ysum (_comp->_parameters.pTable_ysum)
  #define pTable_dxmin (_comp->_parameters.pTable_dxmin)
  #define pTable_dxmax (_comp->_parameters.pTable_dxmax)
  #define pTable_dymin (_comp->_parameters.pTable_dymin)
  #define pTable_dymax (_comp->_parameters.pTable_dymax)
  SIG_MESSAGE("[_Gen_Source_display] component Gen_Source=Source_gen() DISPLAY [Source_gen:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);
  double xmin;
  double xmax;
  double ymin;
  double ymax;

  if (radius)
  {
    
    circle("xy",0,0,0,radius);
    if (zdepth) {
      circle("xy",0,0,-zdepth/2,radius);
      circle("xy",0,0, zdepth/2,radius);
    }
  }
  else
  {
    xmin = -xwidth/2; xmax = xwidth/2;
    ymin = -yheight/2; ymax = yheight/2;

    
    multiline(5, (double)xmin, (double)ymin, 0.0,
             (double)xmax, (double)ymin, 0.0,
             (double)xmax, (double)ymax, 0.0,
             (double)xmin, (double)ymax, 0.0,
             (double)xmin, (double)ymin, 0.0);
    if (zdepth) {
      multiline(5, (double)xmin, (double)ymin, -zdepth/2,
             (double)xmax, (double)ymin, -zdepth/2,
             (double)xmax, (double)ymax, -zdepth/2,
             (double)xmin, (double)ymax, -zdepth/2,
             (double)xmin, (double)ymin, -zdepth/2);
      multiline(5, (double)xmin, (double)ymin, zdepth/2,
             (double)xmax, (double)ymin, zdepth/2,
             (double)xmax, (double)ymax, zdepth/2,
             (double)xmin, (double)ymax, zdepth/2,
             (double)xmin, (double)ymin, zdepth/2);
    }
  }
  if (dist) {
    if (focus_aw) focus_xw=dist*tan(focus_aw*DEG2RAD);
    if (focus_ah) focus_yh=dist*tan(focus_ah*DEG2RAD);
    dashed_line(0,0,0, -focus_xw/2,-focus_yh/2,dist, 4);
    dashed_line(0,0,0,  focus_xw/2,-focus_yh/2,dist, 4);
    dashed_line(0,0,0,  focus_xw/2, focus_yh/2,dist, 4);
    dashed_line(0,0,0, -focus_xw/2, focus_yh/2,dist, 4);
  }
  #undef flux_file
  #undef xdiv_file
  #undef ydiv_file
  #undef radius
  #undef dist
  #undef focus_xw
  #undef focus_yh
  #undef focus_aw
  #undef focus_ah
  #undef E0
  #undef dE
  #undef lambda0
  #undef dlambda
  #undef I1
  #undef yheight
  #undef xwidth
  #undef verbose
  #undef T1
  #undef flux_file_perAA
  #undef flux_file_log
  #undef Lmin
  #undef Lmax
  #undef Emin
  #undef Emax
  #undef T2
  #undef I2
  #undef T3
  #undef I3
  #undef zdepth
  #undef target_index
  #undef p_in
  #undef lambda1
  #undef lambda2
  #undef lambda3
  #undef pTable
  #undef pTable_x
  #undef pTable_y
  #undef pTable_xmin
  #undef pTable_xmax
  #undef pTable_xsum
  #undef pTable_ymin
  #undef pTable_ymax
  #undef pTable_ysum
  #undef pTable_dxmin
  #undef pTable_dxmax
  #undef pTable_dymin
  #undef pTable_dymax
  return(_comp);
} /* class_Source_gen_display */

_class_Arm *class_Arm_display(_class_Arm *_comp
) {
  SIG_MESSAGE("[_NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_display] component NL1A_NL1B_NL2_NL3_InPile_Entrance_Window=Arm() DISPLAY [Arm:0]");

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
  SIG_MESSAGE("[_NL1A_NL1B_NL2_NL3_InPile_display] component NL1A_NL1B_NL2_NL3_InPile=Guide() DISPLAY [Guide:0]");

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

_class_Guide_curved *class_Guide_curved_display(_class_Guide_curved *_comp
) {
  #define w1 (_comp->_parameters.w1)
  #define h1 (_comp->_parameters.h1)
  #define l (_comp->_parameters.l)
  #define R0 (_comp->_parameters.R0)
  #define Qc (_comp->_parameters.Qc)
  #define alpha (_comp->_parameters.alpha)
  #define m (_comp->_parameters.m)
  #define W (_comp->_parameters.W)
  #define curvature (_comp->_parameters.curvature)
  SIG_MESSAGE("[_NL1B_Curved_Guide_1_display] component NL1B_Curved_Guide_1=Guide_curved() DISPLAY [Guide_curved:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);
  double x1, x2, z1, z2;
  double xplot1[100], xplot2[100], zplot1[100], zplot2[100];
  int n = 100;
  int j = 1;
  double R1 = (curvature - 0.5 * w1); /* radius of inside arc */
  double R2 = (curvature + 0.5 * w1); /* radius of outside arc */

  for (j = 0; j < n; j++) {
    z1 = ((double)j) * (R1 * l / curvature) / (double)(n - 1);
    z2 = ((double)j) * (R2 * l / curvature) / (double)(n - 1);
    x1 = curvature - sqrt (R1 * R1 - z1 * z1);
    x2 = curvature - sqrt (R2 * R2 - z2 * z2);
    xplot1[j] = x1;
    xplot2[j] = x2;
    zplot1[j] = z1;
    zplot2[j] = z2;
  }
  line (xplot1[0], 0.5 * h1, zplot1[0], xplot2[0], 0.5 * h1, zplot2[0]);
  line (xplot1[0], 0.5 * h1, zplot1[0], xplot1[0], -0.5 * h1, zplot1[0]);
  line (xplot2[0], -0.5 * h1, zplot2[0], xplot2[0], 0.5 * h1, zplot2[0]);
  line (xplot1[0], -0.5 * h1, zplot1[0], xplot2[0], -0.5 * h1, zplot2[0]);
  for (j = 0; j < n - 1; j++) {
    line (xplot1[j], 0.5 * h1, zplot1[j], xplot1[j + 1], 0.5 * h1, zplot1[j + 1]);
    line (xplot2[j], 0.5 * h1, zplot2[j], xplot2[j + 1], 0.5 * h1, zplot2[j + 1]);
    line (xplot1[j], -0.5 * h1, zplot1[j], xplot1[j + 1], -0.5 * h1, zplot1[j + 1]);
    line (xplot2[j], -0.5 * h1, zplot2[j], xplot2[j + 1], -0.5 * h1, zplot2[j + 1]);
  }
  line (xplot1[n - 1], 0.5 * h1, zplot1[n - 1], xplot2[n - 1], 0.5 * h1, zplot2[n - 1]);
  line (xplot1[n - 1], 0.5 * h1, zplot1[n - 1], xplot1[n - 1], -0.5 * h1, zplot1[n - 1]);
  line (xplot2[n - 1], -0.5 * h1, zplot2[n - 1], xplot2[n - 1], 0.5 * h1, zplot2[n - 1]);
  line (xplot1[n - 1], -0.5 * h1, zplot1[n - 1], xplot2[n - 1], -0.5 * h1, zplot2[n - 1]);
  #undef w1
  #undef h1
  #undef l
  #undef R0
  #undef Qc
  #undef alpha
  #undef m
  #undef W
  #undef curvature
  return(_comp);
} /* class_Guide_curved_display */

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
  SIG_MESSAGE("[_Before_selec_display] component Before_selec=PSD_monitor() DISPLAY [PSD_monitor:0]");

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

_class_Selector *class_Selector_display(_class_Selector *_comp
) {
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define length (_comp->_parameters.length)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define nslit (_comp->_parameters.nslit)
  #define d (_comp->_parameters.d)
  #define radius (_comp->_parameters.radius)
  #define alpha (_comp->_parameters.alpha)
  #define nu (_comp->_parameters.nu)
  SIG_MESSAGE("[_SELEC_display] component SELEC=Selector() DISPLAY [Selector:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);
  double phi, r0, Width, height, l0, l1;
  double r;
  double x0;
  double x1;
  double y0;
  double y1;
  double z0;
  double z1;
  double z2;
  double z3;
  double a;
  double xw, yh;

  phi = alpha;
  Width = (xmax - xmin) / 2;
  height = (ymax - ymin) / 2;
  x0 = xmin;
  x1 = xmax;
  y0 = ymin;
  y1 = ymax;
  l0 = length;
  l1 = l0;
  r0 = radius;

  r = r0 + height;
  x0 = -Width / 2.0;
  x1 = Width / 2.0;
  y0 = -height / 2.0;
  y1 = height / 2.0;
  z0 = 0;
  z1 = 0;
  z2 = l1;
  z3 = l0;

  xw = Width / 2.0;
  yh = height / 2.0;
  /* Draw apertures */
  for (a = z0;;) {
    multiline (3, x0 - xw, (double)y1, a, (double)x0, (double)y1, a, (double)x0, y1 + yh, a);
    multiline (3, x1 + xw, (double)y1, a, (double)x1, (double)y1, a, (double)x1, y1 + yh, a);
    multiline (3, x0 - xw, (double)y0, a, (double)x0, (double)y0, a, (double)x0, y0 - yh, a);
    multiline (3, x1 + xw, (double)y0, a, (double)x1, (double)y0, a, (double)x1, y0 - yh, a);
    if (a == z3)
      break;
    else
      a = z3;
  }

  /* Draw cylinder. */
  circle ("xy", 0, -r0, z1, r);
  circle ("xy", 0, -r0, z2, r);
  line (0, -r0, z1, 0, -r0, z2);
  for (a = 0; a < 2 * PI; a += PI / 8) {
    multiline (4, 0.0, -r0, z1, r * cos (a), r * sin (a) - r0, z1, r * cos (a + DEG2RAD * phi), r * sin (a + DEG2RAD * phi) - r0, z2, 0.0, -r0, z2);
  }
  /*

  multiline(5, (double)xmin, (double)ymin, 0.0,
               (double)xmax, (double)ymin, 0.0,
               (double)xmax, (double)ymax, 0.0,
               (double)xmin, (double)ymax, 0.0,
               (double)xmin, (double)ymin, 0.0); */
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef length
  #undef xwidth
  #undef yheight
  #undef nslit
  #undef d
  #undef radius
  #undef alpha
  #undef nu
  return(_comp);
} /* class_Selector_display */

_class_Guide_tapering *class_Guide_tapering_display(_class_Guide_tapering *_comp
) {
  #define option (_comp->_parameters.option)
  #define w1 (_comp->_parameters.w1)
  #define h1 (_comp->_parameters.h1)
  #define l (_comp->_parameters.l)
  #define linw (_comp->_parameters.linw)
  #define loutw (_comp->_parameters.loutw)
  #define linh (_comp->_parameters.linh)
  #define louth (_comp->_parameters.louth)
  #define R0 (_comp->_parameters.R0)
  #define Qcx (_comp->_parameters.Qcx)
  #define Qcy (_comp->_parameters.Qcy)
  #define alphax (_comp->_parameters.alphax)
  #define alphay (_comp->_parameters.alphay)
  #define W (_comp->_parameters.W)
  #define mx (_comp->_parameters.mx)
  #define my (_comp->_parameters.my)
  #define segno (_comp->_parameters.segno)
  #define curvature (_comp->_parameters.curvature)
  #define curvature_v (_comp->_parameters.curvature_v)
  #define w1c (_comp->_parameters.w1c)
  #define w2c (_comp->_parameters.w2c)
  #define ww (_comp->_parameters.ww)
  #define hh (_comp->_parameters.hh)
  #define whalf (_comp->_parameters.whalf)
  #define hhalf (_comp->_parameters.hhalf)
  #define lwhalf (_comp->_parameters.lwhalf)
  #define lhhalf (_comp->_parameters.lhhalf)
  #define h1_in (_comp->_parameters.h1_in)
  #define h2_out (_comp->_parameters.h2_out)
  #define w1_in (_comp->_parameters.w1_in)
  #define w2_out (_comp->_parameters.w2_out)
  #define l_seg (_comp->_parameters.l_seg)
  #define h12 (_comp->_parameters.h12)
  #define h2 (_comp->_parameters.h2)
  #define w12 (_comp->_parameters.w12)
  #define w2 (_comp->_parameters.w2)
  #define a_ell_q (_comp->_parameters.a_ell_q)
  #define b_ell_q (_comp->_parameters.b_ell_q)
  #define lbw (_comp->_parameters.lbw)
  #define lbh (_comp->_parameters.lbh)
  #define mxi (_comp->_parameters.mxi)
  #define u1 (_comp->_parameters.u1)
  #define u2 (_comp->_parameters.u2)
  #define div1 (_comp->_parameters.div1)
  #define p2_para (_comp->_parameters.p2_para)
  #define test (_comp->_parameters.test)
  #define Div1 (_comp->_parameters.Div1)
  #define seg (_comp->_parameters.seg)
  #define fu (_comp->_parameters.fu)
  #define pos (_comp->_parameters.pos)
  #define file_name (_comp->_parameters.file_name)
  #define ep (_comp->_parameters.ep)
  #define num (_comp->_parameters.num)
  #define rotation_h (_comp->_parameters.rotation_h)
  #define rotation_v (_comp->_parameters.rotation_v)
  SIG_MESSAGE("[_elliptical_piece_display] component elliptical_piece=Guide_tapering() DISPLAY [Guide_tapering:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);
  int i, ii;

  for (ii = 0; ii < seg; ii++) {
    multiline (5, -w1_in[ii] / 2.0, -h1_in[ii] / 2.0, l_seg * (double)ii, -w2_out[ii] / 2.0, -h2_out[ii] / 2.0, l_seg * ((double)ii + 1.0), -w2_out[ii] / 2.0,
               h2_out[ii] / 2.0, l_seg * ((double)ii + 1.0), -w1_in[ii] / 2.0, h1_in[ii] / 2.0, l_seg * (double)ii, -w1_in[ii] / 2.0, -h1_in[ii] / 2.0,
               l_seg * (double)ii);
    multiline (5, w1_in[ii] / 2.0, -h1_in[ii] / 2.0, l_seg * (double)ii, w2_out[ii] / 2.0, -h2_out[ii] / 2.0, l_seg * ((double)ii + 1.0), w2_out[ii] / 2.0,
               h2_out[ii] / 2.0, l_seg * ((double)ii + 1.0), w1_in[ii] / 2.0, h1_in[ii] / 2.0, l_seg * (double)ii, w1_in[ii] / 2.0, -h1_in[ii] / 2.0,
               l_seg * (double)ii);
  }
  line (-w1 / 2.0, -h1 / 2.0, 0.0, w1 / 2.0, -h1 / 2.0, 0.0);
  line (-w1 / 2.0, h1 / 2.0, 0.0, w1 / 2.0, h1 / 2.0, 0.0);
  for (i = 0; i < segno; i++) {
    line (-w2_out[i] / 2.0, -h2_out[i] / 2.0, l_seg * (double)(i + 1), w2_out[i] / 2.0, -h2_out[i] / 2.0, l_seg * (double)(i + 1));
    line (-w2_out[i] / 2.0, h2_out[i] / 2.0, l_seg * (double)(i + 1), w2_out[i] / 2.0, h2_out[i] / 2.0, l_seg * (double)(i + 1));
  }
  #undef option
  #undef w1
  #undef h1
  #undef l
  #undef linw
  #undef loutw
  #undef linh
  #undef louth
  #undef R0
  #undef Qcx
  #undef Qcy
  #undef alphax
  #undef alphay
  #undef W
  #undef mx
  #undef my
  #undef segno
  #undef curvature
  #undef curvature_v
  #undef w1c
  #undef w2c
  #undef ww
  #undef hh
  #undef whalf
  #undef hhalf
  #undef lwhalf
  #undef lhhalf
  #undef h1_in
  #undef h2_out
  #undef w1_in
  #undef w2_out
  #undef l_seg
  #undef h12
  #undef h2
  #undef w12
  #undef w2
  #undef a_ell_q
  #undef b_ell_q
  #undef lbw
  #undef lbh
  #undef mxi
  #undef u1
  #undef u2
  #undef div1
  #undef p2_para
  #undef test
  #undef Div1
  #undef seg
  #undef fu
  #undef pos
  #undef file_name
  #undef ep
  #undef num
  #undef rotation_h
  #undef rotation_v
  return(_comp);
} /* class_Guide_tapering_display */

_class_Slit *class_Slit_display(_class_Slit *_comp
) {
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define radius (_comp->_parameters.radius)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define isradial (_comp->_parameters.isradial)
  SIG_MESSAGE("[_Virtual_source_display] component Virtual_source=Slit() DISPLAY [Slit:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);

  if (is_unset (radius)) {
    double xw, yh;
    xw = (xmax - xmin) / 2.0;
    yh = (ymax - ymin) / 2.0;
    multiline (3, xmin - xw, (double)ymax, 0.0, (double)xmin, (double)ymax, 0.0, (double)xmin, ymax + yh, 0.0);
    multiline (3, xmax + xw, (double)ymax, 0.0, (double)xmax, (double)ymax, 0.0, (double)xmax, ymax + yh, 0.0);
    multiline (3, xmin - xw, (double)ymin, 0.0, (double)xmin, (double)ymin, 0.0, (double)xmin, ymin - yh, 0.0);
    multiline (3, xmax + xw, (double)ymin, 0.0, (double)xmax, (double)ymin, 0.0, (double)xmax, ymin - yh, 0.0);
  } else {
    circle ("xy", 0, 0, 0, radius);
  }
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef radius
  #undef xwidth
  #undef yheight
  #undef isradial
  return(_comp);
} /* class_Slit_display */

_class_Guide_channeled *class_Guide_channeled_display(_class_Guide_channeled *_comp
) {
  #define w1 (_comp->_parameters.w1)
  #define h1 (_comp->_parameters.h1)
  #define w2 (_comp->_parameters.w2)
  #define h2 (_comp->_parameters.h2)
  #define l (_comp->_parameters.l)
  #define R0 (_comp->_parameters.R0)
  #define Qc (_comp->_parameters.Qc)
  #define alpha (_comp->_parameters.alpha)
  #define m (_comp->_parameters.m)
  #define nslit (_comp->_parameters.nslit)
  #define d (_comp->_parameters.d)
  #define Qcx (_comp->_parameters.Qcx)
  #define Qcy (_comp->_parameters.Qcy)
  #define alphax (_comp->_parameters.alphax)
  #define alphay (_comp->_parameters.alphay)
  #define W (_comp->_parameters.W)
  #define mx (_comp->_parameters.mx)
  #define my (_comp->_parameters.my)
  #define nu (_comp->_parameters.nu)
  #define phase (_comp->_parameters.phase)
  #define w1c (_comp->_parameters.w1c)
  #define w2c (_comp->_parameters.w2c)
  #define ww (_comp->_parameters.ww)
  #define hh (_comp->_parameters.hh)
  #define whalf (_comp->_parameters.whalf)
  #define hhalf (_comp->_parameters.hhalf)
  #define lwhalf (_comp->_parameters.lwhalf)
  #define lhhalf (_comp->_parameters.lhhalf)
  SIG_MESSAGE("[_NL1B_Vertical_Guide_display] component NL1B_Vertical_Guide=Guide_channeled() DISPLAY [Guide_channeled:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);
  int i;

  /* Draw the vertial slit-planes along each channel */
  for (i = 0; i < nslit; i++) {
    polygon (4, i * w1c - w1 / 2.0, -h1 / 2.0, 0.0, i * w2c - w2 / 2.0, -h2 / 2.0, (double)l, i * w2c - w2 / 2.0, h2 / 2.0, (double)l, i * w1c - w1 / 2.0,
             h1 / 2.0, 0.0);
    polygon (4, (i + 1) * w1c - d - w1 / 2.0, -h1 / 2.0, 0.0, (i + 1) * w2c - d - w2 / 2.0, -h2 / 2.0, (double)l, (i + 1) * w2c - d - w2 / 2.0, h2 / 2.0,
             (double)l, (i + 1) * w1c - d - w1 / 2.0, h1 / 2.0, 0.0);
  }
  /* Add "bottom" and "lid" */
  polygon (4, -w1 / 2.0, -h1 / 2.0, 0.0, w1 / 2.0, -h1 / 2.0, 0.0, w2 / 2.0, -h2 / 2.0, (double)l, -w2 / 2.0, -h2 / 2.0, (double)l);
  polygon (4, -w1 / 2.0, h1 / 2.0, 0.0, w1 / 2.0, h1 / 2.0, 0.0, w2 / 2.0, h2 / 2.0, (double)l, -w2 / 2.0, h2 / 2.0, (double)l);

  if (nu || phase) {
    double radius = sqrt (w1 * w1 + l * l);
    /* cylinder top/center/bottom  */
    circle ("xz", 0, -h1 / 2, l / 2, radius);
    circle ("xz", 0, 0, l / 2, radius);
    circle ("xz", 0, h1 / 2, l / 2, radius);
  }
  #undef w1
  #undef h1
  #undef w2
  #undef h2
  #undef l
  #undef R0
  #undef Qc
  #undef alpha
  #undef m
  #undef nslit
  #undef d
  #undef Qcx
  #undef Qcy
  #undef alphax
  #undef alphay
  #undef W
  #undef mx
  #undef my
  #undef nu
  #undef phase
  #undef w1c
  #undef w2c
  #undef ww
  #undef hh
  #undef whalf
  #undef hhalf
  #undef lwhalf
  #undef lhhalf
  return(_comp);
} /* class_Guide_channeled_display */

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
  SIG_MESSAGE("[_energy_endguide_display] component energy_endguide=E_monitor() DISPLAY [E_monitor:0]");

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

_class_Monochromator_curved *class_Monochromator_curved_display(_class_Monochromator_curved *_comp
) {
  #define reflect (_comp->_parameters.reflect)
  #define transmit (_comp->_parameters.transmit)
  #define zwidth (_comp->_parameters.zwidth)
  #define yheight (_comp->_parameters.yheight)
  #define gap (_comp->_parameters.gap)
  #define NH (_comp->_parameters.NH)
  #define NV (_comp->_parameters.NV)
  #define mosaich (_comp->_parameters.mosaich)
  #define mosaicv (_comp->_parameters.mosaicv)
  #define r0 (_comp->_parameters.r0)
  #define t0 (_comp->_parameters.t0)
  #define Q (_comp->_parameters.Q)
  #define RV (_comp->_parameters.RV)
  #define RH (_comp->_parameters.RH)
  #define DM (_comp->_parameters.DM)
  #define mosaic (_comp->_parameters.mosaic)
  #define width (_comp->_parameters.width)
  #define height (_comp->_parameters.height)
  #define verbose (_comp->_parameters.verbose)
  #define order (_comp->_parameters.order)
  #define mos_rms_y (_comp->_parameters.mos_rms_y)
  #define mos_rms_z (_comp->_parameters.mos_rms_z)
  #define mos_rms_max (_comp->_parameters.mos_rms_max)
  #define mono_Q (_comp->_parameters.mono_Q)
  #define SlabWidth (_comp->_parameters.SlabWidth)
  #define SlabHeight (_comp->_parameters.SlabHeight)
  #define rTable (_comp->_parameters.rTable)
  #define tTable (_comp->_parameters.tTable)
  #define rTableFlag (_comp->_parameters.rTableFlag)
  #define tTableFlag (_comp->_parameters.tTableFlag)
  #define tiltH (_comp->_parameters.tiltH)
  #define tiltV (_comp->_parameters.tiltV)
  #define ncol_var (_comp->_parameters.ncol_var)
  #define nrow_var (_comp->_parameters.nrow_var)
  SIG_MESSAGE("[_Monochromator_display] component Monochromator=Monochromator_curved() DISPLAY [Monochromator_curved:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);
  int ih;

  for (ih = 0; ih < NH; ih++) {
    int iv;
    for (iv = 0; iv < NV; iv++) {
      double zmin, zmax, ymin, ymax;
      double xt, yt;

      zmin = (SlabWidth + gap) * (ih - NH / 2.0) + gap / 2;
      zmax = zmin + SlabWidth;
      ymin = (SlabHeight + gap) * (iv - NV / 2.0) + gap / 2;
      ymax = ymin + SlabHeight;

      if (RH)
        xt = -(zmax * zmax - zmin * zmin) / RH / 2;
      else
        xt = 0;

      if (RV)
        yt = -(ymax * ymax - ymin * ymin) / RV / 2;
      else
        yt = 0;
      multiline (5, xt + yt, (double)ymin, (double)zmin, xt - yt, (double)ymax, (double)zmin, -xt - yt, (double)ymax, (double)zmax, -xt + yt, (double)ymin,
                 (double)zmax, xt + yt, (double)ymin, (double)zmin);
    }
  }
  #undef reflect
  #undef transmit
  #undef zwidth
  #undef yheight
  #undef gap
  #undef NH
  #undef NV
  #undef mosaich
  #undef mosaicv
  #undef r0
  #undef t0
  #undef Q
  #undef RV
  #undef RH
  #undef DM
  #undef mosaic
  #undef width
  #undef height
  #undef verbose
  #undef order
  #undef mos_rms_y
  #undef mos_rms_z
  #undef mos_rms_max
  #undef mono_Q
  #undef SlabWidth
  #undef SlabHeight
  #undef rTable
  #undef tTable
  #undef rTableFlag
  #undef tTableFlag
  #undef tiltH
  #undef tiltV
  #undef ncol_var
  #undef nrow_var
  return(_comp);
} /* class_Monochromator_curved_display */

_class_Divergence_monitor *class_Divergence_monitor_display(_class_Divergence_monitor *_comp
) {
  #define nh (_comp->_parameters.nh)
  #define nv (_comp->_parameters.nv)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define maxdiv_h (_comp->_parameters.maxdiv_h)
  #define maxdiv_v (_comp->_parameters.maxdiv_v)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nx (_comp->_parameters.nx)
  #define ny (_comp->_parameters.ny)
  #define nz (_comp->_parameters.nz)
  #define Div_N (_comp->_parameters.Div_N)
  #define Div_p (_comp->_parameters.Div_p)
  #define Div_p2 (_comp->_parameters.Div_p2)
  SIG_MESSAGE("[_div_mono_display] component div_mono=Divergence_monitor() DISPLAY [Divergence_monitor:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);
  multiline (5, (double)xmin, (double)ymin, 0.0, (double)xmax, (double)ymin, 0.0, (double)xmax, (double)ymax, 0.0, (double)xmin, (double)ymax, 0.0, (double)xmin,
             (double)ymin, 0.0);
  #undef nh
  #undef nv
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef nowritefile
  #undef xwidth
  #undef yheight
  #undef maxdiv_h
  #undef maxdiv_v
  #undef restore_neutron
  #undef nx
  #undef ny
  #undef nz
  #undef Div_N
  #undef Div_p
  #undef Div_p2
  return(_comp);
} /* class_Divergence_monitor_display */

_class_Div1D_monitor *class_Div1D_monitor_display(_class_Div1D_monitor *_comp
) {
  #define ndiv (_comp->_parameters.ndiv)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define maxdiv (_comp->_parameters.maxdiv)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define vertical (_comp->_parameters.vertical)
  #define Div_N (_comp->_parameters.Div_N)
  #define Div_p (_comp->_parameters.Div_p)
  #define Div_p2 (_comp->_parameters.Div_p2)
  SIG_MESSAGE("[_div_mono_H_display] component div_mono_H=Div1D_monitor() DISPLAY [Div1D_monitor:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);
  multiline (5, (double)xmin, (double)ymin, 0.0, (double)xmax, (double)ymin, 0.0, (double)xmax, (double)ymax, 0.0, (double)xmin, (double)ymax, 0.0, (double)xmin,
             (double)ymin, 0.0);
  #undef ndiv
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef xwidth
  #undef yheight
  #undef maxdiv
  #undef restore_neutron
  #undef nowritefile
  #undef vertical
  #undef Div_N
  #undef Div_p
  #undef Div_p2
  return(_comp);
} /* class_Div1D_monitor_display */

_class_PSDlin_monitor *class_PSDlin_monitor_display(_class_PSDlin_monitor *_comp
) {
  #define nbins (_comp->_parameters.nbins)
  #define filename (_comp->_parameters.filename)
  #define xmin (_comp->_parameters.xmin)
  #define xmax (_comp->_parameters.xmax)
  #define ymin (_comp->_parameters.ymin)
  #define ymax (_comp->_parameters.ymax)
  #define nowritefile (_comp->_parameters.nowritefile)
  #define xwidth (_comp->_parameters.xwidth)
  #define yheight (_comp->_parameters.yheight)
  #define restore_neutron (_comp->_parameters.restore_neutron)
  #define vertical (_comp->_parameters.vertical)
  #define PSDlin_N (_comp->_parameters.PSDlin_N)
  #define PSDlin_p (_comp->_parameters.PSDlin_p)
  #define PSDlin_p2 (_comp->_parameters.PSDlin_p2)
  SIG_MESSAGE("[_dpsd1_display] component dpsd1=PSDlin_monitor() DISPLAY [PSDlin_monitor:0]");

  printf("MCDISPLAY: component %s\n", _comp->_name);
  multiline (5, (double)xmin, (double)ymin, 0.0, (double)xmax, (double)ymin, 0.0, (double)xmax, (double)ymax, 0.0, (double)xmin, (double)ymax, 0.0, (double)xmin,
             (double)ymin, 0.0);
  #undef nbins
  #undef filename
  #undef xmin
  #undef xmax
  #undef ymin
  #undef ymax
  #undef nowritefile
  #undef xwidth
  #undef yheight
  #undef restore_neutron
  #undef vertical
  #undef PSDlin_N
  #undef PSDlin_p
  #undef PSDlin_p2
  return(_comp);
} /* class_PSDlin_monitor_display */


  #undef magnify
  #undef line
  #undef dashed_line
  #undef multiline
  #undef rectangle
  #undef box
  #undef circle
  #undef cylinder
  #undef sphere

int display(void) { /* called by mccode_main for HZB_FLEX:DISPLAY */
  printf("MCDISPLAY: start\n");

  /* call iteratively all components DISPLAY */
  class_Progress_bar_display(&_Origin_var);

  class_Source_gen_display(&_Gen_Source_var);

  class_Arm_display(&_NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var);

  class_Guide_display(&_NL1A_NL1B_NL2_NL3_InPile_var);

  class_Arm_display(&_NL1B_Straight1_Entrance_Window_var);

  class_Guide_display(&_NL1B_Straight1_var);

  class_Arm_display(&_NL1B_Curved_Entrance_Window_var);

  class_Guide_curved_display(&_NL1B_Curved_Guide_1_var);

  class_Arm_display(&_NL1B_Velocity_Selector_Gap_Entrance_Window_var);

  class_PSD_monitor_display(&_Before_selec_var);

  class_Selector_display(&_SELEC_var);

  class_PSD_monitor_display(&_After_selec_var);

  class_Arm_display(&_NL1B_Curved_2_Entrance_Window_var);

  class_Guide_curved_display(&_NL1B_Curved_Guide_2_var);

  class_Arm_display(&_NL1B_Straight2_Entrance_Window_var);

  class_Guide_display(&_NL1B_Straight2_var);

  class_Arm_display(&_NL1B_Elliptical_Entrance_Window_var);

  class_Guide_tapering_display(&_elliptical_piece_var);

  class_Slit_display(&_Virtual_source_var);

  class_Arm_display(&_NL1B_Vertical_Guide_Entrance_Window_var);

  class_Guide_channeled_display(&_NL1B_Vertical_Guide_var);

  class_Arm_display(&_NL1B_Guide_Exit_var);

  class_E_monitor_display(&_energy_endguide_var);

  class_Arm_display(&_Mono_center_var);

  class_Monochromator_curved_display(&_Monochromator_var);

  class_Arm_display(&_Mono_sample_arm_var);

  class_E_monitor_display(&_energy_mono_var);

  class_E_monitor_display(&_energy_pre_sample_var);

  class_Arm_display(&_Sample_center_var);

  class_Arm_display(&_Sample_analyser_arm_var);

  class_Divergence_monitor_display(&_div_mono_var);

  class_Div1D_monitor_display(&_div_mono_H_var);

  class_PSD_monitor_display(&_psd_sam_var);

  class_PSDlin_monitor_display(&_dpsd1_var);

  printf("MCDISPLAY: end\n");

  return(0);
} /* display */

void* _getvar_parameters(char* compname)
/* enables settings parameters based use of the GETPAR macro */
{
  #ifdef OPENACC
    #define strcmp(a,b) str_comp(a,b)
  #endif
  if (!strcmp(compname, "Origin")) return (void *) &(_Origin_var._parameters);
  if (!strcmp(compname, "Gen_Source")) return (void *) &(_Gen_Source_var._parameters);
  if (!strcmp(compname, "NL1A_NL1B_NL2_NL3_InPile_Entrance_Window")) return (void *) &(_NL1A_NL1B_NL2_NL3_InPile_Entrance_Window_var._parameters);
  if (!strcmp(compname, "NL1A_NL1B_NL2_NL3_InPile")) return (void *) &(_NL1A_NL1B_NL2_NL3_InPile_var._parameters);
  if (!strcmp(compname, "NL1B_Straight1_Entrance_Window")) return (void *) &(_NL1B_Straight1_Entrance_Window_var._parameters);
  if (!strcmp(compname, "NL1B_Straight1")) return (void *) &(_NL1B_Straight1_var._parameters);
  if (!strcmp(compname, "NL1B_Curved_Entrance_Window")) return (void *) &(_NL1B_Curved_Entrance_Window_var._parameters);
  if (!strcmp(compname, "NL1B_Curved_Guide_1")) return (void *) &(_NL1B_Curved_Guide_1_var._parameters);
  if (!strcmp(compname, "NL1B_Velocity_Selector_Gap_Entrance_Window")) return (void *) &(_NL1B_Velocity_Selector_Gap_Entrance_Window_var._parameters);
  if (!strcmp(compname, "Before_selec")) return (void *) &(_Before_selec_var._parameters);
  if (!strcmp(compname, "SELEC")) return (void *) &(_SELEC_var._parameters);
  if (!strcmp(compname, "After_selec")) return (void *) &(_After_selec_var._parameters);
  if (!strcmp(compname, "NL1B_Curved_2_Entrance_Window")) return (void *) &(_NL1B_Curved_2_Entrance_Window_var._parameters);
  if (!strcmp(compname, "NL1B_Curved_Guide_2")) return (void *) &(_NL1B_Curved_Guide_2_var._parameters);
  if (!strcmp(compname, "NL1B_Straight2_Entrance_Window")) return (void *) &(_NL1B_Straight2_Entrance_Window_var._parameters);
  if (!strcmp(compname, "NL1B_Straight2")) return (void *) &(_NL1B_Straight2_var._parameters);
  if (!strcmp(compname, "NL1B_Elliptical_Entrance_Window")) return (void *) &(_NL1B_Elliptical_Entrance_Window_var._parameters);
  if (!strcmp(compname, "elliptical_piece")) return (void *) &(_elliptical_piece_var._parameters);
  if (!strcmp(compname, "Virtual_source")) return (void *) &(_Virtual_source_var._parameters);
  if (!strcmp(compname, "NL1B_Vertical_Guide_Entrance_Window")) return (void *) &(_NL1B_Vertical_Guide_Entrance_Window_var._parameters);
  if (!strcmp(compname, "NL1B_Vertical_Guide")) return (void *) &(_NL1B_Vertical_Guide_var._parameters);
  if (!strcmp(compname, "NL1B_Guide_Exit")) return (void *) &(_NL1B_Guide_Exit_var._parameters);
  if (!strcmp(compname, "energy_endguide")) return (void *) &(_energy_endguide_var._parameters);
  if (!strcmp(compname, "Mono_center")) return (void *) &(_Mono_center_var._parameters);
  if (!strcmp(compname, "Monochromator")) return (void *) &(_Monochromator_var._parameters);
  if (!strcmp(compname, "Mono_sample_arm")) return (void *) &(_Mono_sample_arm_var._parameters);
  if (!strcmp(compname, "energy_mono")) return (void *) &(_energy_mono_var._parameters);
  if (!strcmp(compname, "energy_pre_sample")) return (void *) &(_energy_pre_sample_var._parameters);
  if (!strcmp(compname, "Sample_center")) return (void *) &(_Sample_center_var._parameters);
  if (!strcmp(compname, "Sample_analyser_arm")) return (void *) &(_Sample_analyser_arm_var._parameters);
  if (!strcmp(compname, "div_mono")) return (void *) &(_div_mono_var._parameters);
  if (!strcmp(compname, "div_mono_H")) return (void *) &(_div_mono_H_var._parameters);
  if (!strcmp(compname, "psd_sam")) return (void *) &(_psd_sam_var._parameters);
  if (!strcmp(compname, "dpsd1")) return (void *) &(_dpsd1_var._parameters);
  return 0;
}

void* _get_particle_var(char *token, _class_particle *p)
/* enables setpars based use of GET_PARTICLE_DVAR macro and similar */
{
  if (!strcmp(token, "ncol_25")) return (void *) &(p->ncol_25);
  if (!strcmp(token, "nrow_25")) return (void *) &(p->nrow_25);
  return 0;
}

int _getcomp_index(char* compname)
/* Enables retrieving the component position & rotation when the index is not known.
 * Component indexing into MACROS, e.g., POS_A_COMP_INDEX, are 1-based! */
{
  if (!strcmp(compname, "Origin")) return 1;
  if (!strcmp(compname, "Gen_Source")) return 2;
  if (!strcmp(compname, "NL1A_NL1B_NL2_NL3_InPile_Entrance_Window")) return 3;
  if (!strcmp(compname, "NL1A_NL1B_NL2_NL3_InPile")) return 4;
  if (!strcmp(compname, "NL1B_Straight1_Entrance_Window")) return 5;
  if (!strcmp(compname, "NL1B_Straight1")) return 6;
  if (!strcmp(compname, "NL1B_Curved_Entrance_Window")) return 7;
  if (!strcmp(compname, "NL1B_Curved_Guide_1")) return 8;
  if (!strcmp(compname, "NL1B_Velocity_Selector_Gap_Entrance_Window")) return 9;
  if (!strcmp(compname, "Before_selec")) return 10;
  if (!strcmp(compname, "SELEC")) return 11;
  if (!strcmp(compname, "After_selec")) return 12;
  if (!strcmp(compname, "NL1B_Curved_2_Entrance_Window")) return 13;
  if (!strcmp(compname, "NL1B_Curved_Guide_2")) return 14;
  if (!strcmp(compname, "NL1B_Straight2_Entrance_Window")) return 15;
  if (!strcmp(compname, "NL1B_Straight2")) return 16;
  if (!strcmp(compname, "NL1B_Elliptical_Entrance_Window")) return 17;
  if (!strcmp(compname, "elliptical_piece")) return 18;
  if (!strcmp(compname, "Virtual_source")) return 19;
  if (!strcmp(compname, "NL1B_Vertical_Guide_Entrance_Window")) return 20;
  if (!strcmp(compname, "NL1B_Vertical_Guide")) return 21;
  if (!strcmp(compname, "NL1B_Guide_Exit")) return 22;
  if (!strcmp(compname, "energy_endguide")) return 23;
  if (!strcmp(compname, "Mono_center")) return 24;
  if (!strcmp(compname, "Monochromator")) return 25;
  if (!strcmp(compname, "Mono_sample_arm")) return 26;
  if (!strcmp(compname, "energy_mono")) return 27;
  if (!strcmp(compname, "energy_pre_sample")) return 28;
  if (!strcmp(compname, "Sample_center")) return 29;
  if (!strcmp(compname, "Sample_analyser_arm")) return 30;
  if (!strcmp(compname, "div_mono")) return 31;
  if (!strcmp(compname, "div_mono_H")) return 32;
  if (!strcmp(compname, "psd_sam")) return 33;
  if (!strcmp(compname, "dpsd1")) return 34;
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

/* end of generated C code ./HZB_FLEX.c */
