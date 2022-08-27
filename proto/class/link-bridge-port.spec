node bridge : "Сетевой мост" {
	type is name re "^br"
	ref interfaces bridge *

	attr cost : "Стоимость маршрута для данного порта" {
		type is number from 1 to 65535
	}

	attr priority : "Приоритет порта" {
		type is number from 0 to 63
	}
}
