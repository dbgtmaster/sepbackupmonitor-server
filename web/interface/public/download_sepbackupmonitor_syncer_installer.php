#!/bin/bash

<?php
$httpBaseUrl = $_SERVER['REQUEST_SCHEME'] . '://' . $_SERVER['SERVER_NAME'] . '/' . $_SERVER['CONTEXT_PREFIX'];
?>

function errorMessage {
    echo "$1" >&2            
    exit "${2:-1}"  ## Return a code specified by $2 or 1 by default.
}

echo ""
echo "======================================="
echo " SEPBACKUPMONITOR-SYNCER INSTALLATION "
echo "======================================="
echo ""

## Variables for proxy:
#PROXY_HOSTNAME="$1"
#PROXY_USERNAME="$2"
#PROXY_PASSWORD="$3"

echo -n "Check if command php5 exists... "
if which php5 &> /dev/null; then
        echo "ok"
else
        echo "command php5 does not exists... install php5-cli or php5"
        exit
fi

echo -n "Check if command wget exists... "
if which wget &> /dev/null; then
        echo "ok"
else
        echo "command wget does not exists... install wget"
        exit
fi

clear

echo ""



# KundenID:
while [ true ]; do
	read -p "Kunden ID: " customerId
	customerId="`echo $customerId | sed 's/ *$//g' | sed 's/^ *//g'`"
	if [ ! -z "$customerId" ]; then
		break
	fi
done

# Interface Key
while [ true ]; do
        read -p "Kunden Interface Key: " customerInterfaceKey
        customerInterfaceKey="`echo $customerInterfaceKey | sed 's/ *$//g' | sed 's/^ *//g'`"
        if [ ! -z "$customerInterfaceKey" ]; then
                break
        fi
done

echo ""
echo "Download client..."
export https_proxy="$PROXY_HOSTNAME"
wget --no-check-certificate --proxy-user="$PROXY_USERNAME" --proxy-password="$PROXY_PASSWORD" -O client.tar "<?=$httpBaseUrl?>/sync_backup_status.php?action=get_newest_client&customerkey=$customerInterfaceKey&customerid=$customerId" 
if [ "$?" != "0" ]; then
	echo "Error downloading newest client version! Network reachable?? Proxy configured??"
	rm client.tar
	exit
fi

sleep 1;

if [ "`cat client.tar`" = "No permission to use this interface" ];  then
	echo "== ERROR: CustomerID or customer interface key wrong!"
        echo ""
        echo ""
	rm client.tar
	exit
fi

# Entpacken:
echo "Extract client..."
rm -Rf /usr/local/lib/sepbackupmonitor-syncer
mkdir -p /usr/local/lib/sepbackupmonitor-syncer
mv client.tar /usr/local/lib/sepbackupmonitor-syncer
cd /usr/local/lib/sepbackupmonitor-syncer || errorMessage "Cannot change into /usr/local/lib/sepbackupmonitor-syncer"
tar --strip 1 -xf /usr/local/lib/sepbackupmonitor-syncer/client.tar

if [ "$?" != "0" ]; then
        echo "Error extract newest client version!"
        exit
fi

rm /usr/local/lib/sepbackupmonitor-syncer/client.tar

echo "Create configuration file..."
echo "<?php" > /usr/local/lib/sepbackupmonitor-syncer/config.php
echo "define('HTTP_URL', '<?=$httpBaseUrl?>');" >> /usr/local/lib/sepbackupmonitor-syncer/config.php
echo "define('CUSTOMER_ID', '$customerId');" >> /usr/local/lib/sepbackupmonitor-syncer/config.php
echo "define('CUSTOMER_SYNC_KEY', '$customerInterfaceKey');" >> /usr/local/lib/sepbackupmonitor-syncer/config.php
if [ "$PROXY_HOSTNAME" != "" ]; then
	echo "define('PROXY_HOSTNAME', '$PROXY_HOSTNAME');" >> /usr/local/lib/sepbackupmonitor-syncer/config.php
fi
if [ "$PROXY_USERNAME" != "" ]; then
        echo "define('PROXY_USERNAME', '$PROXY_USERNAME');" >> /usr/local/lib/sepbackupmonitor-syncer/config.php
fi
if [ "$PROXY_PASSWORD" != "" ]; then
        echo "define('PROXY_PASSWORD', '$PROXY_PASSWORD');" >> /usr/local/lib/sepbackupmonitor-syncer/config.php
fi
echo "?>" >> /usr/local/lib/sepbackupmonitor-syncer/config.php
chmod 660 /usr/local/lib/sepbackupmonitor-syncer -R

echo "Create Binary Script /usr/local/local/bin/sepbackupmonitor-syncer"
echo "#!/bin/bash" > /usr/local/bin/sepbackupmonitor-syncer
echo "cd /usr/local/lib/sepbackupmonitor-syncer/; php5 -f sync.php; cd -" >> /usr/local/bin/sepbackupmonitor-syncer
chmod +x /usr/local/bin/sepbackupmonitor-syncer

echo "Install cronjob"
rm -f /etc/cron.hourly/sepbackupmonitor-syncer
echo "#!/bin/bash" > /etc/cron.hourly/sepbackupmonitor-syncer
echo "/usr/local/bin/sepbackupmonitor-syncer > /dev/null" >> /etc/cron.hourly/sepbackupmonitor-syncer
chmod +x /etc/cron.hourly/sepbackupmonitor-syncer

echo "Run syncer..."

# Um Update- Vorgang zu testen:
echo "0.0.0" > /usr/local/lib/sepbackupmonitor-syncer/VERSION
/usr/local/bin/sepbackupmonitor-syncer testSoftware
if [ "$?" != "0" ]; then
        echo "Error running sepbackupmonitor-syncer... Run the installer again!!"
        exit
fi
if [ "`cat /usr/local/lib/sepbackupmonitor-syncer/VERSION`" = "0.0.0" ]; then
        echo "Error running sepbackupmonitor-syncer... Softwareversion not updated!!"
        exit
fi


echo ""
echo "======================================="
echo "Installation finished... Try to run:"
echo "/usr/local/bin/sepbackupmonitor-syncer"
echo ""
echo "Installed sepbackupmonitor-syncer version is '"`cat /usr/local/lib/sepbackupmonitor-syncer/VERSION`"'"
echo "Cronjob installed: /etc/cron.hourly/sepbackupmonitor-syncer"
echo "======================================="
echo ""

