#Authors
# Palanivel Kodeswaran <palani.kodeswaran@in.ibm.com>
# Sayandeep Sen <sayandes@in.ibm.com>


# namespace ns1 -> veth1 40.0.1.2/24
# namespace ns2 -> veth2 40.0.2.2/24


# Add qdisc to interface
function add_qdisc {
    iface=$1
    echo "IFACE: ${iface} \n"
    tc qdisc add dev ${iface} clsact
}


function del_qdisc {
    iface=$1
    echo "IFACE: ${iface} \n"
    tc qdisc del dev ${iface} clsact
}


# Attach TC to interface
function attach_to_filter {
  iface=${1}
  prog=${2}
  sec=${3}
  sudo tc filter add dev ${iface} ingress bpf da obj ${prog} sec ${sec}
}

function clean_TC  {
    del_qdisc $1
}

# attach_at_TC <iface> <prog> <sec>
function attach_at_TC {
    add_qdisc $1
    attach_to_filter $1 $2 $3
}

#attach_at_XDP <prog> <sec>
function attach_at_XDP {
    echo "Hello"
    iface=${1}
    PROG=${2}
    SEC=${3}
    sudo ip link set ${iface} xdpgeneric obj ${PROG} sec ${SEC}
}

function clean_XDP {
    iface=${1}
    sudo ip link set ${iface} xdpgeneric off
}


function clean {
    sudo ip netns del ns1
    sudo ip netns del ns2
    sudo ip link del veth1
    sudo ip link del veth2
}


function setup_interfaces {
    for i in `seq 1 2`;
    do
	sudo ip netns del ns${i} > /dev/null 2>&1 # remove ns if already existed
	sudo ip link del veth${i} > /dev/null 2>&1

	sudo ip netns add ns${i}
	sudo ip link add veth${i}_ type veth peer name veth${i}
	sudo ip link set veth${i}_ netns ns${i}
	sudo ip netns exec ns${i} ip link set dev veth${i}_ up
	sudo ip netns exec ns${i} ip link set dev lo up
	sudo ip link set dev veth${i} up
	sudo ip netns exec ns${i} ifconfig veth${i}_ 40.0.${i}.2/24
  	sudo ifconfig veth${i} 40.0.${i}.1/24
	sudo ip netns exec ns${i} route add  default gw 40.0.${i}.1 veth${i}_
    done
}

 function start_nc_server {
    # run nc server in ns2 and scapy in ns1
    if [ -z ${1} ]
    then
	PORT=10000
    else
	PORT=${1}
    fi
    sudo ip netns exec ns2 nc -l -p ${PORT} &
}


function start_python_receiver {
    # run python server in ns2 and scapy in ns1
    if [ -z ${1} ]
    then
	PORT=10000
    else
	PORT=${1}
    fi
    sudo ip netns exec ns2 python3 ./pkt-gen/recv.py &
}

function start_python_sender {
    sudo ip netns exec ns1  python3 ./pkt-gen/send.py 
}


#attach_and_check <hookpoint> <prog> <sec>
function attach_only {
    #clean
    #setup_interfaces
 
    iface="veth2"
    read
    if [ $1 == "TC" ]; then
	attach_at_TC ${iface} $2 $3
	tcpdump -i ${iface} -vvv -e -u not arp and not icmp and not ip6 -w recv-tc.pcap &

    else
	attach_at_XDP ${iface} $2 $3
	ip link show ${iface}
	read
	tcpdump -i ${iface} -vvv -e  -u not arp and not icmp and not ip6 -w recv-xdp.pcap &

    fi

}

#attach_and_check <hookpoint> <prog> <sec>
function attach_and_check {
    echo "TODO"
    #clean
    setup_interfaces
 
    iface="veth2"
    if [ $1 == "TC" ]; then
	attach_at_TC ${iface} $2 $3
	tcpdump -i ${iface} -vvv -e -u not arp and not icmp and not ip6 -w recv-tc.pcap &

    else
	attach_at_XDP ${iface} $2 $3
	ip link show ${iface}
	read
	tcpdump -i ${iface} -vvv -e  -u not arp and not icmp and not ip6 -w recv-xdp.pcap &

    fi
    #read
    #sleep 2
    start_python_receiver 20000
    #sleep 2
    start_python_sender

    #wait for tcpdump to flush
    sleep 5
    killall tcpdump
}

#<script> <prog> <sec>
PROG_TC="./decap-transformed/extraction/extracted.o"
#demo_lpc
PROG_TC="/root/github/demo_lpc/codequery/extraction/extracted.o"
PROG_XDP="xdp_filter.o"
SEC="decap"

echo "Attaching at TC"
attach_only TC ${PROG_TC} ${SEC}
bpftool net
