group interfaces {
	nodes bonding : "Агрегированный интерфейс" {
		type name re "^bond"
		class link-base
		class link-bonding
		class link-ethernet
		class link-bridge-port
		class link-inet
	}
}
