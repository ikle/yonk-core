group interfaces : "Интерфейсы" {
	nodes ethernet : "Ethernet интерфейс" {
		match name "^eth"
		class link-base
		class link-ethernet-device
		class link-ethernet
		class link-bonding-port
		class link-bridge-port
		class link-inet
	}
}
