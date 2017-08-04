--#ENDPOINT POST /device/masterSet
response.message = {}

status, resp = request_inspector.has_auth(request.headers)
print(tostring(status) .. resp)

if status then
  for k, v in pairs(request.body) do
  response.message[k] = Lampy.setIdentityState({
    identity = "90:A2:DA:F3:1E:1E",
    print(k),
    print(v),
    [k] = tonumber(v),
    p = 1
  })
end
else
  response.code = 400
  response.message = message
end

response.message["Lock State"] = "LOCKED"
return response.message
