group service : "Сервисы" {
	group dhcp-relay : "Сервис DHCP релея" {
		class service-dhcp-relay

		attrs listen-on : "Адрес на котором принимаются запросы" {
			type is ip-listen
		}

		group server : "Параметры DHCP сервера" {
			attr address : "Адрес DHCP сервера" {
				type is ip-address
			}

			attr interface : "Интерфейс доступа к DHCP серверу" {
				match name
				ref interfaces * *
			}
		}
	}
}
