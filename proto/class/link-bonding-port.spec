node bonding : "Агрегированный интерфейс" {
	type name re "^bond"
	ref interfaces bonding *

	attr port-id : "Номер порта 802.3ad" {
		type is number from 1 to 65535
	}
}
