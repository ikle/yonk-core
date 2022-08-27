node bridge : "Сетевой мост" {
	type is name re "^br"
	ref interfaces bridge *

	attr cost : "Стоимость маршрута для данного порта" {
		number from 1 to 65535
	}

	attr priority : "Приоритет порта" {
		number from 0 to 63
	}
}
