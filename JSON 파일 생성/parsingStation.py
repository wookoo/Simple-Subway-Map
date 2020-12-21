from selenium import webdriver
import time
dr = webdriver.Chrome("chromedriver")
dr.get('https://namu.wiki/w/%EC%9D%B8%EC%B2%9C%20%EB%8F%84%EC%8B%9C%EC%B2%A0%EB%8F%84%201%ED%98%B8%EC%84%A0/%EC%97%AD%20%EB%AA%A9%EB%A1%9D')
for i in range(3,60):
    path = """//*[@id="app"]/div/div[2]/article/div[3]/div[2]/div/div/div[5]/table/tbody/tr[%d]/td[3]"""%i
    t = dr.find_element_by_xpath(path)
    t = t.text
    t = t.replace("　","")
    print(t)
