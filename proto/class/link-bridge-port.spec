node bridge : "Сетевой мост" {
	ref interfaces bridge *

	attr cost : "Стоимость маршрута для данного порта" {
		type is number from 1 to 65535
	}

	attr priority : "Приоритет порта" {
		type is number from 0 to 63
	}
}
