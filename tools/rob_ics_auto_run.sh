#!/bin/bash
source rob.sh.marcos
ROB_SERVICE_LIB_PATH=/usr/local/games/robgame/bin/${ROB_SERVICE_NAME}/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${ROB_SERVICE_LIB_PATH}
usage()
{
	echo "-------------Welcome To RobSystem------------"
	echo ""
	echo "**********************************************"
	echo "Usage"
	echo "**********************************************"
	echo ""
	echo "�÷���"
	echo "$0 -1 dnf2"
	echo "$0 -����ģ�� ҵ����"
	echo "���磺eg."
	echo "$0 -1 ${ROB_SERVICE_NAME}"
	echo "��ʾ����˼�ǣ�ִ��${ROB_SERVICE_NAME}ҵ��ĵ�һ������ģ��"
}
robFliterModel()
{
	echo "[INFO]: robFliterModel() RUNNINGIX------"
	./rob_core_fliter
	if [ $? -ne 0 ];then
		echo "[ERROR] ./rob_core_fliter Not Exit 0,It is [${?}]"
		exit $?
	fi
	proc_time=`date +%s`
	proc_time=`expr $proc_time - $START_TIME`
	echo "[INFO] ./rob_core_fliter Expire ${proc_time}"
}
robRobSlotModel()
{
	echo "[INFO]:robRobSlotModel() RUNNING--- ---"
	./rob_core_loginlogout
	if [ $? -ne 0 ];then
		echo "[ERROR] ./rob_core_loginlogout Not Return 0,It is [${?}]"
		exit $?
	fi
	proc_time=`date +%s`
	proc_time=`expr $proc_time - $START_TIME`
	echo "[INFO] ./rob_core_loginlogout Expire ${proc_time}"
}
robItemFlowModel()
{
	echo "[INFO]:robItemFlowModel() RUNNING--- ---"
	./rob_core_item_flow
	if [ $? -ne 0 ];then
		echo "[ERROR] .rob_core_item_flow Not Return 0,It is [${?}]"
		exit $?
	fi
	proc_time=`date +%s`
	proc_time=`expr $proc_time - $START_TIME`
	echo "[INFO] ./rob_core_item_flow Expire ${proc_time}"

}
robItemTraceModel()
{
	echo "[INFO]:robItemTraceModel() RUNNING--- ---"
	./rob_core_item_trace
	if [ $? -ne 0 ];then
		echo "[ERROR] .rob_core_item_trace Not Return 0,It is [${?}]"
		exit $?
	fi
	proc_time=`date +%s`
	proc_time=`expr $proc_time - $START_TIME`
	echo "[INFO] ./rob_core_item_trace Expire ${proc_time}"
}
robItemSendModel()
{
	echo "[INFO]:robItemSendModel() RUNNING--- ---"
	./rob_core_item_send
	if [ $? -ne 0 ];then
		echo "[ERROR] ./rob_core_item_send Not Return 0,It is [${?}]"
		exit $?
	fi
	proc_time=`date +%s`
	proc_time=`expr $proc_time - $START_TIME`
	echo "[INFO] ./rob_core_item_send Expire ${proc_time}"
}
robReplySysMode()
{
	echo "[INFO]:robReplySysMode() RUNNING--- ---"
	./rob_core_replay_auto
	if [ $? -ne 0 ];then
		echo "[ERROR] ./rob_core_replay_auto Not Return 0,It is [${?}]"
		exit $?
	fi
	proc_time=`date +%s`
	proc_time=`expr $proc_time - $START_TIME`
	echo "[INFO] ./rob_core_replay_auto Expire ${proc_time}"
}

robItemOfflineModel()
{
	echo "[INFO]:robItemOfflineModel() RUNNING--- ---"
	./rob_core_offline_module
	if [ $? -ne 0 ];then
		echo "[ERROR] ./rob_core_offline_module Not Return 0,It is [${?}]"
		#exit $?
	fi
	proc_time=`date +%s`
	proc_time=`expr $proc_time - $START_TIME`
	echo "[INFO] ./rob_core_offline_module Expire ${proc_time} return_value[$?]"
	exit $?
}
RUN_TYPE=$1
RUN_SERVICE=$2
START_TIME=`date +%s`

if [ $# -ne 2 -o "${RUN_SERVICE}" != "dnf2" ];then
	echo "[ERROR] Params Nums Is Must 2, Or Your Service Name Must 'dnf2'"
	exit -1
fi
case ${RUN_TYPE} in

-1)
	robFliterModel
	;;
-2)
	robRobSlotModel
	;;
-3)
	robItemFlowModel
	;;
-4)
	robItemTraceModel
	;;
-5)
	robItemSendModel
	;;
-6)
	robReplySysMode
	;;
-7)
	robItemOfflineModel
	;;
*)
	echo "[ERROR]: Wrong Type Of Running"
	;;

esac
