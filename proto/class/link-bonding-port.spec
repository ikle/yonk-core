node bonding : "Агрегированный интерфейс" {
	match name "^bond"
	ref interfaces bonding *

	attr port-id : "Номер порта 802.3ad" {
		number from 1 to 65535
	}
}
