group interfaces {
	nodes bridge : "Мостовой интерфейс" {
		type is name re "^br"
		class link-base
		class link-bridge
		class link-ethernet
		class link-inet
	}
}
