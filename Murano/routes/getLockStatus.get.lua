--#ENDPOINT GET /getLockStatus
response.message = {}

if Lampy.getIdentityState({identity = "90:A2:DA:F3:1E:1E"}).p.set == 0 then
  response.message["Lock State"] = "UNLOCKED"
else if Lampy.getIdentityState({identity = "90:A2:DA:F3:1E:1E"}).p.set == 1 then
  response.message["Lock State"] = "LOCKED"
else
  response.message["Lock State"] = Lampy.getIdentityState({identity = "90:A2:DA:F3:1E:1E"}).p.set
end
end

return response.message
