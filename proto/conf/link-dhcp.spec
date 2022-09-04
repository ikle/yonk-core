group policy : "Политики" {
	nodes dhcp : "Политика DHCP" {
		match name
		class policy-dhcp
	}
}

class policy-dhcp {
	attr description : "Описание политики"

	attr gateway       = on  : "Принимать маршрут по-умолчанию"
	attr dns-server    = on  : "Принимать адрес DNS сервера"
	attr host-name     = on  : "Принимать имя узла"

#	domain-name requested and accepted if host-name = on; domain-name
#	requested if gateway = off and dns-server = on to filter DNS requests

	attr mtu           = off : "Принимать MTU интерфейса"
	attr static-routes = on  : "Принимать постоянные маршруты"
	attr ntp-server    = off : "Принимать адрес NTP сервера"
	attr smtp-server   = off : "Принимать адрес SMTP сервера"
	attr pop3-server   = off : "Принимать адрес POP3 сервера"
	attr irc-server    = off : "Принимать адрес IRC сервера"

	attr server : "Адрес сервера" {
		type ipv4-address
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
