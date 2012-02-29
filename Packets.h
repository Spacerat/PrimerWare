enum PacketTypes {
	PACKET_NULL = 0,
	/* --- Menu packets --- */
	PACKET_requestGame,
	PACKET_ACKGame,
	PACKET_NextGame,

	/* --- Game packets --- */
	PACKET_gameWon,
	PACKET_gameFail,
	PACKET_roundFinish,
	PACKET_gameData
};


