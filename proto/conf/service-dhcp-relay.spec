group service : "Сервисы" {
	group dhcp-relay : "Сервис DHCP релея" {
		class service-dhcp-relay

		attrs listen-on : "Адрес на котором принимаются запросы" {
			match ip-listen
		}

		group server : "Параметры DHCP сервера" {
			attr address : "Адрес DHCP сервера" {
				match ip-target
			}

			attr interface : "Интерфейс доступа к DHCP серверу" {
				match link-name
				ref interfaces * *
			}
		}
	}
}
