group interfaces : "Интерфейсы" {
	nodes bridge : "Мостовой интерфейс" {
		match name "^br"
		class link-base, link-bridge, link-ethernet
	}
}

class link-bridge {
	attr ageing = 300 : "Время хранения MAC адресов" {
		number from 1 to 86400
	}

	attr forwarding-delay = 15 : "Задержка перенаправления после изменения топологии" {
		number from 2 to 30
	}

	attr hello-time = 2 : "Интервал посылки hello-пакета" {
		number from 1 to 10
	}

	attr priority = 32768 : "Приоритет для данного моста" {
		number from 0 to 65535
	}

	attr stp = off : "Поддержка протокола STP" {
		value on  : "Включено"
		value off : "Выключено"
	}
}

class link-bridge-port {
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
}
