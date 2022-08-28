group system : "Системные параметры" {
	group ntp : "Настройка серверов NTP для получения времени" {
		class system-ntp

		attrs server : "DNS имя или IP адрес сервера" {
			type is idn, ip-address
		}

		attr step-at-start = off : "Синхронизировать при старте" {
			value on  : "Включено"
			value off : "Выключено"
		}
	}
}

group service : "Сервисы" {
	group ntp : "Сервис NTP" {
		class service-ntp

		attrs listen-on : "Адрес на котором принимаются запросы" {
			type is ip-listen
		}

		attr stratum : "Часовой слой" {
			number from 1 to 16
		}
	}
}
