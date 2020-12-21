from django.shortcuts import render
from django.views.decorators.csrf import csrf_exempt
from django.http import JsonResponse
import ctypes
import json
@csrf_exempt
def message(request):

    answer = ((request.body).decode('utf-8'))
    return_json_str = json.loads(answer)
    start = return_json_str["start"] #사용자가 보낸 시작점
    end = return_json_str["end"] #사용자가 보낸 도착점
    start = start.encode("euc-kr")
    end = end.encode("euc-kr") #시작점 종점을 인코딩
    p = ctypes.CDLL('./p.dll') #c로 작성한 dijkstra 함수 컴파일된걸 불러오기
    p.dijkstra.argtypes = [ctypes.c_char_p,ctypes.c_char_p] #파이썬에서 c로 값을 전송해야함으로 파라메터 타입 설정
    p.dijkstra.restype = ctypes.c_char_p #반환값은 string 이므로 반환값 타입 설정
    result = p.dijkstra(start,end).decode("euc-kr") #다익스트라 실행 결과후 날라온 string 디코드

    return JsonResponse({"result":result}) #받아온 결과값 안드로이드에 전송
