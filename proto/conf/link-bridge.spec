group interfaces {
	nodes bridge : "Мостовой интерфейс" {
		match name "^br"
		class link-base
		class link-bridge
		class link-ethernet
		class link-inet
	}
}
