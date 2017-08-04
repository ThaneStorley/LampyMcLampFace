--#ENDPOINT POST /lock
response.message = {}

status, resp = request_inspector.has_auth(request.headers)
print(tostring(status) .. resp)

if status then
  response.message["Set"] = Lampy.setIdentityState({
    identity = "90:A2:DA:F3:1E:1E",
    p = 1
  })
else
  response.code = 400
  response.message = message
end

response.message["Lock State"] = "LOCKED"
return response.message
