group interfaces : "Интерфейсы" {
	nodes wireless : "Интерфейс беспроводной локальной сети" {
		match link-name "^wlan"
		class link-base, link-wlan
	}
}

class link-eapol {
	attrs auth-policy : "Политика аутентификации" {
		match name
		ref policy auth *
	}

	attr eapol-version = 2 : "Версия протокола EAPoL" {
		numder from 1 to 3
	}
}

class link-wlan {
	nodes peer : "Идентификатор сети (SSID)" {
		attrs auth-policy : "Политика аутентификации" {
			match name
			ref policy auth *
		}
	}

	attr wpa-version = 2 : "Версия протокола WPA" {
		numder from 1 to 3
	}

	class link-ethernet
}
