group system : "Системные параметры" {
	group ntp : "Настройка серверов NTP для получения времени" {
		class system-ntp

		attrs server : "DNS имя или IP адрес сервера" {
			match idn or ip-address
		}

		attr sync-on-start = off : "Синхронизировать при старте"
	}
}

group service : "Сервисы" {
	group ntp : "Сервис NTP" {
		class service-ntp

		attrs listen-on : "Адрес на котором принимаются запросы" {
			match ip-listen
		}

		attr stratum : "Часовой слой" {
			number from 1 to 16
		}
	}
}
