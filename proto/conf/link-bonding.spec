group interfaces : "Интерфейсы" {
	nodes bonding : "Агрегированный интерфейс" {
		match name "^bond"
		class link-base, link-bonding, link-ethernet
	}
}

class class link-bonding {
	attr mode = 802.3ad : "Режим агрегирования" {
		value 802.3ad       : "Динамическое агрегирование IEEE 802.3ad"
		value active-backup : "Отказоустойчивость, активный только slave"
		value broadcast     : "Отказоустойчивость, все данные передавать на всех интерфейсах slave"
		value round-robin   : "Балансировка, передавать пакеты последовательно"
		value transmit-load-balance : "Балансировка на основе скорости передачи"
		value adaptive-load-balance : "Балансировка на основе скорости передачи и приема, плюс ARP"
		value xor-hash      : "Балансировка, распределение на основе MAC адреса"
	}

	attr hash-mode = layer2 : "Политика хеширования в режимах xor-hash, 802.3ad и transmit-load-balance {
		value layer2   : "Хеш на основе протокола 2 уровня"
		value layer2+3 : "Хеш на основе протоколов 2 и 3 уровней"
		value layer3+4 : "Хеш на основе протоколов 3 и 4 уровней"
		value encap2+3 : "Хеш на основе протоколов 2 и 3 уровней с разбором инкапсуляции"
		value encap3+4 : "Хеш на основе протоколов 3 и 4 уровней с разбором инкапсуляции"
	}

	attr lacp-rate = slow : "Частота LACP запросов" {
		value slow : "1 раз в 30 секунд"
		value fast : "1 раз в секунду"
	}

	attr primary : "Главное устройство" {
		match name "^eth"
		ref interfaces ethernet *  # must be part of this bonding group
	}
}

class class link-bonding-port {
	node bonding : "Агрегированный интерфейс" {
		match name "^bond"
		ref interfaces bonding *

		attr port-id : "Номер порта 802.3ad" {
			number from 1 to 65535
		}
	}
}
