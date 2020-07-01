	• 기능 구분 
		○ Manager 
			§ 장비
				§ 모니터링 장비 확인 (Get - DB)
				§ 모니터링 장비 수정 (Set -DB)
			§ DB 연결
				§ Query
					® Select / Insert / Update / Delete
					® SP
			§ Socket
				§ Connect Collector
				§ Send / Receive Data to Collector
				§ Managing Collector like disconneting and reconneting
		○ Collector 
			§ 장비
				§ SNMP로 성능정보 수집
				§ SNMP로 Ping 상태 확인 
				§ (보류) 성능 정보 이상시 서버로(Manager) 알람 Push or Event 처리 
			§ Socket 
				§ Connect Manager
				§ Send / Receive Data to Manager
				§ Close Socket (TCP)
				
	• 요구 사항
		○ 기본 기능 
			§ 소켓을 이용한 서버와 클라이언트의 구분과 통신이 가능해야 한다 
		○ 장애 관리 
			§ Ping을 이용한 장애 감지와 상태 이벤트를 DB에 기록한다 
		○ 성능 관리
			§ 등록된 장비의 CPU, Memory, 회선 사용량을 수집하여 DB에 저장한다. ( 수집 주기 : 1분 )
			§ MIB 값을 이용해 자동으로 성능 수집을 하도록 한다
		○ 구성 관리
			§ 장비 추가 시 자동으로 시스템 description과 같은 정보를 수집한다
