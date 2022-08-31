group policy : "Поилитики" {
	nodes dhcp : "Политика DHCP" {
		match name
		class policy-dhcp
	}
}

class policy-dhcp {
	attr default-route = on : "Принимать маршрут по-умолчанию" {
		value on  : "Включено"
		value off : "Выключено"
	}

	attr dns-server = on : "Принимать адрес DNS сервера" {
		value on  : "Включено"
		value off : "Выключено"
	}

	attr host-name = on : "Принимать имя узла" {
		value on  : "Включено"
		value off : "Выключено"
	}

	attr domain-name = on : "Принимать имя сети" {
		value on  : "Включено"
		value off : "Выключено"
	}

	attr mtu = on : "Принимать MTU интерфейса" {
		value on  : "Включено"
		value off : "Выключено"
	}

	attr static-routes = on : "Принимать постоянные маршруты" {
		value on  : "Включено"
		value off : "Выключено"
	}

	attr ntp-server = on : "Принимать адрес NTP сервера" {
		value on  : "Включено"
		value off : "Выключено"
	}

	attr smtp-server = on : "Принимать адрес SMTP сервера" {
		value on  : "Включено"
		value off : "Выключено"
	}

	attr pop3-server = on : "Принимать адрес POP3 сервера" {
		value on  : "Включено"
		value off : "Выключено"
	}

	attr irc-server = on : "Принимать адрес IRC сервера" {
		value on  : "Включено"
		value off : "Выключено"
	}
}

class link-dhcp {
	attrs address : "IP адрес с маской сети" {
		value dhcp : "Получить адрес по DHCP"
	}

	attr dhcp-policy : "Политика DHCP" {
		match name
		ref policy dhcp
	}
}
