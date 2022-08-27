node bridge : "Сетевой мост" {
	match name "^br"
	ref interfaces bridge *

	attr cost : "Стоимость маршрута для данного порта" {
		number from 1 to 65535
	}

	attr priority : "Приоритет порта" {
		number from 0 to 63
	}
}
