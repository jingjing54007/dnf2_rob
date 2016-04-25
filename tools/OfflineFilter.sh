#!/bin/bash

#date: 2013-08-02
#author: v_zhtang
#desc: filter data in offline module


curday=`date +"%Y-%m-%d"`
logfile=./tools/logs/offlineFilter_$curday.log
echoLogWithTime()
{
	local curtime=`date +"%Y-%m-%d %H:%M:%S"`
	echo "[$curtime]: $1" >> $logfile
}

if [ $# -ne 2 ]
then
	echo "incorrect input params,please input area/qq such as below."
	echo "$0 14 23243213"
	exit 1
else
	area=$1
	uin=$2
fi

resultfile=$3
echoLogWithTime "Begin--[$area][$uin]"

find /data/RobGameSystem/webdata -name "dnf2*.txt" -ctime -10 -exec cat {} \; | grep "$area $uin" | awk '{a[$4$5]=$0}END{for(t in a) print a[t]}' > $resultfile

echoLogWithTime "End----[$area][$uin]"

linenum=`wc -l $resultfile`
echoLogWithTime "result-[$linenum]"
return ${linenum};
