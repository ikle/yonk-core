group policy : "Поилитики" {
	nodes auth "Политика аутентификации" {
		match name
		class policy-auth
	}
}

class policy-auth {
	attr description : "Описание политики"
	attr tunnel-nai  : "Идентификатор сетевого доступа туннеля"
	attr nai         : "Идентификатор сетевого доступа (NAI)"
	attr peer-nai    : "Идентификатор сетевого доступа удалённой стороны"

	attr key-format = plain : "Формат ключа" {
		value base64     : "Кодировка передачи Base64, RFC 2045"
		value hex-dump   : "Шестнадцатеричный дамп"
		value hex-number : "Шестнадцатеричное число"
		value plain      : "Строка печатных символов"
	}

	attr key : "Пароль, ключ или ключевая фраза"

	attr cert : "Имя X.509 сертификата" {
		ref pki ca * certificate *
	}

	attrs proto : "Разрешённый протокол аутентификации" {
		value chap-md5          : "CHAP-MD5, RFC 1994"
		value chap-sha1         : "CHAP-SHA1"
		value chap-sha2-256     : "CHAP-SHA2-256"
		value chap-sha3-256     : "CHAP-SHA3-256"
		value wpa-psk           : "WPA-PSK"
		value eap-md5           : "EAP-MD5"
		value eap-psk           : "EAP-PSK, RFC 4764"
		value eap-sake          : "EAP-SAKE, RFC 4763"
		value eap-gpsk          : "EAP-GPSK, RFC 5433"
		value eap-tls           : "EAP-TLS"
		value eap-peap-md5      : "EAP-PEAP/EAP-MD5"
		value eap-peap-gtc      : "EAP-PEAP/EAP-GTC"
		value eap-peap-mschapv2 : "EAP-PEAP/EAP-MSCHAPv2"
		value eap-peap-tls      : "EAP-PEAP/EAP-TLS"
		value eap-ttls-md5      : "EAP-TTLS/EAP-MD5"
		value eap-ttls-gtc      : "EAP-TTLS/EAP-GTC"
		value eap-ttls-tls      : "EAP-TTLS/EAP-TLS"
		value eap-ttls-pap      : "EAP-TTLS/PAP"
		value eap-ttls-chap     : "EAP-TTLS/CHAP"
		value eap-ttls-mschap   : "EAP-TTLS/MSCHAP"
		value eap-ttls-mschapv2 : "EAP-TTLS/MSCHAPv2"
	}
}
