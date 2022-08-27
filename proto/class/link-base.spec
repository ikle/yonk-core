attr description : "Описание интерфейса"

attr mtu : "Mаксимальный размер передаваемого блока данных" {
	number from 68 to 9000
}

attr link-detect = on : "Реагирование на изменение несущей" {
	value on  : "Включено"
	value off : "Выключено"
}
