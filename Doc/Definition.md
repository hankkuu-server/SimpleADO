# Main Server
1. DB에서 IP리스트 가져오기
   - DB에 사용자가 추가한 장비의 IP리스트를 가져옴
   - 추가 된 IP가 있는지 정기적으로 확인 
     * 전체의 리스트를 주기적으로 변화를 폴링해서 확인해서 업데이트 하는 방식으로 할건지?? 
     * 사용자가 장비추가시 바로 반영을 할 수 있는지? 
 
2. CS(Collect Server)에게 IP할당하기 (Mutiple Collect 상황 시)
   - 장비의 리스트를 균등하게 CS에게 할당
   * CS의 개수만큼 n등분하여 할당 (Round robin 시 하나하나 씩) 
 
3. CS의 작동여부 확인
   * CS에 문제 발생시 에러를 발생 및 기록 
   - Socket Establish 된 상태에서 이상 현상이 발생된다면? 
     * 문제 발생시 문제가 발생한 CS의 작업 할 부분을 다른 CS에게 할당

4. CS의 결과 수집
   - 각 CS에게 할당한 장비의 결과를 수신
   - 모인 결과데이터를 취합 

5. 최종 결과물 DB에 저장

# Collect Server
1. 장비 상태 확인
   - 할당 받은 IP에 최초 Ping으로 장비 상태 파악
   - 1분 단위로 Ping으로 상태 체크
 
2. 장비의 정보 수집
   - Ping 전송과 구성정보를 확인해서 OS에 알맞은 MIB 전송으로 현재 상태 요청
   - 결과값을 받으면 필요한 정보 추출(
``` 
등록된 장비의 CPU, Memory, 회선 사용량을 수집하여 DB에 저장한다. ( 수집 주기 : 1분 )
```
   - CPU 사용량
   - 회선 입출력 사용량
   - 메모리 사용률
			 
3. MS(Main Server)에게 결과보고
   - 각 스레드별로 수집한 장비 별 결과를 하나의 리스트로 수집
   * 쓰레드 수로 데이터 수집을 균등 분배할 경우 수집된 리스트를 MS(Main Server)에게 전달