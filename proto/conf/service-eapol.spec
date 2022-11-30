group service : "Сервисы" {
	nodes eapol : "Сервис EAPoL" {
		match name
		class eapol-service
	}
}

class eapol-service {
	attrs listen-on : "Иинтерфейс на котором принимаются запросы" {
		match name
		ref interfaces ethernet *
	}

	attr proto = 2 : "Версия протокола" {
		number from 1 to 2
	}

	attr reauth = 3600 : "Период переаутентификации в секундах" {
		value off : "Переаутентификация отключена"
		number from 1 to 86400
	}

	attr pae-group = on : "Использовать групповой PAE адрес" {
		value on  : "Использовать групповой адрес"
		value off : "Использовать целевой адрес"
	}

	group nas : "Параметры сервера доступа" {
		attr ident : "NAS-Identifier"

		attr own-ip : "NAS-IP-Address" {
			type is ipv4-address
		}

		attr address : "Адрес RADIUS сервера" {
			type is ip-target
		}

		attr auth-port = radius : "Порт аутентификации" {
			type is ip-service
		}

		attr auth-key : "Ключ доступа к сервису аутентификации"

		attr acct-port = radius-acct : "Порт регистрации" {
			type is ip-service
		}

		attr acct-key : "Ключ доступа к сервису регистрации"
	}
}
