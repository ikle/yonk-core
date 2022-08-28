group interfaces : "Интерфейсы" {
	nodes bonding : "Агрегированный интерфейс" {
		match name "^bond"
		class link-base
		class link-bonding
		class link-ethernet
		class link-bridge-port
		class link-inet
	}
}
