--#ENDPOINT POST /device/setValues
response.message = {}

if Lampy.getIdentityState({identity = "90:A2:DA:F3:1E:1E"}).p.set == 0 then
  for k, v in pairs(request.body) do
    response.message[k] = Lampy.setIdentityState({
      identity = "90:A2:DA:F3:1E:1E",
      print(k),
      print(v),
      [k] = tonumber(v)
    })
  end
  response.message["Lock State"] = "UNLOCKED"
else
  response.message["Lock State"] = "LOCKED"
end

return response.message
