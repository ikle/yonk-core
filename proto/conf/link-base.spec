class link-base {
	attr description : "Описание интерфейса"

	attr mtu : "Mаксимальный размер передаваемого блока данных" {
		number from 68 to 9000
	}

	attr link-detect = on : "Реагирование на изменение несущей"
}

class link-ethernet {
	attr mac : "MAC адрес" {
		type is macaddr
	}

	nodes vlan : "Идентификатор виртуальной сети" {
		number from 1 to 4094
		class link-base, link-inet
	}

	class link-inet
}

class link-inet {
	attrs address : "IP адрес с маской сети" {
		type is ip-masked
	}

	attr proxy-arp = off : "Проксирование ARP"

	class link-dhcp
}
