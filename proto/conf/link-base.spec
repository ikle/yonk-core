require ip

class link-base {
	attr description : "Описание интерфейса"

	attr mtu : "Mаксимальный размер передаваемого блока данных" {
		number from 68 to 9000
	}

	attr link-detect = on : "Реагирование на изменение несущей"
}

class link-ethernet {
	attr mac : "MAC адрес" {
		match macaddr
	}

	nodes vlan : "Идентификатор виртуальной сети" {
		number from 1 to 4094
		class link-base, link-inet
	}

	class link-inet
}

class link-inet {
	attrs address : "IP адрес узла с маской сети" {
		match ip-masked
	}

	attr proxy-arp = off : "Проксирование ARP"

	class link-dhcp
}
