void differenceToTime(long timeDifference){
  long minute = (int) timeDifference / 60000; // to minute
  long second = (int) (timeDifference % 60000) / 1000;
  Serial.print(minute); Serial.print(" Minutes "); Serial.print(second); Serial.println(" Seconds.");
  
}
