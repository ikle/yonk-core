attrs address : "IP адрес с маской сети" {
	value dhcp : "Получить адрес по DHCP"
	type is ip-masked
}

attr proxy-arp = off : "Проксирование ARP" {
	value on  : "Включено"
	value off : "Выключено"
}
