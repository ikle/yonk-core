require wpa_supplicant

class link-eapol {
	attrs auth-policy : "Политика аутентификации" {
		match name
		ref policy auth *
	}

	attr eapol-version = 2 : "Версия протокола EAPoL" {
		numder from 1 to 3
	}
}
