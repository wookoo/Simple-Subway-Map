# Simple Subway Map

## 소개

안드로이드 스튜디오 기반으로 만든 지하철 어플 입니다.

C언어 소스코드 안에 p.c 파일에서 c언어로 구현한 다익스트라 소스를 확인할수 있습니다.


###파일 구조

메인 c언어 알고리즘위치
~~~
C언어 소스코드/p.c
~~~
메인 서버 알고리즘 위치
~~~
서버/main/views.py 의 message 함수
~~~
안드로이드 서버 통신 위치
~~~
Application/app/src/main/java/com/example/myapplication/LineInterface.java
~~~

### 알고리즘

1. 안드로이드 단에서 시작점 도착점 확인

2. 안드로이드 단에서 파이썬 서버단으로 시작점 도착점 전송

3. 파이썬 서버는 C언어로 구현한 최단거리 구하는 프로그램 실행 후 결과값 안드로이드에 전송

4. 안드로이드는 수신한 경로를 보여줌.




## 개발 환경 및 기술 정보

### 개발 운영체제

- Windows 10

### 언어

- Python (3.x 이상)

### 프레임 워크

- Django (2.0 이상)
- 안드로이드 스튜디오

### 파이썬 사용 라이브러리

- django
- ctypes
- json

ctypes 와 json 은 기본 지원 파이썬 라이브러리 입니다.

아래 명령으로 django 를 설치해주세요.
~~~
python -m pip install django
~~~

### 서버
- Windows10


## c언어 수정후 컴파일 하기 (옵션)

해당 프로그램에서 c언어 파일을 수정하고 수정된파일을 실행하기 위해선 gcc 를 설치해아 합니다.

[[설치]](https://like-tomato.tistory.com/16) 를 눌러 해당 블로그의 글을 따라해서 64비트 gcc 를 설치해주세요.

gcc 가 설치가 다 되었으면, "C언어 소스코드" 에서 cmd 를 실행 후 아래 명령을 입력해주세요.

아래 명령은 공유 가능한 dll 을 생성하는것 입니다.


~~~
gcc -fPIC -c p.c
gcc --shared -o p.dll p.o
~~~

해당 명령어로 컴파일이 완료되었으면, C언어 소스코드 디렉토리에 있는 p.dll 파일을

서버 폴더 안으로 옮겨주세요.


### 실행방법

어플을 실행하기 전 cmd 에서 파이썬 서버가 실행되어야 합니다.

cmd 를 서버 폴더에서 실행후 아래 명령어로 서버를 실행시켜주세요.

~~~
python manage.py runserver 0.0.0.0:5000
~~~

그후, Application/app/src/main/java/com/example/myapplication/LineInterface.java

파일에서 58번라인 74번 라인의 다음 코드를

~~~
getData("http://172.30.1.31:5000/message");
~~~

아래와 같이 바꿔주세요

~~~
getData("http://호스트컴퓨터IP:5000/message");
~~~

그후 어플리케이션을 실행하면 됩니다.
