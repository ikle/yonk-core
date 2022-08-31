class link-base {
	attr description : "Описание интерфейса"

	attr mtu : "Mаксимальный размер передаваемого блока данных" {
		number from 68 to 9000
	}

	attr link-detect = on : "Реагирование на изменение несущей"
}

class link-inet {
	attrs address : "IP адрес с маской сети" {
		type is ip-masked
	}

	attr proxy-arp = off : "Проксирование ARP"

	class link-dhcp
}
