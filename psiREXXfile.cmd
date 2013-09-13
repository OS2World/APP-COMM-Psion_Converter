/*** Psion REXX Interface API Usage Example ******/
/*** (c) 2001 Mike Potapoff                 ******/

RET_OK = 0

SAY 'Psion REXX Interface v1.0. Usage Example'
SAY '(c) 2001 Mike Potapoff'
SAY
SAY 'Registering all Functions for further usage'

CALL RxFuncAdd 'prxInit', 'psilib.dll', 'prxInit'
CALL prxInit

SAY 'Connecting to Psion, attached to COM1...'

IF prxConnect("COM1") \== RET_OK THEN DO
    SAY 'Err: Connection to Psion Failure.'
    SAY '     Check the Connection or Parameters.'
    CALL prxQuit
    EXIT
END
SAY 'Ok'
SAY
SAY 'Performing Various file-operations with Psion'
SAY
SAY 'Version = ' prxPsionVersion()

SAY 'Current Dir:' prxPWD()

DO FOREVER
  SAY 'Enter filemask for file list or:'
  SAY 'c-CD, m-MD, r-RD, d-Del, g-get, p-put, s-set attr, v-view attr, q-quit.'
  PARSE PULL s
SAY "You just entered '" || s || "'"
  SELECT
    WHEN s == '0' 
      THEN LEAVE
    WHEN s == 'q' 
      THEN LEAVE
    WHEN s == 's'
      THEN DO
        SAY 'Enter Filename and desirable attr mask'
        PARSE PULL p1 p2
        SAY 'rc =' prxSetFileInfo(p1,p2)
      END
    WHEN s == 'v'
      THEN DO
        SAY 'Enter Filename to see attrubutes'
        PARSE PULL d
        SAY 'rc =' prxGetFileInfo(d)
      END
    WHEN s == 'c'
      THEN DO
        SAY 'Enter DirName to change'
        PARSE PULL d
        SAY 'rc =' prxCD(d)
      END
    WHEN s == 'm'
      THEN DO
        SAY 'Enter DirName to make'
        PARSE PULL d
        SAY 'rc =' prxMD(d)
      END
    WHEN s == 'r'
      THEN DO
        SAY 'Enter DirName to remove'
        PARSE PULL d
        SAY 'rc =' prxRD(d)
      END
    WHEN s == 'g'
      THEN DO
        SAY 'Enter Source Psion filename to get and target filename'
        PARSE PULL p1 p2
        SAY 'rc =' prxGet(p1,p2)
      END
    WHEN s == 'p'
      THEN DO
        SAY 'Enter Source OS/2 filename to put and target Psion filename'
        PARSE PULL p1 p2
        SAY 'rc =' prxPut(p1,p2)
      END
    WHEN s == 'd'
      THEN DO
        SAY 'Enter FileName to delete'
        PARSE PULL d
        SAY 'rc =' prxDel(d)
      END
  OTHERWISE
    CALL prxFileList 'Dr', s
    SAY 'Files in dir "' || prxPWD() || '": ' || Dr.0
    DO i = 1 to Dr.0 by 1
      SAY Dr.i
    END
  END
END

CALL prxQuit
EXIT
