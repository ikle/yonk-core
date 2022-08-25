attr mac : "MAC адрес" {
	type is macaddr
}

nodes vlan : "Идентификатор виртуальной сети" {
	type is number from 1 to 4094
	class link-base
	class link-inet
}
