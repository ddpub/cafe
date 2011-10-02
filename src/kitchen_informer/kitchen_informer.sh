#/bin/bash
export ORACLE_HOME=/opt/instantclient_11_1
export NLS_LANG
export LD_LIBRARY_PATH=/opt/instantclient_11_1

PIDFILE=/var/local/ksi/run/kitchen_informer.pid
KI_BIN=/usr/local/ksi/bin/kitchen_informer
KI_CONFIG=/usr/local/ksi/etc/kitchen_informer.conf
export LD_LIBRARY_PATH=/opt/instantclient_11_1:${LD_LIBRARY_PATH}


if [[ -f $PIDFILE ]]; then
    pid=`cat $PIDFILE`
    if [[ ! `ps $pid | wc -l` -eq 1 ]]; then
	echo "${KI_BIN} is running"
	exit 1
    fi
fi

echo -n "$$" >> $PIDFILE

echo "BEGIN"; date
${KI_BIN} --config-file=${KI_CONFIG}
echo "END"; date

rm $PIDFILE
