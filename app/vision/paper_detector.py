import cv2
import numpy as np

class PaperDetector:
    def __init__(self, min_area=1000):
        self.min_area = min_area

    def detect(self, image):
        # Convert to grayscale
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        
        # Apply Gaussian blur
        blurred = cv2.GaussianBlur(gray, (5, 5), 0)
        
        # Threshold the image
        _, thresh = cv2.threshold(blurred, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
        
        # Find contours
        contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        
        # Filter contours by area
        paper_regions = []
        for contour in contours:
            area = cv2.contourArea(contour)
            if area > self.min_area:
                paper_regions.append(contour)
        
        return paper_regions 