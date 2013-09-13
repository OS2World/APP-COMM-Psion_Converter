/*** Psion REXX Interface API Usage Example ******/
/*** (c) 2001 Mike Potapoff                 ******/

/*** Function Return Code Definitions ************/
RET_OK              =  0
RET_NOT_CONNECTED   = -1
RET_NOT_ENOUGH_MEM  = -2
RET_ALREADY_RUNNED  = -3
RET_RUN_TIMEOUT     = -4
RET_NO_SUCH_PROCESS = -5

SAY 'Psion REXX Interface v1.0. Usage Example'
SAY '(c) 2001 Mike Potapoff'
SAY
SAY 'Registering all Functions for further usage'
SAY

CALL RxFuncAdd 'prxInit', 'psilib.dll', 'prxInit'
CALL prxInit

port = 'COM1'
SAY 'Connecting to Psion, attached to' port '...'

IF prxConnect(port,0,'',1) \== RET_OK THEN DO
    SAY 'Err: Connection to Psion Failure, code =' ERR
    SAY '     Check the Connection or Parameters.'
    CALL prxQuit
    EXIT
END
SAY 'Ok'
SAY
SAY 'Getting Various Information from Psion'
SAY
SAY 'Psion Version = ' prxPsionVersion()
SAY 'Psion Type    = ' prxPsionType()
SAY

/*** Owner Info consists of some strings. Parse it ***/
own = prxPsionOwner()
SAY "Owner's Info:"
DO i = 1 BY 1 WHILE LENGTH(STRIP(own)) \= 0
    PARSE VALUE own WITH s '0A'x own
    say 'Line' i '=' s
END

'@pause'
SAY
SAY 'Get Other Information'
rc = prxPsionParameters('Info')
SAY 'Info Pieces = ' Info.0
SAY
DO i = 1 to Info.0 by 1
    INTERPRET 'j = Info.' || Info.i
    SAY Info.i " = " j
END

'@pause'

SAY
SAY 'Testing process control routines'
SAY

/*  Put any name or path to run here */
task = 'Z:\System\Apps\Calc\Calc.app'   

SAY 'Running "' || task || '" and Waiting for its PID...'
rc = prxTaskStart(task,  0, "id")
SELECT
  WHEN rc = RET_OK THEN
    SAY 'Task is Started, its ID = ' id
  WHEN rc = RET_ALREADY_RUNNED THEN
    SAY 'Task is Already Running, its ID = ' id
  WHEN rc = RET_NO_SUCH_PROCESS THEN
    SAY 'Err: No such task'
  WHEN rc = RET_RUN_TIMEOUT THEN
    SAY "Err: Psion didn't return task's PID in reasonable time"
END

SAY
SAY "Testing if Task is Running by it's PID..."
IF prxTaskIsRunning(id) == RET_OK THEN
    SAY 'Ok'
ELSE
    SAY 'Err: No Such Process'

SAY
SAY "Testing if Task is Running by it's FileName..."
IF prxTaskIsRunning(task) == RET_OK THEN
    SAY 'Ok'
ELSE
    SAY 'Err: No Such Opened File '

SAY
SAY "Getting PID and File Lists..."
call prxTaskList 'PIDList', 'fileList'
SAY 'Ok, Total Number of Processes Running:' PIDList.0
SAY
SAY  'Process List'
DO i = 1 to PIDList.0
    SAY i'.' PIDList.i fileList.i
END

'@pause'
SAY
SAY 'Terminating Task without Waiting...'
SAY 'Stop by Name: ' prxTaskKill(task)
SAY 'Stop by ID:   ' prxTaskKill(id)
SAY
SAY 'Testing if Task is Still Running...'
SAY 'Running: ' prxTaskIsRunning(id)
SAY
SAY 'Terminating Task and waiting for termination...'
SAY 'Stop:    ' prxTaskKill(id,1)
SAY
SAY 'Testing if Task is Still Runnung...'
SAY 'Running: ' prxTaskIsRunning(id)
SAY
SAY 'Testing completed. Exiting.'

CALL prxQuit
EXIT

/*
    End of Example
*/
