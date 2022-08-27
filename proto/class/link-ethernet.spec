attr mac : "MAC адрес" {
	type is macaddr
}

nodes vlan : "Идентификатор виртуальной сети" {
	number from 1 to 4094
	class link-base
	class link-inet
}
