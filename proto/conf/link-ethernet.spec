group interfaces : "Интерфейсы" {
	nodes ethernet : "Ethernet интерфейс" {
		match link-name "^eth"
		class link-base, link-ethernet-device
	}
}

class link-ethernet-device {
	attr speed = auto : "Скорость физического соединения" {
		value auto  : "Автоматическое определение"
		value 10    : "10 Мбит/с"
		value 100   : "100 Мбит/с"
		value 1000  : "1 Гбит/с"
		value 2500  : "2,5 Гбит/с"
		value 10000 : "10 Гбит/с"
	}

	attr duplex = auto : "Режим дуплекса" {
		value auto : "Автоматическое определение"
		value half : "Полудуплекс"
		value full : "Полный дуплекс"
	}

	attr flow-control = on : "Управление потоком кадров Ethernet"

	class link-eapol, link-bonding-port, link-bridge-port, link-ethernet
}
