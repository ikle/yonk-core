group policy : "Поилитики" {
	nodes dhcp : "Политика DHCP" {
		match name
		class policy-dhcp
	}
}

class policy-dhcp {
	attr default-route = on : "Принимать маршрут по-умолчанию"
	attr dns-server    = on : "Принимать адрес DNS сервера"
	attr host-name     = on : "Принимать имя узла"
	attr domain-name   = on : "Принимать имя сети"
	attr mtu           = on : "Принимать MTU интерфейса"
	attr static-routes = on : "Принимать постоянные маршруты"
	attr ntp-server    = on : "Принимать адрес NTP сервера"
	attr smtp-server   = on : "Принимать адрес SMTP сервера"
	attr pop3-server   = on : "Принимать адрес POP3 сервера"
	attr irc-server    = on : "Принимать адрес IRC сервера"
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
