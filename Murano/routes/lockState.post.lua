--#ENDPOINT POST /lockState
response.message = {}

status, message = request_inspector.has_auth(request.headers)
print(tostring(status) .. message)

if status then
  for k, v in pairs(request.body) do
    response.message[k] = Lampy.setIdentityState({
      identity = "90:A2:DA:F3:1E:1E",
      print(k),
      print(v),
      [k] = tonumber(v)
    })
  end
else
  response.code = 400
  response.message = message
end

return response.message
