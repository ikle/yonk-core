group policy : "Политики" {
	nodes dhcp : "Политика DHCP" {
		match name
		class policy-dhcp
	}
}

#
# NOTE: The domain-name can be used to filter DNS requests on interface if
# gateway = off and dns-server = on
#

class policy-dhcp {
	attr description : "Описание политики"

	attr gateway       = on  : "Принимать маршрут по-умолчанию"
	attr dns-server    = on  : "Принимать адрес DNS сервера"
	attr host-name     = on  : "Принимать имя узла"
	attr domain-name   = on  : "Принимать имя сети"

	attr mtu           = off : "Принимать MTU интерфейса"
	attr static-routes = on  : "Принимать постоянные маршруты"
	attr ntp-server    = off : "Принимать адрес NTP сервера"
	attr smtp-server   = off : "Принимать адрес SMTP сервера"
	attr pop3-server   = off : "Принимать адрес POP3 сервера"
	attr irc-server    = off : "Принимать адрес IRC сервера"

	attr server : "Адрес сервера" {
		match ip-target
	}
}

class link-dhcp {
	attrs address : "IP адрес узла с маской сети" {
		value dhcp : "Получить адрес по DHCP"
	}

	attr dhcp-policy : "Политика DHCP" {
		match name
		ref policy dhcp
	}
}
