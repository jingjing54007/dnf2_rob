#!/bin/bash

#Author: v_zhtang
#Date: 2013-04-25
#Desc: collect the suspended ics and handle with it.


timeNow=`date -d "now()" "+%Y-%m-%d %H:%M:%S"`
timeDay=`date -d "now()" "+%Y-%m-%d"`
timeUnix=`date +%s`

function getCurrentTimeNow(){
	timeNow=`date -d "now()" "+%Y-%m-%d %H:%M:%S"`
	return ${timeNow}	
}

function echoLogWithTime(){
	logstr=$1
	timeNow=`date -d "now()" "+%Y-%m-%d %H:%M:%S"`
	if [ $# -ne 1 ];then
		echo "[${timeNow}]: Error Print Log Params In"
	else
		echo "[${timeNow}]: ${logstr}"
	fi
}

if [ $# -eq 2 ]
then
	serid=$1
	dbname=$2
else
	serid=148
	dbname="OssAutoRob_DNF"
fi

icsfile="./logs/ics_suspend_${timeDay}.txt"
updateics="./logs/ics_update_${timeDay}.sql"

#1. fetch the suspend ics
sql="select iID,iICSId,iICSAnaylseResult,iRobed_Uin,dtBookTime from tbICSComplaint where iServiceID=${serid} and dtUpdateTime<CURDATE() and DATEDIFF(SUBDATE(CURDATE(),INTERVAL 1 DAY),dtUpdateTime) between 0 and 2 and iICSAnaylseResult in (11,20,30,40,50)"
mysql -h10.134.139.215 -urobgame -probgame@123 -P3306 $dbname -e "${sql}" > $icsfile

#2. get the icsid and format it into the update sql
if [ -s $icsfile ]
then
        cat $icsfile | grep -v iICSId | awk -v ser=$serid '{ printf("update tbICSComplaint set iICSAnaylseResult=-1,iICSAnaylseStat=-1 where iServiceID=%s and iID=%s and iICSId=\"%s\";\n",ser,$1,$2) }' > $updateics
fi

#3. update the ics status
if [ -s $updateics ]
then
	mysql -h10.134.139.215 -urobgame -probgame@123 -P3306 $dbname < $updateics
fi