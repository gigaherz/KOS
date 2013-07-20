/* Copyright (c) Microsoft Corporation. All rights reserved. */
typedef unsigned long int uint;

extern uint t_align;

void quit(
    int exit_code,
    char    *message,
    void    *arg);

int join_them(
    int kern_infile,
    int fs_infile,
    int outfile);

int catfiles(
    char    *fout,
    char    *fkern,
    char    *fsys);

int SizeOfTheseFiles(
    int argc,
    char    **argv);

int file_zero(
    int outfile,
    uint    bytes);
 
int file_copy(
    int infile,
    int outfile,
    uint    bytes);
 
int file_check_copy(
    int infile,
    int outfile,
    uint    bytes,
    uint    fssize,
    uint    fpointer,
    uint    ftype);
 
int check(
    int nmatch,
    uint    *data,
    int nbytes,
    int *pmatch,
    uint    fssize,
    uint    fpointer,
    uint    ftype);

char *MakeAName(void);

char *mkfs(
    int argc,
    char    **argv);

char *mkfat(
    int argc,
    char    **argv);

char *mkrom(
    int argc,
    char    **argv);

int CreateNewCmdArguments(
    int     argc, 
    char    **argv, 
    int     *newArgc, 
    char    ***newArgv);

void FreeCmdArguments(
    char    **argv);
