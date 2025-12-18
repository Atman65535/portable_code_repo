from selenium.webdriver.common.by import By
from selenium import webdriver
import random
import time
import pyautogui

def filling_blanks(driver:webdriver.Chrome, path_answer_dict:dict):
    for key, ans in path_answer_dict.items():
        selection = driver.find_element(By.CSS_SELECTOR, "#"+key)
        if selection:
            selection.send_keys(ans)
            print(f"send {ans} to {key}")
        else:
            print(f"fuck node {key}")
        

def single_selection(driver:webdriver.Chrome, answer:str):
    selection = driver.find_element(By.CSS_SELECTOR, "#div5")
    choices = selection.find_elements(By.CSS_SELECTOR,"div.ui-controlgroup.column1 > div.ui-radio")
    for choice in choices:
        content = choice.find_element(By.CSS_SELECTOR,"div.label")
        if content.get_attribute("for") == answer:
            print(f"choose : {content.text}")
            content.click()

        else:
            print(f"dump : {content.text}")

def main():
    with open('../url.txt') as f_in:
        target_url = f_in.readline()
        test_url = f_in.readline()

    service = webdriver.ChromeService("../chromedriver-linux64/chromedriver")
    option = webdriver.ChromeOptions()
    option.add_experimental_option("excludeSwitches", ["enable-automation"])
    option.add_experimental_option("useAutomationExtension", False)

    option.binary_location = "/opt/google/chrome/chrome"

    driver = webdriver.Chrome(service=service, options=option)
    driver.execute_cdp_cmd("Page.addScriptToEvaluateOnNewDocument",
                           {"source":"Object.defineProperty(navigator, 'webdriver', {get: () => undefined})"})
    
    driver.get(test_url)

    blanks = dict(
        q1="BUAA SA",
        q2="atman",
        q3="138138",
        q4="1145141919810",
    )
    filling_blanks(driver=driver, path_answer_dict=blanks)
    single_selection(driver=driver, answer="q5_1")

    distance = 500
    js = "document.documentElement.scrollTop=" + str(distance)   
    driver.execute_script(js)

    # exit 
    finish = driver.find_element(By.CSS_SELECTOR, "#ctlNext")
    finish.click()

    print("finish a quiz!")



if __name__ == "__main__":
    main()