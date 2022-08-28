class link-base {
	attr description : "Описание интерфейса"

	attr mtu : "Mаксимальный размер передаваемого блока данных" {
		number from 68 to 9000
	}

	attr link-detect = on : "Реагирование на изменение несущей" {
		value on  : "Включено"
		value off : "Выключено"
	}
}

class link-inet {
	attrs address : "IP адрес с маской сети" {
		value dhcp : "Получить адрес по DHCP"
		type is ip-masked
	}

	attr proxy-arp = off : "Проксирование ARP" {
		value on  : "Включено"
		value off : "Выключено"
	}
}
